//
// Created by anonymus-raccoon on 2/14/20.
//

#include "DebugCpu.hpp"

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(ComSquare::CPU::CPU &basicCPU, SNES &snes)
		: CPU::CPU(basicCPU), QMainWindow(), _ui(), _snes(snes)
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this);
		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		this->show();
	}

	unsigned CPUDebug::update()
	{
		if (!this->isVisible()) {
			this->_snes.disableCPUDebugging();
			return 0;
		}

		if (this->_isPaused)
			return 0xFF;
		return CPU::update();
	}

	unsigned CPUDebug::_executeInstruction(uint8_t opcode)
	{
		this->_ui.logger->append(this->_getInstructionString(opcode).c_str());
		return CPU::_executeInstruction(opcode);
	}

	std::string CPUDebug::_getInstructionString(uint8_t opcode)
	{
		return "Super";
	}

	void CPUDebug::pause()
	{
		this->_isPaused = !this->_isPaused;
		if (this->_isPaused)
			this->_ui.actionPause->setText("Resume");
		else
			this->_ui.actionPause->setText("Pause");
	}
}