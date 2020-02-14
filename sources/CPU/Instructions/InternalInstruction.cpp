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
		this->_registers.a = this->_pop16();
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & 0x8000u;
	}

	void CPU::PLB()
	{

	}

	void CPU::PLD()
	{

	}

	void CPU::PLK()
	{

	}

	void CPU::PLP()
	{

	}

	void CPU::PLX()
	{

	}

	void CPU::PLY()
	{

	}
}