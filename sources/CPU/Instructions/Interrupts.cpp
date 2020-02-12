//
// Created by anonymus-raccoon on 2/10/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	unsigned CPU::RESB()
	{
		this->_registers.p.i = true;
		this->_registers.p.d = false;
		this->_isEmulationMode = true;
		this->_registers.p.m = true;
		this->_registers.p.x_b = true;
		this->_registers.dbr = 0x00;
		this->_registers.pbr = 0x00;
		this->_registers.d = 0x0000;
		this->_registers.sh = 0x01; // the low bit of the stack pointer is undefined on reset.
		this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.reset;
		return 0;
	}

	unsigned CPU::BRK()
	{
		// TODO rework this. The PC should be pushed to the stack.
		// Info here: http://softpixel.com/~cwright/sianse/docs/65816NFO.HTM at BRK Software Break
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