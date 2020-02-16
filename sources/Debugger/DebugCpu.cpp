//
// Created by anonymus-raccoon on 2/14/20.
//

#include "DebugCpu.hpp"

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(ComSquare::CPU::CPU &basicCPU, SNES &snes)
		: CPU::CPU(basicCPU), Ui::CPUView(), _snes(snes)
	{
		this->setupUi(&this->_widget);
		this->_widget.show();
	}

	unsigned CPUDebug::update()
	{
		if (!this->_widget.isVisible()) {
			this->_snes.disableCPUDebugging();
			return 0;
		}

		this->logger->append("Update");
		if (this->_isPaused)
			return 0xFF;
		return CPU::update();
	}
}