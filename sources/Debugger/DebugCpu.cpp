//
// Created by anonymus-raccoon on 2/14/20.
//

#include <iostream>
#include "DebugCpu.hpp"

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(ComSquare::CPU::CPU &basicCPU, SNES &snes)
		: CPU::CPU(basicCPU), _snes(snes)
	{
		this->_ui.setupUi(&this->_widget);
		this->_widget.show();
	}

	unsigned CPUDebug::update()
	{
		if (!this->_widget.isVisible()) {
			this->_snes.disableCPUDebugging();
			return 0;
		}

//		this->_renderer.drawScreen();
//		this->_renderer.getEvents();
		if (this->_isPaused)
			return 0xFF;
		return CPU::update();
	}
}