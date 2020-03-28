//
// Created by anonymus-raccoon on 2/14/20.
//

#include "CPUDebug.hpp"
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidOpcode.hpp"
#include "../CPU/CPU.hpp"
#include <QtEvents>
#include <QPainter>
#include <iostream>
#include <utility>

using namespace ComSquare::CPU;

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(CPU &basicCPU, SNES &snes)
		: CPU(basicCPU),
		_window(new ClosableWindow<CPUDebug>(*this, &CPUDebug::disableDebugger)),
		_ui(),
		_model(*this),
		_painter(*this),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);

		this->_updateDisassembly(0xFFFF - this->_registers.pc); //Parse the first page of the ROM (the code can't reach the second page without a jump).
		this->_ui.disassembly->setModel(&this->_model);
		this->_ui.disassembly->horizontalHeader()->setStretchLastSection(true);
		this->_ui.disassembly->resizeColumnsToContents();
		this->_ui.disassembly->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
		this->_ui.disassembly->verticalHeader()->setHighlightSections(false);
		this->_ui.disassembly->setItemDelegate(&this->_painter);

		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		QMainWindow::connect(this->_ui.actionStep, &QAction::triggered, this, &CPUDebug::step);
		QMainWindow::connect(this->_ui.actionNext, &QAction::triggered, this, &CPUDebug::next);
		QMainWindow::connect(this->_ui.clear, &QPushButton::released, this, &CPUDebug::clearHistory);
		QMainWindow::connect(this->_ui.disassembly->verticalHeader(), &QHeaderView::sectionClicked, this, &CPUDebug::toggleBreakpoint);
		this->_window->show();
		this->_updateRegistersPanel();
	}

	bool CPUDebug::isDebugger()
	{
		return true;
	}

	void CPUDebug::disableDebugger()
	{
		this->_snes.disableCPUDebugging();
	}

	unsigned CPUDebug::update()
	{
		try {
			unsigned cycles = 0;

			if (this->_isPaused)
				return 0xFF;
			if (this->_isStepping) {
				cycles = this->_executeInstruction(this->readPC());
				this->_updateDisassembly();
				return cycles;
			}

			for (int i = 0; i < 0xFF; i++) {
				auto breakpoint = std::find_if(this->breakpoints.begin(), this->breakpoints.end(), [this](Breakpoint &brk) {
					return brk.address == this->_registers.pac;
				});
				if (i != 0 && breakpoint != this->breakpoints.end()) {
					if (breakpoint->oneTime)
						this->breakpoints.erase(breakpoint);
					this->_isPaused = false;
					this->pause();
					return cycles;
				}
				cycles += this->_executeInstruction(this->readPC());
			}
			return cycles;
		} catch (InvalidOpcode &e) {
			if (!this->_isPaused)
				this->pause();
			std::cout << "Invalid Opcode: " << e.what() << std::endl;
			return 0xFF;
		}
	}

	unsigned CPUDebug::_executeInstruction(uint8_t opcode)
	{
		if (this->_isPaused)
			return 0;
		if (this->_isStepping) {
			this->_isStepping = false;
			this->_isPaused = true;
		}
		uint24_t pc = (this->_registers.pbr << 16u) | (this->_registers.pc - 1u);
		DisassemblyContext ctx = this->_getDisassemblyContext();
		DisassembledInstruction instruction = this->_parseInstruction(pc, ctx);
		this->_ui.logger->append((instruction.toString() + " -  " + Utility::to_hex(opcode)).c_str());
		unsigned ret = CPU::_executeInstruction(opcode);
		this->_updateRegistersPanel();
		return ret;
	}

	void CPUDebug::pause()
	{
		this->_isPaused = !this->_isPaused;
		if (this->_isPaused)
			this->_ui.actionPause->setText("Resume");
		else
			this->_ui.actionPause->setText("Pause");
		this->_updateDisassembly();
	}

	void CPUDebug::step()
	{
		this->_isStepping = true;
		this->_isPaused = false;
	}

	void CPUDebug::next()
	{
		auto next = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(), [this](DisassembledInstruction &i) {
			return i.address > this->_registers.pac;
		});
		this->breakpoints.push_back({next->address, true});
		this->_isPaused = false;
	}

	void CPUDebug::toggleBreakpoint(int logicalIndex)
	{
		DisassembledInstruction instruction = this->disassembledInstructions[logicalIndex];
		auto existing = std::find_if(this->breakpoints.begin(), this->breakpoints.end(), [instruction](Breakpoint &i) {
			return i.address == instruction.address;
		});
		if (existing == this->breakpoints.end())
			this->breakpoints.push_back({instruction.address, false});
		else
			this->breakpoints.erase(existing);
		this->_ui.disassembly->viewport()->repaint();
	}

	void CPUDebug::_updateRegistersPanel()
	{
		if (!this->_registers.p.m)
			this->_ui.accumulatorLineEdit->setText(Utility::to_hex(this->_registers.a).c_str());
		else
			this->_ui.accumulatorLineEdit->setText(Utility::to_hex(this->_registers.al).c_str());
		this->_ui.programBankRegisterLineEdit->setText(Utility::to_hex(this->_registers.pbr).c_str());
		this->_ui.programCounterLineEdit->setText(Utility::to_hex(this->_registers.pc).c_str());
		this->_ui.directBankLineEdit->setText(Utility::to_hex(this->_registers.dbr).c_str());
		this->_ui.directPageLineEdit->setText(Utility::to_hex(this->_registers.d).c_str());
		this->_ui.stackPointerLineEdit->setText(Utility::to_hex(this->_registers.s).c_str());
		if (!this->_registers.p.x_b) {
			this->_ui.xIndexLineEdit->setText(Utility::to_hex(this->_registers.x).c_str());
			this->_ui.yIndexLineEdit->setText(Utility::to_hex(this->_registers.y).c_str());
		} else {
			this->_ui.xIndexLineEdit->setText(Utility::to_hex(this->_registers.xl).c_str());
			this->_ui.yIndexLineEdit->setText(Utility::to_hex(this->_registers.yl).c_str());
		}
		this->_ui.flagsLineEdit->setText(this->_getFlagsString().c_str());
		this->_ui.emulationModeCheckBox->setCheckState(this->_isEmulationMode ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
	}

	std::string CPUDebug::_getFlagsString()
	{
		std::string str;
		str += this->_registers.p.n ? "n" : "-";
		str += this->_registers.p.v ? "v" : "-";
		str += this->_registers.p.m ? "m" : "-";
		if (this->_isEmulationMode)
			str += this->_registers.p.x_b ? "b" : "-";
		else
			str += this->_registers.p.x_b ? "x" : "-";
		str += this->_registers.p.d ? "d" : "-";
		str += this->_registers.p.i ? "i" : "-";
		str += this->_registers.p.z ? "z" : "-";
		str += this->_registers.p.c ? "c" : "-";
		return str;
	}

	void CPUDebug::clearHistory()
	{
		this->_ui.logger->clear();
	}

	void CPUDebug::_updateDisassembly(uint24_t refreshSize)
	{
		auto first = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(), [this](DisassembledInstruction &i) {
			return i.address >= this->_registers.pac;
		});
		auto end = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(),[this, refreshSize](DisassembledInstruction &i) {
            return i.address >= this->_registers.pac + refreshSize;
        });
		this->disassembledInstructions.erase(first, end);

		auto next = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(), [this](DisassembledInstruction &i) {
			return i.address >= this->_registers.pac;
		});
		int row = next - this->disassembledInstructions.begin();
		DisassemblyContext ctx = this->_getDisassemblyContext();
		std::vector<DisassembledInstruction> nextInstructions = this->_disassemble(this->_registers.pac, refreshSize, ctx);
		this->disassembledInstructions.insert(next, nextInstructions.begin(), nextInstructions.end());
		if (this->_ui.disassembly->rowAt(0) > row || this->_ui.disassembly->rowAt(this->_ui.disassembly->height()) < row)
			this->_ui.disassembly->scrollTo(this->_model.index(row, 0), QAbstractItemView::PositionAtCenter);
		this->_ui.disassembly->viewport()->repaint();
	}

	DisassemblyContext CPUDebug::_getDisassemblyContext()
	{
		return {this->_registers.p.m, this->_registers.p.x_b, false};
	}

	std::vector<DisassembledInstruction> CPUDebug::_disassemble(uint24_t pc, uint24_t length, DisassemblyContext &ctx)
	{
		std::vector<DisassembledInstruction> map;
		uint24_t endAddr = pc + length;

		while (pc < endAddr) {
			DisassembledInstruction instruction = this->_parseInstruction(pc, ctx);
			instruction.level = ctx.level;
			map.push_back(instruction);
			pc += instruction.size;
			if (instruction.addressingMode == ImmediateForA && !ctx.mFlag)
				pc++;
			if (instruction.addressingMode == ImmediateForX && !ctx.xFlag)
				pc++;

			if (instruction.opcode == 0x40 && ctx.isEmulationMode) { // RTI
				ctx.mFlag = true;
				ctx.xFlag = true;
			}
			if (instruction.opcode == 0xC2) { // REP
				if (ctx.isEmulationMode) {
					ctx.mFlag = true;
					ctx.xFlag = true;
				} else {
					uint8_t m = this->_bus->read(pc - 1);
					ctx.mFlag &= ~m & 0b00100000u;
					ctx.xFlag &= ~m & 0b00010000u;
				}
			}
			if (instruction.opcode == 0xE2) { // SEP
				uint8_t m = this->_bus->read(pc - 1);
				ctx.mFlag |= m & 0b00100000u;
				ctx.xFlag |= m & 0b00010000u;
			}
			if (instruction.opcode == 0x28) { // PLP
				if (ctx.isEmulationMode) {
					ctx.mFlag = true;
					ctx.xFlag = true;
				} else
					ctx.level = Compromised;
			}
			if (instruction.opcode == 0xFB) {// XCE
				ctx.level = Unsafe;
				ctx.isEmulationMode = false; // The most common use of the XCE is to enable native mode at the start of the ROM so we guess that it has done that.
			}
		}
		return map;
	}

	DisassembledInstruction CPUDebug::_parseInstruction(uint24_t pc, DisassemblyContext &ctx)
	{
		uint24_t opcode = this->_bus->read(pc, true);
		Instruction instruction = this->_instructions[opcode];
		std::string argument = this->_getInstructionParameter(instruction, pc + 1, ctx);
		return DisassembledInstruction(instruction, pc, argument, opcode);
	}

	std::string CPUDebug::_getInstructionParameter(Instruction &instruction, uint24_t pc, DisassemblyContext &ctx)
	{
		switch (instruction.addressingMode) {
		case Implied:
			return "";
		case ImmediateForA:
			return this->_getImmediateValue(pc, !ctx.mFlag);
		case ImmediateForX:
			return this->_getImmediateValue(pc, !ctx.xFlag);
		case Immediate8bits:
			return this->_getImmediateValue(pc, false);
		case Absolute:
			return this->_getAbsoluteValue(pc);
		case AbsoluteLong:
			return this->_getAbsoluteLongValue(pc);
		case DirectPage:
			return this->_getDirectValue(pc);
		case DirectPageIndexedByX:
			return this->_getDirectIndexedByXValue(pc);

		default:
			return "???";
		}
	}

	std::string CPUDebug::_getImmediateValue(uint24_t pc, bool dual)
	{
		unsigned value = this->_bus->read(pc, true);

		if (dual)
			value += this->_bus->read(pc + 1, true) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getDirectValue(uint24_t pc)
	{
		std::stringstream ss;
		ss << "$" << std::hex << static_cast<int>(this->_bus->read(pc, true));
		return ss.str();
	}

	std::string CPUDebug::_getAbsoluteValue(uint24_t pc)
	{
		std::stringstream ss;
		ss << "$" << std::hex << (this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u));
		return ss.str();
	}

	std::string CPUDebug::_getAbsoluteLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc++, true);
		value += this->_bus->read(pc++, true) << 8u;
		value += this->_bus->read(pc, true) << 16u;

		std::stringstream ss;
		ss << "$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getDirectIndexedByXValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "$" << std::hex << value << ", x";
		return ss.str();
	}

	int CPUDebug::RESB(uint24_t)
	{
		CPU::RESB();
		this->_updateRegistersPanel();
		return (0);
	}

	void CPUDebug::focus()
	{
		this->_window->activateWindow();
	}

	uint24_t CPUDebug::getPC()
	{
		return this->_registers.pac;
	}

	DisassembledInstruction::DisassembledInstruction(const CPU::Instruction &instruction, uint24_t addr, std::string arg, uint8_t op)
		: CPU::Instruction(instruction), address(addr), argument(std::move(arg)), opcode(op) {}

	std::string DisassembledInstruction::toString()
	{
		return this->name + " " + this->argument;
	}
}

