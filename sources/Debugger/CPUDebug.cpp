//
// Created by anonymus-raccoon on 2/14/20.
//

#include "CPUDebug.hpp"
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

using namespace ComSquare::CPU;

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(CPU &basicCPU, SNES &snes)
		: CPU(basicCPU), QMainWindow(), _ui(), _snes(snes)
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this);
		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		QMainWindow::connect(this->_ui.actionStep, &QAction::triggered, this, &CPUDebug::step);
		QMainWindow::connect(this->_ui.clear, &QPushButton::released, this, &CPUDebug::clearHistory);
		this->show();
		this->_updateRegistersPanel();
	}

	unsigned CPUDebug::update()
	{
		try {
			if (!this->isVisible()) {
				this->_snes.disableCPUDebugging();
				return 0;
			}

			if (this->_isPaused)
				return 0xFF;
			return CPU::update();
		} catch (InvalidOpcode &e) {
			if (!this->_isPaused)
				this->pause();
			return 0xFF;
		}
	}

	unsigned CPUDebug::_executeInstruction(uint8_t opcode)
	{
		if (this->_isPaused) {
			this->_registers.pac--;
			return 0;
		}
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
		unsigned value = this->_bus->read(pc);

		if (!this->_registers.p.m)
			value += this->_bus->read(pc + 1) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getImmediateValueForX(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc);

		if (!this->_registers.p.x_b)
			value += this->_bus->read(pc + 1) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getImmediateValue8Bits(uint24_t pc)
	{
		std::stringstream ss;
		ss << "#$" << std::hex << static_cast<int>(this->_bus->read(pc));
		return ss.str();
	}

	std::string CPUDebug::_getDirectValue(uint24_t pc)
	{
		std::stringstream ss;
		ss << "$" << std::hex << static_cast<int>(this->_bus->read(pc));
		return ss.str();
	}

	std::string CPUDebug::_getAbsoluteValue(uint24_t pc)
	{
		std::stringstream ss;
		ss << "$" << std::hex << (this->_bus->read(pc) + (this->_bus->read(pc + 1) << 8u));
		return ss.str();
	}

	std::string CPUDebug::_getAbsoluteLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc++);
		value += this->_bus->read(pc++) << 8u;
		value += this->_bus->read(pc) << 16u;

		std::stringstream ss;
		ss << "$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getInstructionString(uint24_t pc)
	{
		uint8_t opcode = this->_bus->read(pc++);

		switch (opcode) {
		case Instructions::BRK:      return "BRK";

		case Instructions::COP:      return "COP";

		case Instructions::RTI:	     return "RTI";

		case Instructions::ADC_IM:   return "ADC " + this->_getImmediateValueForA(pc);
		case Instructions::ADC_ABS:  return "ADC " + this->_getAbsoluteValue(pc);
		case Instructions::ADC_ABSl: return "ADC " + this->_getAbsoluteLongValue(pc);
		case Instructions::ADC_DP:   return "ADC " + this->_getDirectValue(pc);
		case Instructions::ADC_DPi:  return "ADC";
		case Instructions::ADC_DPil: return "ADC";
		case Instructions::ADC_ABSX: return "ADC";
		case Instructions::ADC_ABSXl:return "ADC";
		case Instructions::ADC_ABSY: return "ADC";
		case Instructions::ADC_DPX:  return "ADC";
		case Instructions::ADC_DPXi: return "ADC";
		case Instructions::ADC_DPYi: return "ADC";
		case Instructions::ADC_DPYil:return "ADC";
		case Instructions::ADC_SR:   return "ADC";
		case Instructions::ADC_SRYi: return "ADC";

		case Instructions::STA_ABS:  return "STA " + this->_getAbsoluteValue(pc);
		case Instructions::STA_ABSl: return "STA " + this->_getAbsoluteLongValue(pc);
		case Instructions::STA_DP:   return "STA " + this->_getDirectValue(pc);
		case Instructions::STA_DPi:  return "STA";
		case Instructions::STA_DPil: return "STA";
		case Instructions::STA_ABSX: return "STA";
		case Instructions::STA_ABSXl:return "STA";
		case Instructions::STA_ABSY: return "STA";
		case Instructions::STA_DPX:  return "STA";
		case Instructions::STA_DPXi: return "STA";
		case Instructions::STA_DPYi: return "STA";
		case Instructions::STA_DPYil:return "STA";
		case Instructions::STA_SR:   return "STA";
		case Instructions::STA_SRYi: return "STA";

		case Instructions::STX_ABS:  return "STX " + this->_getAbsoluteValue(pc);
		case Instructions::STX_DP:   return "STX " + this->_getDirectValue(pc);
		case Instructions::STX_DPY:  return "STX";

		case Instructions::STY_ABS:  return "STY " + this->_getAbsoluteValue(pc);
		case Instructions::STY_DP:   return "STY " + this->_getDirectValue(pc);
		case Instructions::STY_DPX:  return "STY";

		case Instructions::STZ_ABS:  return "STZ " + this->_getAbsoluteValue(pc);
		case Instructions::STZ_DP:   return "STZ " + this->_getDirectValue(pc);
		case Instructions::STZ_ABSX: return "STZ";
		case Instructions::STZ_DPX:  return "STZ";

		case Instructions::LDA_IM:   return "LDA " + this->_getImmediateValueForA(pc);
		case Instructions::LDA_ABS:  return "LDA " + this->_getAbsoluteValue(pc);
		case Instructions::LDA_ABSl: return "LDA " + this->_getAbsoluteLongValue(pc);
		case Instructions::LDA_DP:   return "LDA " + this->_getDirectValue(pc);
		case Instructions::LDA_DPi:  return "LDA";
		case Instructions::LDA_DPil: return "LDA";
		case Instructions::LDA_ABSX: return "LDA";
		case Instructions::LDA_ABSXl:return "LDA";
		case Instructions::LDA_ABSY: return "LDA";
		case Instructions::LDA_DPX:  return "LDA";
		case Instructions::LDA_DPXi: return "LDA";
		case Instructions::LDA_DPYi: return "LDA";
		case Instructions::LDA_DPYil:return "LDA";
		case Instructions::LDA_SR:   return "LDA";
		case Instructions::LDA_SRYi: return "LDA";

		case Instructions::LDX_IM:   return "LDX " + this->_getImmediateValueForX(pc);
		case Instructions::LDX_ABS:  return "LDX " + this->_getAbsoluteValue(pc);
		case Instructions::LDX_DP:   return "LDX " + this->_getDirectValue(pc);
		case Instructions::LDX_ABSY: return "LDX";
		case Instructions::LDX_DPY:  return "LDX";

		case Instructions::LDY_IM:   return "LDY " + this->_getImmediateValueForX(pc);
		case Instructions::LDY_ABS:  return "LDY " + this->_getAbsoluteValue(pc);
		case Instructions::LDY_DP:   return "LDY " + this->_getDirectValue(pc);
		case Instructions::LDY_ABSY: return "LDY";
		case Instructions::LDY_DPY:  return "LDY";

		case Instructions::SEP: return "SEP " + this->_getImmediateValue8Bits(pc);

		case Instructions::REP: return "REP " + this->_getImmediateValue8Bits(pc);

		case Instructions::PHA: return "PHA";
		case Instructions::PHB: return "PHB";
		case Instructions::PHD: return "PHD";
		case Instructions::PHK: return "PHK";
		case Instructions::PHP: return "PHP";
		case Instructions::PHX: return "PHX";
		case Instructions::PHY: return "PHY";

		case Instructions::PLA: return "PLA";
		case Instructions::PLB: return "PLB";
		case Instructions::PLD: return "PLD";
		case Instructions::PLP: return "PLP";
		case Instructions::PLX: return "PLX";
		case Instructions::PLY: return "PLY";

		case Instructions::JSR_ABS:   return "JSR " + this->_getAbsoluteValue(pc);
		case Instructions::JSR_ABSXi: return "JSR";

		case Instructions::JSL: return "JSL " + this->_getAbsoluteLongValue(pc);

		case Instructions::CLC: return "CLC";
		case Instructions::CLI: return "CLI";
		case Instructions::CLD: return "CLD";
		case Instructions::CLV: return "CLV";

		case Instructions::SEC: return "SEC";
		case Instructions::SED: return "SED";
		case Instructions::SEI: return "SEI";

		case Instructions::AND_IM:   return "AND " + this->_getImmediateValueForA(pc);
		case Instructions::AND_ABS:  return "AND " + this->_getAbsoluteValue(pc);
		case Instructions::AND_ABSl: return "AND " + this->_getAbsoluteLongValue(pc);
		case Instructions::AND_DP:   return "AND " + this->_getDirectValue(pc);
		case Instructions::AND_DPi:  return "AND";
		case Instructions::AND_DPil: return "AND";
		case Instructions::AND_ABSX: return "AND";
		case Instructions::AND_ABSXl:return "AND";
		case Instructions::AND_ABSY: return "AND";
		case Instructions::AND_DPX:  return "AND";
		case Instructions::AND_DPXi: return "AND";
		case Instructions::AND_DPYi: return "AND";
		case Instructions::AND_DPYil:return "AND";
		case Instructions::AND_SR:   return "AND";
		case Instructions::AND_SRYi: return "AND";

		case Instructions::XCE: return "XCE";

		case Instructions::SBC_IM:   return "SBC " + this->_getImmediateValueForA(pc);
		case Instructions::SBC_ABS:  return "SBC " + this->_getAbsoluteValue(pc);
		case Instructions::SBC_ABSl: return "SBC " + this->_getAbsoluteLongValue(pc);
		case Instructions::SBC_DP:   return "SBC " + this->_getDirectValue(pc);
		case Instructions::SBC_DPi:  return "SBC";
		case Instructions::SBC_DPil: return "SBC";
		case Instructions::SBC_ABSX: return "SBC";
		case Instructions::SBC_ABSXl:return "SBC";
		case Instructions::SBC_ABSY: return "SBC";
		case Instructions::SBC_DPX:  return "SBC";
		case Instructions::SBC_DPXi: return "SBC";
		case Instructions::SBC_DPYi: return "SBC";
		case Instructions::SBC_DPYil:return "SBC";
		case Instructions::SBC_SR:   return "SBC";
		case Instructions::SBC_SRYi: return "SBC";

		case Instructions::TAX: 	 return "TAX";
		case Instructions::TAY: 	 return "TAY";
		case Instructions::TXS: 	 return "TXS";

		default: return "Unknown";
		}
	}

	void CPUDebug::RESB()
	{
		CPU::RESB();
		this->_updateRegistersPanel();
	}
}