//
// Created by anonymus-raccoon on 2/13/20.
//

#include "CPU/CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::STA(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.m)
			this->getBus().write(addr, this->_registers.al);
		else {
			this->getBus().write(addr, this->_registers.al);
			this->getBus().write(addr + 1, this->_registers.ah);
		}

		int cycles = !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndirect:
		case DirectPageIndirectLong:
		case DirectPageIndexedByX:
		case DirectPageIndirectIndexedByX:
		case DirectPageIndirectIndexedByY:
		case DirectPageIndirectIndexedByYLong:
			cycles += this->_registers.dl != 0;
		default:
			break;
		}
		return cycles;
	}

	int CPU::STX(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.x_b)
			this->getBus().write(addr, this->_registers.xl);
		else {
			this->getBus().write(addr, this->_registers.xl);
			this->getBus().write(addr + 1, this->_registers.xh);
		}
		return !this->_registers.p.x_b + (mode != Absolute && this->_registers.dl != 0);
	}

	int CPU::STY(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.x_b)
			this->getBus().write(addr, this->_registers.yl);
		else {
			this->getBus().write(addr, this->_registers.yl);
			this->getBus().write(addr + 1, this->_registers.yh);
		}
		return !this->_registers.p.x_b + (mode != Absolute && this->_registers.dl != 0);
	}

	int CPU::STZ(uint24_t addr, AddressingMode mode)
	{
		this->getBus().write(addr, 0x00);
		if (!this->_registers.p.m)
			this->getBus().write(addr + 1, 0x00);
		if (mode == Absolute || mode == AbsoluteIndexedByX)
			return !this->_registers.p.m;
		return !this->_registers.p.m + this->_registers.dl != 0;
	}

	int CPU::LDA(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.m) {
			this->_registers.a = this->getBus().read(addr);
			this->_registers.p.n = this->_registers.al & 0xF0u;
		} else {
			this->_registers.al = this->getBus().read(addr);
			this->_registers.ah = this->getBus().read(addr + 1);
			this->_registers.p.n = this->_registers.a & 0xF000u;
		}
		this->_registers.p.z = this->_registers.a == 0x0;

		int cycles = !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndirect:
		case DirectPageIndirectLong:
		case DirectPageIndexedByX:
		case DirectPageIndirectIndexedByX:
		case DirectPageIndirectIndexedByYLong:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
		case AbsoluteIndexedByY:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		case DirectPageIndirectIndexedByY:
			cycles += this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::LDX(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.x = this->getBus().read(addr);
			this->_registers.p.n = this->_registers.xl & 0xF0u;
		} else {
			this->_registers.xl = this->getBus().read(addr);
			this->_registers.xh = this->getBus().read(addr + 1);
			this->_registers.p.n = this->_registers.x & 0xF000u;
		}
		this->_registers.p.z = this->_registers.x == 0x0;

		int cycles = !this->_registers.p.x_b;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByY:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByY:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::LDY(uint24_t addr, AddressingMode mode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.y = this->getBus().read(addr);
			this->_registers.p.n = this->_registers.yl & 0xF0u;
		} else {
			this->_registers.yl = this->getBus().read(addr);
			this->_registers.yh = this->getBus().read(addr + 1);
			this->_registers.p.n = this->_registers.y & 0xF000u;
		}
		this->_registers.p.z = this->_registers.y == 0x0;

		int cycles = !this->_registers.p.x_b;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByY:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByY:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}
}