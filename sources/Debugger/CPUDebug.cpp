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
			this->pause();
			this->_ui.logger->append(e.what());
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
		this->_ui.logger->append(CPUDebug::_getInstructionString(opcode).c_str());
		this->_updateRegistersPanel();
		return CPU::_executeInstruction(opcode);
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
		if (this->_registers.p.x_b) {
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

	std::string CPUDebug::_getInstructionString(uint8_t opcode)
	{
		switch (opcode) {
		case Instructions::BRK:      return "BRK";

		case Instructions::COP:      return "COP";

		case Instructions::RTI:	     return "RTI";

		case Instructions::ADC_IM:   return "ADC";
		case Instructions::ADC_ABS:  return "ADC";
		case Instructions::ADC_ABSl: return "ADC";
		case Instructions::ADC_DP:   return "ADC";
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

		case Instructions::STA_ABS:  return "STA";
		case Instructions::STA_ABSl: return "STA";
		case Instructions::STA_DP:   return "STA";
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

		case Instructions::STX_ABS:  return "STX";
		case Instructions::STX_DP:   return "STX";
		case Instructions::STX_DPY:  return "STX";

		case Instructions::STY_ABS:  return "STX";
		case Instructions::STY_DP:   return "STX";
		case Instructions::STY_DPX:  return "STX";

		case Instructions::STZ_ABS:  return "STX";
		case Instructions::STZ_DP:   return "STX";
		case Instructions::STZ_ABSX: return "STX";
		case Instructions::STZ_DPX:  return "STX";

		case Instructions::LDA_IM:   return "LDA";
		case Instructions::LDA_ABS:  return "LDA";
		case Instructions::LDA_ABSl: return "LDA";
		case Instructions::LDA_DP:   return "LDA";
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

		case Instructions::LDX_IM:   return "LDX";
		case Instructions::LDX_ABS:  return "LDX";
		case Instructions::LDX_DP:   return "LDX";
		case Instructions::LDX_ABSY: return "LDX";
		case Instructions::LDX_DPY:  return "LDX";

		case Instructions::LDY_IM:   return "LDY";
		case Instructions::LDY_ABS:  return "LDY";
		case Instructions::LDY_DP:   return "LDY";
		case Instructions::LDY_ABSY: return "LDY";
		case Instructions::LDY_DPY:  return "LDY";

		case Instructions::SEP: return "SEP";

		case Instructions::REP: return "REP";

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

		case Instructions::JSR_ABS:   return "JSR";
		case Instructions::JSR_ABSXi: return "JSR";

		case Instructions::JSL: return "JSR";

		case Instructions::CLC: return "CLC";
		case Instructions::CLI: return "CLI";
		case Instructions::CLD: return "CLD";
		case Instructions::CLV: return "CLV";

		default: return "Unknown";
		}
	}

	void CPUDebug::RESB()
	{
		CPU::RESB();
		this->_updateRegistersPanel();
	}
}