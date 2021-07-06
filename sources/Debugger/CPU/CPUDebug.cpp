//
// Created by anonymus-raccoon on 2/14/20.
//

#include "CPUDebug.hpp"
#include "SNES.hpp"
#include "Exceptions/InvalidOpcode.hpp"
#include "Utility/Utility.hpp"
#include <QMessageBox>
#include <QPainter>
#include <QtEvents>
#include <fstream>
#include <iostream>

using namespace ComSquare::CPU;

namespace ComSquare::Debugger::CPU
{
	CPUDebug::CPUDebug(ComSquare::CPU::CPU &basicCPU, SNES &snes)
	    : _cpu(basicCPU),
	      _window(new ClosableWindow([&snes] { snes.disableCPUDebugging(); })),
	      _ui(),
	      _model(*this),
	      _painter(*this),
	      _stackModel(snes.bus, *this),
	      _snes(snes),
	      _labels(),
	      initialStackPointer(this->_cpu._registers.s)
	{
		this->_loadLabels(snes.cartridge.getRomPath());
		this->_ui.setupUi(this->_window);


		//Parse the first page of the ROM (the code can't reach the second page without a jump).
		uint16_t resetInter = snes.cartridge.header.emulationInterrupts.reset;
		this->_updateDisassembly(resetInter, 0xFFFF - resetInter);

		this->_ui.disassembly->setModel(&this->_model);
		this->_ui.disassembly->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.disassembly->horizontalHeader()->setStretchLastSection(true);
		this->_ui.disassembly->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		this->_ui.disassembly->verticalHeader()->setHighlightSections(false);
		this->_ui.disassembly->setItemDelegate(&this->_painter);

		this->_ui.stackView->setModel(&this->_stackModel);
		this->_ui.stackView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		this->_ui.stackView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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
		this->_updateDisassembly(this->_cpu._registers.pac, 0);

		this->_cpu.isDisabled = true;
		this->_callback = this->_cpu.onReset.addCallback([this] {
			this->disassembled.clear();
			//Parse the first page of the ROM (the code can't reach the second page without a jump).
			uint16_t reset = this->_snes.cartridge.header.emulationInterrupts.reset;
			this->_updateDisassembly(reset, 0xFFFF - reset);
			this->_updateRegistersPanel();
		});

		this->_timer.setInterval(1000 / 60);
		this->_timer.setSingleShot(false);
		connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
		this->_timer.start();
	}

	CPUDebug::~CPUDebug()
	{
		this->_cpu.onReset.removeCallback(this->_callback);
		this->_cpu.isDisabled = false;
	}

	unsigned CPUDebug::update()
	{
		try {
			unsigned cycles = this->_cpu.runDMA(INT_MAX);

			if (this->_isPaused)
				return 0xFF;

			for (int i = 0; i < 0xFF; i++) {
				auto breakpoint = std::find_if(this->breakpoints.begin(), this->breakpoints.end(), [this](auto &brk) {
					return brk.address == this->_cpu._registers.pac;
				});
				if (i != 0 && breakpoint != this->breakpoints.end()) {
					if (breakpoint->oneTime)
						this->breakpoints.erase(breakpoint);
					this->pause(true);
					return cycles;
				}
				this->_logInstruction();
				cycles += this->_cpu.executeInstruction();
				this->_updateRegistersPanel();
				if (this->_isStepping) {
					this->_isStepping = false;
					this->pause(true);
					return cycles;
				}
			}
			return cycles;
		} catch (const DebuggableError &e) {
			this->pause(true);
			CPUDebug::showError(e);
			return 0xFF;
		} catch (const std::exception &e) {
			std::cerr << "An error occurred: " << e.what() << std::endl;
			QApplication::quit();
		}
	}

	void CPUDebug::_logInstruction()
	{
		DisassemblyContext ctx = this->_getDisassemblyContext();
		DisassembledInstruction instruction = this->_parseInstruction(this->_cpu._registers.pac, ctx);
		this->_historyModel.log({instruction.opcode, instruction.name, instruction.argument, this->getProceededParameters()});
		this->_ui.history->scrollToBottom();
	}

	void CPUDebug::showError(const DebuggableError &error)
	{
		QMessageBox msg;
		msg.setIcon(QMessageBox::Critical);
		msg.setText("Invalid rom action");
		msg.setInformativeText(error.what());
		msg.exec();
	}

	void CPUDebug::pause(bool forcePause)
	{
		if (forcePause && this->_isPaused)
			return;
		this->_isPaused = !this->_isPaused;
		if (this->_isPaused)
			this->_ui.actionPause->setText("Continue");
		else
			this->_ui.actionPause->setText("Pause");
		this->_updateDisassembly(this->_cpu._registers.pac);
	}

