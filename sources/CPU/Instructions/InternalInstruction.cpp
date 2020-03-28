//
// Created by anonymus-raccoon on 2/13/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::SEC(uint24_t, AddressingMode)
	{
		this->_registers.p.c = true;
		return 0;
	}

	int CPU::SED(uint24_t, AddressingMode)
	{
		this->_registers.p.d = true;
		return 0;
	}

	int CPU::SEI(uint24_t, AddressingMode)
	{
		this->_registers.p.i = true;
		return 0;
	}

	int CPU::CLC(uint24_t, AddressingMode)
	{
		this->_registers.p.c = false;
		return 0;
	}

	int CPU::CLI(uint24_t, AddressingMode)
	{
		this->_registers.p.i = false;
		return 0;
	}

	int CPU::CLD(uint24_t, AddressingMode)
	{
		this->_registers.p.d = false;
		return 0;
	}

	int CPU::CLV(uint24_t, AddressingMode)
	{
		this->_registers.p.v = false;
		return 0;
	}

	int CPU::SEP(uint24_t valueAddr, AddressingMode)
	{
		this->_registers.p.flags |= this->_bus->read(valueAddr);
		return 0;
	}

	int CPU::REP(uint24_t valueAddr, AddressingMode)
	{
		this->_registers.p.flags &= ~this->_bus->read(valueAddr);
		if (this->_isEmulationMode) {
			this->_registers.p.x_b = true;
			this->_registers.p.m = true;
		}
		return 0;
	}

	int CPU::JSR(uint24_t valueAddr, AddressingMode)
	{
		this->_push(--this->_registers.pc);
		this->_registers.pc = valueAddr;
		return 0;
	}

	int CPU::JSL(uint24_t valueAddr, AddressingMode)
	{
		this->_registers.pac--;
		this->_push(this->_registers.pbr);
		this->_push(this->_registers.pc);
		this->_registers.pac = valueAddr;
		return 0;
	}

	int CPU::PHA(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.a);
		return !this->_registers.p.m;
	}

	int CPU::PHB(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.dbr);
		return 0;
	}

	int CPU::PHD(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.d);
		return 0;
	}

	int CPU::PHK(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.pbr);
		return 0;
	}

	int CPU::PHP(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.p.flags);
		return 0;
	}

	int CPU::PHX(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.x);
		return !this->_registers.p.x_b;
	}

	int CPU::PHY(uint24_t, AddressingMode)
	{
		this->_push(this->_registers.y);
		return !this->_registers.p.x_b;
	}

	int CPU::PLA(uint24_t, AddressingMode)
	{
		if (this->_registers.p.m) {
			this->_registers.a = this->_pop();
			this->_registers.ah = 0;
		} else
			this->_registers.a = this->_pop16();
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & 0x8000u;
		return !this->_registers.p.m;
	}

	int CPU::PLB(uint24_t, AddressingMode)
	{
		this->_registers.dbr = this->_pop();
		this->_registers.p.z = this->_registers.dbr == 0;
		this->_registers.p.n = this->_registers.dbr & 0x80u;
		return 0;
	}

	int CPU::PLD(uint24_t, AddressingMode)
	{
		this->_registers.d = this->_pop16();
		this->_registers.p.z = this->_registers.d == 0;
		this->_registers.p.n = this->_registers.d & 0x8000u;
		return 0;
	}

	int CPU::PLP(uint24_t, AddressingMode)
	{
		this->_registers.p.flags = this->_pop();
		if (this->_isEmulationMode) {
			this->_registers.p.m = true;
			this->_registers.p.x_b = true;
		}
		return 0;
	}

	int CPU::PLX(uint24_t, AddressingMode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.x = this->_pop();
			this->_registers.xh = 0;
		} else
			this->_registers.x = this->_pop16();
		this->_registers.p.z = this->_registers.x == 0;
		this->_registers.p.n = this->_registers.x & 0x8000u;
		return !this->_registers.p.x_b;
	}

	int CPU::PLY(uint24_t, AddressingMode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.y = this->_pop();
			this->_registers.yh = 0;
		} else
			this->_registers.y = this->_pop16();
		this->_registers.p.z = this->_registers.y == 0;
		this->_registers.p.n = this->_registers.y & 0x8000u;
		return !this->_registers.p.x_b;
	}

	int CPU::XCE(uint24_t, AddressingMode)
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
		return 0;
	}

	int CPU::INX(uint24_t, AddressingMode)
	{
		this->_registers.x++;

		if (this->_registers.p.x_b)
			this->_registers.x %= 0x100;

		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;
		this->_registers.p.z = this->_registers.x == 0;
		this->_registers.p.n = this->_registers.x & negativeFlag;
		return 0;
	}

	int CPU::INY(uint24_t, AddressingMode)
	{
		this->_registers.y++;

		if (this->_registers.p.x_b)
			this->_registers.y %= 0x100;

		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;
		this->_registers.p.z = this->_registers.y == 0;
		this->_registers.p.n = this->_registers.y & negativeFlag;
		return 0;
	}

	int CPU::CPX(uint24_t valueAddr, AddressingMode mode)
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
		return !this->_registers.p.x_b + (mode == DirectPage && this->_registers.dl != 0);
	}

	int CPU::CPY(uint24_t valueAddr, AddressingMode mode)
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
		return !this->_registers.p.x_b + (mode == DirectPage && this->_registers.dl != 0);
	}

	int CPU::BCC(uint24_t valueAddr, AddressingMode)
	{
		if (!this->_registers.p.c)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.c + this->_isEmulationMode;
	}

	int CPU::BCS(uint24_t valueAddr, AddressingMode)
	{
		if (this->_registers.p.c)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.c + this->_isEmulationMode;
	}

	int CPU::BEQ(uint24_t valueAddr, AddressingMode)
	{
		if (this->_registers.p.z)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.z + this->_isEmulationMode;
	}

	int CPU::BNE(uint24_t valueAddr, AddressingMode)
	{
		if (!this->_registers.p.z)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.z + this->_isEmulationMode;
	}

	int CPU::BMI(uint24_t valueAddr, AddressingMode)
	{
		if (this->_registers.p.n)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.n + this->_isEmulationMode;
	}

	int CPU::BPL(uint24_t valueAddr, AddressingMode)
	{
		if (!this->_registers.p.n)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.n + this->_isEmulationMode;
	}

	int CPU::BRA(uint24_t valueAddr, AddressingMode)
	{
		this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_isEmulationMode;
	}

	int CPU::BRL(uint24_t valueAddr, AddressingMode)
	{
		unsigned value = this->_bus->read(valueAddr);
		value += this->_bus->read(valueAddr + 1) << 8u;

		this->_registers.pc += static_cast<int16_t>(value);
		return 0;
	}

	int CPU::BVC(uint24_t valueAddr, AddressingMode)
	{
		if (!this->_registers.p.v)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return !this->_registers.p.v + this->_isEmulationMode;
	}

	int CPU::BVS(uint24_t valueAddr, AddressingMode)
	{
		if (this->_registers.p.v)
			this->_registers.pc += static_cast<int8_t>(this->_bus->read(valueAddr));
		return this->_registers.p.v + this->_isEmulationMode;
	}

	int CPU::JMP(uint24_t value, AddressingMode)
	{
		this->_registers.pc = value;
		return 0;
	}

	int CPU::JML(uint24_t value, AddressingMode)
	{
		this->_registers.pac = value;
		return 0;
	}

	int CPU::NOP(uint24_t, AddressingMode)
	{
		return 0;
	}
}