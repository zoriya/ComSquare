//
// Created by anonymus-raccoon on 2/13/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::SEP(uint24_t valueAddr)
	{
		this->_registers.p.flags |= this->_bus->read(valueAddr);
	}

	void CPU::REP(uint24_t valueAddr)
	{
		this->_registers.p.flags &= ~this->_bus->read(valueAddr);
		if (this->_isEmulationMode) {
			this->_registers.p.x_b = true;
			this->_registers.p.m = true;
		}
	}

	void CPU::JSR(uint24_t valueAddr)
	{
		this->_push(--this->_registers.pc);
		this->_registers.pc = this->_bus->read(valueAddr) + (this->_bus->read(valueAddr + 1) << 8u);
	}

	void CPU::JSL(uint24_t valueAddr)
	{
		this->_registers.pac--;
		this->_push(this->_registers.pbr);
		this->_push(this->_registers.pc);
		this->_registers.pc = this->_bus->read(valueAddr) + (this->_bus->read(valueAddr + 1) << 8u);
	}

	void CPU::PHA()
	{
		this->_push(this->_registers.a);
	}

	void CPU::PHB()
	{
		this->_push(this->_registers.dbr);
	}

	void CPU::PHD()
	{
		this->_push(this->_registers.d);
	}

	void CPU::PHK()
	{
		this->_push(this->_registers.pbr);
	}

	void CPU::PHP()
	{
		this->_push(this->_registers.p.flags);
	}

	void CPU::PHX()
	{
		this->_push(this->_registers.x);
	}

	void CPU::PHY()
	{
		this->_push(this->_registers.y);
	}

	void CPU::PLA()
	{
		// TODO this register should be poped by 8 if the m flag is 1
		this->_registers.a = this->_pop16();
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & 0x8000u;
	}

	void CPU::PLB()
	{
		this->_registers.dbr = this->_pop();
		this->_registers.p.z = this->_registers.dbr == 0;
		this->_registers.p.n = this->_registers.dbr & 0x80u;
	}

	void CPU::PLD()
	{
		this->_registers.d = this->_pop16();
		this->_registers.p.z = this->_registers.d == 0;
		this->_registers.p.n = this->_registers.d & 0x8000u;
	}

	void CPU::PLP()
	{
		this->_registers.p.flags = this->_pop();
		if (this->_isEmulationMode) {
			this->_registers.p.m = true;
			this->_registers.p.x_b = true;
		}
	}

	void CPU::PLX()
	{
		// TODO this register should be poped by 8 if the x_b is 1
		this->_registers.x = this->_pop16();
		this->_registers.p.z = this->_registers.x == 0;
		this->_registers.p.n = this->_registers.x & 0x8000u;
	}

	void CPU::PLY()
	{
		// TODO this register should be poped by 8 if the x_b is 1
		this->_registers.y = this->_pop16();
		this->_registers.p.z = this->_registers.y == 0;
		this->_registers.p.n = this->_registers.y & 0x8000u;
	}

	void CPU::CLC()
	{
		this->_registers.p.c = false;
	}

	void CPU::CLI()
	{
		this->_registers.p.i = false;
	}

	void CPU::CLD()
	{
		this->_registers.p.d = false;
	}

	void CPU::CLV()
	{
		this->_registers.p.v = false;
	}
}