	void CPUDebug::step()
	{
		this->_isStepping = true;
		this->_isPaused = false;
	}

	void CPUDebug::next()
	{
		auto next = std::find_if(this->disassembled.begin(), this->disassembled.end(), [this](auto &i) {
			return i.address > this->_cpu._registers.pac;
		});
		this->breakpoints.push_back({next->address, true});
		this->_isPaused = false;
	}

	void CPUDebug::toggleBreakpoint(int logicalIndex)
	{
		DisassembledInstruction instruction = this->disassembled[logicalIndex];
		auto existing = std::find_if(this->breakpoints.begin(), this->breakpoints.end(), [instruction](auto &i) {
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
		if (!this->_cpu._registers.p.m)
			this->_ui.accumulatorLineEdit->setText(Utility::to_hex(this->_cpu._registers.a).c_str());
		else
			this->_ui.accumulatorLineEdit->setText(Utility::to_hex(this->_cpu._registers.al).c_str());
		this->_ui.programBankRegisterLineEdit->setText(Utility::to_hex(this->_cpu._registers.pbr).c_str());
		this->_ui.programCounterLineEdit->setText(Utility::to_hex(this->_cpu._registers.pc).c_str());
		this->_ui.directBankLineEdit->setText(Utility::to_hex(this->_cpu._registers.dbr).c_str());
		this->_ui.directPageLineEdit->setText(Utility::to_hex(this->_cpu._registers.d).c_str());
		this->_ui.stackPointerLineEdit->setText(Utility::to_hex(this->_cpu._registers.s).c_str());
		if (!this->_cpu._registers.p.x_b) {
			this->_ui.xIndexLineEdit->setText(Utility::to_hex(this->_cpu._registers.x).c_str());
			this->_ui.yIndexLineEdit->setText(Utility::to_hex(this->_cpu._registers.y).c_str());
		} else {
			this->_ui.xIndexLineEdit->setText(Utility::to_hex(this->_cpu._registers.xl).c_str());
			this->_ui.yIndexLineEdit->setText(Utility::to_hex(this->_cpu._registers.yl).c_str());
		}
		this->_ui.emulationModeCheckBox->setCheckState(this->_cpu._isEmulationMode ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

		this->_ui.mCheckbox->setCheckState(this->_cpu._registers.p.m ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.xCheckbox->setCheckState(this->_cpu._registers.p.x_b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.bCheckbox->setCheckState(this->_cpu._registers.p.x_b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.iCheckbox->setCheckState(this->_cpu._registers.p.i ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.vCheckbox->setCheckState(this->_cpu._registers.p.v ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.dCheckbox->setCheckState(this->_cpu._registers.p.d ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.cCheckbox->setCheckState(this->_cpu._registers.p.c ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.zCheckbox->setCheckState(this->_cpu._registers.p.z ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
		this->_ui.nCheckbox->setCheckState(this->_cpu._registers.p.n ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

		auto index = this->_stackModel.index(this->_cpu._registers.s / 2, 0);
		this->_ui.stackView->scrollTo(index, QAbstractItemView::PositionAtCenter);
	}

	void CPUDebug::clearHistory()
	{
		this->_historyModel.clear();
	}

	void CPUDebug::_updateDisassembly(uint24_t start, uint24_t refreshSize)
	{
		auto first = std::find_if(this->disassembled.begin(), this->disassembled.end(), [start](DisassembledInstruction &i) {
			return i.address >= start;
		});
		auto end = std::find_if(this->disassembled.begin(), this->disassembled.end(), [start, refreshSize](DisassembledInstruction &i) {
			return i.address >= start + refreshSize;
		});
		auto next = this->disassembled.erase(first, end);

		DisassemblyContext ctx = this->_getDisassemblyContext();
		std::vector<DisassembledInstruction> nextInstructions = this->_disassemble(start, refreshSize, ctx);
		auto inserted = this->disassembled.insert(next, nextInstructions.begin(), nextInstructions.end());

		if (this->disassembled.empty())
			return;

		int row = static_cast<int>(inserted - this->disassembled.begin());
		if (this->_ui.disassembly->rowAt(0) > row || this->_ui.disassembly->rowAt(this->_ui.disassembly->height()) < row) {
			auto index = this->_model.index(row, 0);
			this->_ui.disassembly->scrollTo(index, QAbstractItemView::PositionAtCenter);
		}
		this->_ui.disassembly->viewport()->repaint();
	}

	DisassemblyContext CPUDebug::_getDisassemblyContext()
	{
		return {this->_cpu._registers.p.m, this->_cpu._registers.p.x_b, this->_cpu._isEmulationMode};
	}

	void CPUDebug::focus()
	{
		this->_window->activateWindow();
	}

	uint24_t CPUDebug::getPC() const
	{
		return this->_cpu._registers.pac;
	}

	uint16_t CPUDebug::getStackPointer() const
	{
		return this->_cpu._registers.s;
	}

	std::string CPUDebug::getProceededParameters() const
	{
		uint24_t pac = this->_cpu._registers.pac;
		auto &bus = this->_cpu.getBus();
		this->_cpu.setBus(this->_snes.bus);
		const Instruction &instruction = this->_cpu.instructions[this->_cpu._readPC()];
		uint24_t valueAddr = this->_cpu._getValueAddr(instruction);
		this->_cpu._registers.pac = pac;
		this->_cpu.setBus(bus);
		if (instruction.size == 1)
			return "";
		return "[" + Utility::to_hex(valueAddr, Utility::AsmPrefix) + "]";
	}

	void CPUDebug::_loadLabels(std::filesystem::path romPath)
	{
		std::string symbolPath = romPath.replace_extension(".sym");
		std::ifstream sym(symbolPath);

		this->_labels.clear();
		//if (sym) {
		//	std::vector<Label> symLabels = WlaDx::parse(sym);
		//	this->_labels.insert(labels.end(),
		//	              std::make_move_iterator(symLabels.begin()),
		//	              std::make_move_iterator(symLabels.end()));
		//}
	}

	DisassemblyModel::DisassemblyModel(CPUDebug &cpu)
	    : QAbstractTableModel(), _cpu(cpu)
	{}

	int DisassemblyModel::columnCount(const QModelIndex &) const
	{
		return 4;
	}

	int DisassemblyModel::rowCount(const QModelIndex &) const
	{
		return static_cast<int>(this->_cpu.disassembled.size());
	}

	QVariant DisassemblyModel::data(const QModelIndex &index, int role) const
	{
		DisassembledInstruction instruction = this->_cpu.disassembled[index.row()];

		switch (role) {
		case Qt::DecorationRole:
			if (index.column() == 2 && instruction.level == TrustLevel::Unsafe)
				return QColor(Qt::yellow);
			if (index.column() == 2 && instruction.level == TrustLevel::Compromised)
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
		DisassembledInstruction instruction = this->_cpu.disassembled[section];
		return QString(Utility::to_hex(instruction.address, Utility::HexString::NoPrefix).c_str());
	}

	RowPainter::RowPainter(CPUDebug &cpu, QObject *parent)
	    : QStyledItemDelegate(parent), _cpu(cpu)
	{}

	void RowPainter::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		DisassembledInstruction instruction = this->_cpu.disassembled[index.row()];
		bool isBreakpoint = false;

		auto breakpoint = std::find_if(this->_cpu.breakpoints.begin(), this->_cpu.breakpoints.end(), [instruction](auto &brk) {
			return brk.address == instruction.address;
		});
		if (breakpoint != this->_cpu.breakpoints.end())
			isBreakpoint = true;

		QStyleOptionViewItem style = option;
		if (instruction.address == this->_cpu.getPC()) {
			painter->fillRect(option.rect, QColor(Qt::darkGreen));
			style.state &= ~QStyle::State_Selected;
		} else if (isBreakpoint && !breakpoint->oneTime) {
			painter->fillRect(option.rect, QColor(Qt::darkRed));
			style.state &= ~QStyle::State_Selected;
		}
		QStyledItemDelegate::paint(painter, style, index);
	}

	QSize RowPainter::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
	{
		return QSize();
	}

	StackModel::StackModel(Memory::IMemoryBus &bus, CPUDebug &cpu)
	    : _bus(bus),
	      _cpu(cpu)
	{}

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
			uint8_t value = this->_bus.peek_v(addr);
			return (Utility::to_hex(value, Utility::NoPrefix).c_str());
		} catch (const std::exception &) {
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
		return QString(Utility::to_hex(addr, Utility::HexString::NoPrefix).c_str());
	}

	HistoryModel::HistoryModel() = default;

	int HistoryModel::rowCount(const QModelIndex &) const
	{
		return static_cast<int>(this->_instructions.size());
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

		ExecutedInstruction instruction = this->_instructions[index.row()];
		switch (index.column()) {
		case 0:
			return QString(Utility::to_hex(instruction.opcode, Utility::NoPrefix).c_str());
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

	void HistoryModel::log(const ExecutedInstruction &instruction)
	{
		int row = static_cast<int>(this->_instructions.size());
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
}// namespace ComSquare::Debugger