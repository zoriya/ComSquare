//
// Created by anonymus-raccoon on 2/14/20.
//

#include "CPUDebug.hpp"
#include "../../Utility/Utility.hpp"
#include "../../Exceptions/InvalidOpcode.hpp"
#include "../../CPU/CPU.hpp"
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
		_stackModel(*this->_bus, *this),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);

		this->_updateDisassembly(this->_cartridgeHeader.emulationInterrupts.reset, 0xFFFF - this->_cartridgeHeader.emulationInterrupts.reset); //Parse the first page of the ROM (the code can't reach the second page without a jump).
		this->_ui.disassembly->setModel(&this->_model);
		this->_ui.disassembly->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.disassembly->horizontalHeader()->setStretchLastSection(true);
		this->_ui.disassembly->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		this->_ui.disassembly->verticalHeader()->setHighlightSections(false);
		this->_ui.disassembly->setItemDelegate(&this->_painter);

		this->_ui.stackView->setModel(&this->_stackModel);
		this->_ui.stackView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.stackView->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
		this->_ui.stackView->verticalHeader()->setHighlightSections(false);


		this->_ui.history->setModel(&this->_historyModel);
		this->_ui.history->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		this->_ui.history->horizontalHeader()->setStretchLastSection(true);
		this->_ui.history->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		this->_ui.history->verticalHeader()->hide();

		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		QMainWindow::connect(this->_ui.actionStep, &QAction::triggered, this, &CPUDebug::step);
		QMainWindow::connect(this->_ui.actionNext, &QAction::triggered, this, &CPUDebug::next);
		QMainWindow::connect(this->_ui.clear, &QPushButton::released, this, &CPUDebug::clearHistory);
		QMainWindow::connect(this->_ui.disassembly->verticalHeader(), &QHeaderView::sectionClicked, this, &CPUDebug::toggleBreakpoint);
		this->_window->show();
		this->_updateRegistersPanel();
		this->_updateDisassembly(this->_registers.pac, 0);
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
				this->_updateDisassembly(this->_registers.pac);
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
		this->_registers.pc--;
		this->_historyModel.log({opcode, instruction.name, instruction.argument, this->getProceededParameters()});
		this->_ui.history->scrollToBottom();
		this->_registers.pc++;
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
		this->_updateDisassembly(this->_registers.pac);
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
		this->_ui.emulationModeCheckBox->setCheckState(this->_isEmulationMode ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

		this->_ui.mCheckbox->setCheckState(this->_registers.p.m ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.xCheckbox->setCheckState(this->_registers.p.x_b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.bCheckbox->setCheckState(this->_registers.p.x_b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.iCheckbox->setCheckState(this->_registers.p.i ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.vCheckbox->setCheckState(this->_registers.p.v ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.dCheckbox->setCheckState(this->_registers.p.d ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.cCheckbox->setCheckState(this->_registers.p.c ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.zCheckbox->setCheckState(this->_registers.p.z ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.nCheckbox->setCheckState(this->_registers.p.n ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

		auto index = this->_stackModel.index(this->_registers.s / 2, 0);
		this->_ui.stackView->scrollTo(index, QAbstractItemView::PositionAtCenter);
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
		this->_historyModel.clear();
	}

	void CPUDebug::_updateDisassembly(uint24_t start, uint24_t refreshSize)
	{
		auto first = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(), [start](DisassembledInstruction &i) {
			return i.address >= start;
		});
		auto end = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(),[start, refreshSize](DisassembledInstruction &i) {
            return i.address >= start + refreshSize;
        });
		this->disassembledInstructions.erase(first, end);

		auto next = std::find_if(this->disassembledInstructions.begin(), this->disassembledInstructions.end(), [start](DisassembledInstruction &i) {
			return i.address >= start;
		});
		DisassemblyContext ctx = this->_getDisassemblyContext();
		std::vector<DisassembledInstruction> nextInstructions = this->_disassemble(start, refreshSize, ctx);
		this->disassembledInstructions.insert(next, nextInstructions.begin(), nextInstructions.end());

		int row = next - this->disassembledInstructions.begin();
		if (this->_ui.disassembly->rowAt(0) > row || this->_ui.disassembly->rowAt(this->_ui.disassembly->height()) < row) {
			auto index = this->_model.index(row, 0);
			this->_ui.disassembly->scrollTo(index, QAbstractItemView::PositionAtCenter);
		}
		this->_ui.disassembly->viewport()->repaint();
	}

	DisassemblyContext CPUDebug::_getDisassemblyContext()
	{
		return {this->_registers.p.m, this->_registers.p.x_b, this->_isEmulationMode};
	}

	int CPUDebug::RESB()
	{
		CPU::RESB();
		this->disassembledInstructions.clear();
		this->_updateDisassembly(0xFFFF - this->_cartridgeHeader.emulationInterrupts.reset);
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

	uint16_t CPUDebug::getStackPointer()
	{
		return this->_registers.s;
	}

	std::string CPUDebug::getProceededParameters()
	{
		uint24_t pac = this->_registers.pac;
		this->_bus->forceSilence = true;
		Instruction instruction = this->_instructions[this->readPC()];
		uint24_t valueAddr = this->_getValueAddr(instruction);
		this->_registers.pac = pac;
		this->_bus->forceSilence = false;
		if (instruction.size == 1)
			return "";
		return "[" + Utility::to_hex(valueAddr, Utility::AsmPrefix) + "]";
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
	ComSquare::Debugger::DisassembledInstruction instruction = this->_cpu.disassembledInstructions[index.row()];

	switch (role) {
	case Qt::DecorationRole:
		if (index.column() == 2 && instruction.level == ComSquare::Debugger::TrustLevel::Unsafe)
			return QColor(Qt::yellow);
		if (index.column() == 2 && instruction.level == ComSquare::Debugger::TrustLevel::Compromised)
			return QColor(Qt::red);
		return QVariant();
	case Qt::DisplayRole:
		switch (index.column()) {
		case 0:
			return QString(instruction.name.c_str());
		case 1:
			return QString(instruction.argument.c_str());
		case 3:
			if (instruction.address != this->_cpu.getPC())
				return QVariant();
			return QString(this->_cpu.getProceededParameters().c_str());
		default:
			return QVariant();
		}
	default:
		return QVariant();
	}
}

QVariant DisassemblyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString("INST");
		case 1:
			return QString("Parameter");
		case 2:
			return QString("Thrust");
		case 3:
			return QString("Data Address");
		default:
			return QVariant();
		}
	}
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

StackModel::StackModel(ComSquare::Memory::MemoryBus &bus, ComSquare::Debugger::CPUDebug &cpu) : _bus(bus), _cpu(cpu) { }

int StackModel::rowCount(const QModelIndex &) const
{
	return 0x10000 / 2;
}

int StackModel::columnCount(const QModelIndex &) const
{
	return 2;
}

QVariant StackModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::BackgroundRole) {
		if (index.row() * 2 + index.column() == this->_cpu.getStackPointer())
			return QColor(Qt::darkBlue);
		if (index.row() * 2 + index.column() == this->_cpu.initialStackPointer)
			return QColor(Qt::darkCyan);
	}
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::DisplayRole)
		return QVariant();
	uint16_t addr = index.row() * 2 + index.column();
	try {
		uint8_t value = this->_bus.read(addr);
		return (ComSquare::Utility::to_hex(value, ComSquare::Utility::NoPrefix).c_str());
	} catch (std::exception &) {
		return "??";
	}
}

QVariant StackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
		return QVariant();
	if (role != Qt::DisplayRole)
		return QVariant();
	uint16_t addr = section * 2;
	return QString(ComSquare::Utility::to_hex(addr, ComSquare::Utility::HexString::NoPrefix).c_str());
}

HistoryModel::HistoryModel() = default;

int HistoryModel::rowCount(const QModelIndex &) const
{
	return this->_instructions.size();
}

int HistoryModel::columnCount(const QModelIndex &) const
{
	return 4;
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::DisplayRole)
		return QVariant();

	ComSquare::Debugger::ExecutedInstruction instruction = this->_instructions[index.row()];
	switch (index.column()) {
	case 0:
		return QString(ComSquare::Utility::to_hex(instruction.opcode, ComSquare::Utility::NoPrefix).c_str());
	case 1:
		return QString(instruction.name.c_str());
	case 2:
		return QString(instruction.params.c_str());
	case 3:
		return QString(instruction.proceededParams.c_str());
	default:
		return QVariant();
	}
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();
	switch (section) {
	case 0:
		return QString("op");
	case 1:
		return QString("ins");
	case 2:
		return QString("Parameter");
	case 3:
		return QString("Pointer");
	default:
		return QVariant();
	}
}

void HistoryModel::log(const ComSquare::Debugger::ExecutedInstruction& instruction)
{
	int row = this->_instructions.size();
	this->beginInsertRows(QModelIndex(), row, row);
	this->_instructions.push_back(instruction);
	this->insertRow(row);
	this->endInsertRows();
}

void HistoryModel::clear()
{
	this->beginResetModel();
	this->_instructions.clear();
	this->endResetModel();
}