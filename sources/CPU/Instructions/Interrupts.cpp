//
// Created by anonymus-raccoon on 2/10/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	unsigned CPU::BRK()
	{
		this->_registers.pc += 2;

		this->_registers.p.i = true;
		if (this->_isEmulationMode)
			this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.brk;
		else
			this->_registers.pc = this->_cartridgeHeader.nativeInterrupts.brk;
		this->_registers.p.d = false;
		return !this->_isEmulationMode;
	}
}