DisassemblyModel::DisassemblyModel(ComSquare::Debugger::CPUDebug &cpu) : QAbstractTableModel(), _cpu(cpu){ }

int DisassemblyModel::columnCount(const QModelIndex &) const
{
	return 4;
}

int DisassemblyModel::rowCount(const QModelIndex &) const
{
	return this->_cpu.disassembledInstructions.size();
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::DecorationRole)
		return QVariant();
	ComSquare::Debugger::DisassembledInstruction instruction = this->_cpu.disassembledInstructions[index.row()];
	if (role == Qt::DecorationRole) {
		if (index.column() == 3 && instruction.level == ComSquare::Debugger::TrustLevel::Unsafe)
			return QColor(Qt::yellow);
		if (index.column() == 3 && instruction.level == ComSquare::Debugger::TrustLevel::Compromised)
			return QColor(Qt::red);
		return QVariant();
	}
	switch (index.column()) {
	case 0:
		return QString(instruction.name.c_str());
	case 1:
		return QString(instruction.argument.c_str());
	default:
		return QVariant();
	}
}

QVariant DisassemblyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
		return QVariant();
	if (role != Qt::DisplayRole)
		return QVariant();
	ComSquare::Debugger::DisassembledInstruction instruction = this->_cpu.disassembledInstructions[section];
	return QString(ComSquare::Utility::to_hex(instruction.address, ComSquare::Utility::HexString::NoPrefix).c_str());
}

RowPainter::RowPainter(ComSquare::Debugger::CPUDebug &cpu, QObject *parent) : QStyledItemDelegate(parent), _cpu(cpu) { }

void RowPainter::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	ComSquare::Debugger::DisassembledInstruction instruction = this->_cpu.disassembledInstructions[index.row()];
	bool isBreakpoint = false;

	auto breakpoint = std::find_if(this->_cpu.breakpoints.begin(), this->_cpu.breakpoints.end(), [instruction](ComSquare::Debugger::Breakpoint brk) {
		return brk.address == instruction.address;
	});
	if (breakpoint != this->_cpu.breakpoints.end())
		isBreakpoint = true;

	QStyleOptionViewItem style = option;
	if (instruction.address == this->_cpu.getPC()) {
		painter->fillRect(option.rect, QColor(Qt::darkGreen));
		style.state &= ~QStyle::State_Selected;
	} else if (isBreakpoint && !breakpoint->oneTime) {
		painter->fillRect(option.rect,QColor(Qt::darkRed));
		style.state &= ~QStyle::State_Selected;
	}
	QStyledItemDelegate::paint(painter, style, index);
}

QSize RowPainter::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
	return QSize();
}
