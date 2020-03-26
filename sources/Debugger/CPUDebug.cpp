//
// Created by anonymus-raccoon on 2/14/20.
//

#include "CPUDebug.hpp"
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidOpcode.hpp"
#include <QtEvents>
#include <iostream>

using namespace ComSquare::CPU;

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(CPU &basicCPU, SNES &snes)
		: CPU(basicCPU),
		_window(new ClosableWindow<CPUDebug>(*this, &CPUDebug::disableDebugger)),
		_ui(),
		_model(*this),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);

		this->_ui.disasembly->setModel(&this->_model);
		this->_ui.disasembly->setShowGrid(false);
		this->_ui.disasembly->verticalHeader()->hide();
		this->_ui.disasembly->horizontalHeader()->hide();
		this->_ui.disasembly->horizontalHeader()->setStretchLastSection(true);

		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		QMainWindow::connect(this->_ui.actionStep, &QAction::triggered, this, &CPUDebug::step);
		QMainWindow::connect(this->_ui.clear, &QPushButton::released, this, &CPUDebug::clearHistory);
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
			if (this->_isPaused)
				return 0xFF;
			if (this->_isStepping)
				return this->_executeInstruction(this->_bus->read(this->_registers.pac++));
			return CPU::update();
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
		this->_ui.logger->append((CPUDebug::_getInstructionString(this->_registers.pac - 1) + " -  " + Utility::to_hex(opcode)).c_str());
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
	}

	void CPUDebug::step()
	{
		this->_isStepping = true;
		this->_isPaused = false;
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

	std::string CPUDebug::_getImmediateValueForA(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		if (!this->_registers.p.m)
			value += this->_bus->read(pc + 1, true) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getImmediateValueForX(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		if (!this->_registers.p.x_b)
			value += this->_bus->read(pc + 1, true) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getImmediateValue8Bits(uint24_t pc)
	{
		std::stringstream ss;
		ss << "#$" << std::hex << static_cast<int>(this->_bus->read(pc), true);
		return ss.str();
	}

	std::string CPUDebug::_getImmediateValue16Bits(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);
		value += this->_bus->read(pc + 1, true) << 8u;

		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getDirectValue(uint24_t pc)
	{
		std::stringstream ss;
		ss << "$" << std::hex << static_cast<int>(this->_bus->read(pc), true);
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

	std::string CPUDebug::_getInstructionString(uint24_t pc)
	{
		uint8_t opcode = this->_bus->read(pc++, true);
		return this->_instructions[opcode].name;
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
}

DisassemblyModel::DisassemblyModel(ComSquare::Debugger::CPUDebug &cpu) : QAbstractTableModel(), _cpu(cpu){ }

int DisassemblyModel::columnCount(const QModelIndex &) const
{
	return 4;
}

int DisassemblyModel::rowCount(const QModelIndex &) const
{
	return 0xFFFFFF;
}

QVariant DisassemblyModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
	return QString();
}