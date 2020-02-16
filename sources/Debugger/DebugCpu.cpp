//
// Created by anonymus-raccoon on 2/14/20.
//

#include <iostream>
#include "DebugCpu.hpp"

namespace ComSquare::Debugger
{
	CPUDebug::CPUDebug(ComSquare::CPU::CPU &basicCPU, SNES &snes)
		: CPU::CPU(basicCPU), _renderer(600, 1000), _snes(snes)
	{
		this->_renderer.setWindowName("CPU's Debugger");
	}

	unsigned CPUDebug::update()
	{
		if (this->_renderer.shouldExit) {
			this->_snes.disableCPUDebugging ();
			return 0;
		}

		this->_renderer.drawScreen();
		this->_renderer.getEvents();
		if (this->_isPaused)
			return 0xFF;
		return CPU::update();
	}
}