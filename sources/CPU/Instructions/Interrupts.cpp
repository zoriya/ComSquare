//
// Created by anonymus-raccoon on 2/10/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::RESB()
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
		this->_isStopped = false;
		return 0;
	}

	int CPU::BRK(uint24_t, AddressingMode)
	{
		if (this->_isEmulationMode) {
			this->_push(this->_registers.pc);
			this->_push(this->_registers.p.flags);
			this->_registers.p.i = true;
			this->_registers.p.d = false;
			this->_registers.pbr = 0x0;
			this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.brk;
		} else {
			this->_push(this->_registers.pbr);
			this->_push(this->_registers.pc);
			this->_push(this->_registers.p.flags);
			this->_registers.p.i = true;
			this->_registers.p.d = false;
			this->_registers.pbr = 0x0;
			this->_registers.pc = this->_cartridgeHeader.nativeInterrupts.brk;
		}
		return !this->_isEmulationMode;
	}

	int CPU::COP(uint24_t, AddressingMode)
	{
		if (this->_isEmulationMode) {
			this->_push(this->_registers.pc);
			this->_push(this->_registers.p.flags);
			this->_registers.p.i = true;
			this->_registers.p.d = false;
			this->_registers.pbr = 0x0;
			this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.cop;
		} else {
			this->_push(this->_registers.pbr);
			this->_push(this->_registers.pc);
			this->_push(this->_registers.p.flags);
			this->_registers.p.i = true;
			this->_registers.p.d = false;
			this->_registers.pbr = 0x0;
			this->_registers.pc = this->_cartridgeHeader.nativeInterrupts.cop;
		}
		return !this->_isEmulationMode;
	}

	int CPU::RTI(uint24_t, AddressingMode)
	{
		this->_registers.p.flags = this->_pop();
		this->_registers.pc = this->_pop16();

		if (!this->_isEmulationMode)
			this->_registers.pbr = this->_pop16();
		return !this->_isEmulationMode;
	}
}