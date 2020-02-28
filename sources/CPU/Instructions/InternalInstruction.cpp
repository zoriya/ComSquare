//
// Created by anonymus-raccoon on 2/13/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::SEC()
	{
		this->_registers.p.c = true;
	}

	void CPU::SED()
	{
		this->_registers.p.d = true;
	}

	void CPU::SEI()
	{
		this->_registers.p.i = true;
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

	void CPU::SEP(uint24_t value)
	{
		this->_registers.p.flags |= value;
	}

	void CPU::REP(uint24_t value)
	{
		this->_registers.p.flags &= ~value;
		if (this->_isEmulationMode) {
			this->_registers.p.x_b = true;
			this->_registers.p.m = true;
		}
	}

	void CPU::JSR(uint24_t value)
	{
		this->_push(--this->_registers.pc);
		this->_registers.pc = value;
	}

	void CPU::JSL(uint24_t value)
	{
		this->_registers.pac--;
		this->_push(this->_registers.pbr);
		this->_push(this->_registers.pc);
		this->_registers.pac = value;
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

	void CPU::XCE()
	{
		bool oldCarry = this->_registers.p.c;
		this->_registers.p.c = this->_isEmulationMode;
		this->_isEmulationMode = oldCarry;

		if (!this->_isEmulationMode)  {
			this->_registers.p.m = true;
			this->_registers.p.x_b = true;
			this->_registers.xh = 0;
			this->_registers.yh = 0;
		}
	}

	void CPU::INX()
	{
		this->_registers.x++;

		if (this->_registers.p.x_b)
			this->_registers.x %= 0x100;

		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;
		this->_registers.p.z = this->_registers.x == 0;
		this->_registers.p.n = this->_registers.x & negativeFlag;
	}

	void CPU::INY()
	{
		this->_registers.y++;

		if (this->_registers.p.x_b)
			this->_registers.y %= 0x100;

		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;
		this->_registers.p.z = this->_registers.y == 0;
		this->_registers.p.n = this->_registers.y & negativeFlag;
	}

	void CPU::CPX(uint24_t valueAddr)
	{
		unsigned value = this->_bus->read(valueAddr++);

		if (this->_registers.p.x_b) {
			uint8_t x = this->_registers.x;
			x -= value;
			this->_registers.p.z = x == 0;
			this->_registers.p.n = x & 0x80u;
		} else {
			value += this->_bus->read(valueAddr) << 8u;
			uint16_t x = this->_registers.x;
			x -= value;
			this->_registers.p.z = x == 0;
			this->_registers.p.n = x & 0x8000u;
		}
		this->_registers.p.c = this->_registers.x >= value;
	}

	void CPU::CPY(uint24_t valueAddr)
	{
		unsigned value = this->_bus->read(valueAddr++);

		this->_registers.p.c = this->_registers.y >= value;
		if (this->_registers.p.x_b) {
			uint8_t y = this->_registers.y;
			y -= value;
			this->_registers.p.z = y == 0;
			this->_registers.p.n = y & 0x80u;
		} else {
			value += this->_bus->read(valueAddr) << 8u;
			uint16_t y = this->_registers.y;
			y -= value;
			this->_registers.p.z = y == 0;
			this->_registers.p.n = y & 0x8000u;
		}
	}

	bool CPU::BCC(uint24_t valueAddr)
	{
		if (!this->_registers.p.c)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.c;
	}

	bool CPU::BCS(uint24_t valueAddr)
	{
		if (this->_registers.p.c)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.c;
	}

	bool CPU::BEQ(uint24_t valueAddr)
	{
		if (this->_registers.p.z)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.z;
	}

	bool CPU::BNE(uint24_t valueAddr)
	{
		if (!this->_registers.p.z)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.z;
	}

	bool CPU::BMI(uint24_t valueAddr)
	{
		if (this->_registers.p.n)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.n;
	}

	bool CPU::BPL(uint24_t valueAddr)
	{
		if (!this->_registers.p.n)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.n;
	}

	bool CPU::BRA(uint24_t valueAddr)
	{
		this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return true;
	}

	bool CPU::BRL(uint24_t valueAddr)
	{
		unsigned value = this->_bus->read(valueAddr);
		value += this->_bus->read(valueAddr + 1) << 8u;

		this->_registers.pac += static_cast<int16_t>(value);
		return true;
	}

	bool CPU::BVC(uint24_t valueAddr)
	{
		if (!this->_registers.p.v)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.v;
	}

	bool CPU::BVS(uint24_t valueAddr)
	{
		if (this->_registers.p.v)
			this->_registers.pac += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.v;
	}
}