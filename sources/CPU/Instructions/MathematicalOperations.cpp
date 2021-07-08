//
// Created by anonymus-raccoon on 2/10/20.
//

#include <iostream>
#include "CPU/CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::ADC(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned value = this->getBus().read(valueAddr) + this->_registers.p.c;
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned maxValue = this->_registers.p.m ? UINT8_MAX : UINT16_MAX;

		this->_registers.p.c = static_cast<unsigned>(this->_registers.a) + value > maxValue;
		if ((this->_registers.a & negativeMask) == (value & negativeMask))
			this->_registers.p.v = (this->_registers.a & negativeMask) != ((this->_registers.a + value) & negativeMask);
		else
			this->_registers.p.v = false;
		this->_registers.a += value;
		if (this->_registers.p.m)
			this->_registers.a %= 0x100;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;

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

	int CPU::SBC(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		bool oldCarry = this->_registers.p.c;

		this->_registers.p.c = this->_registers.a >= value;
		if ((this->_registers.a & negativeMask) == (value & negativeMask))
			this->_registers.p.v = (this->_registers.a & negativeMask) != ((this->_registers.a + value) & negativeMask);
		else
			this->_registers.p.v = false;
		this->_registers.a += ~value + oldCarry;
		if (this->_registers.p.m)
			this->_registers.a %= 0x100;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;

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

	int CPU::ORA(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		this->_registers.a |= value;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;

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

	int CPU::DEX(uint24_t, AddressingMode)
	{
		unsigned negativeMask = this->_registers.p.x_b ? 0x80 : 0x8000;

		this->_registers.x--;
		if (this->_registers.p.x_b)
			this->_registers.xh = 0;
		this->_registers.p.z = this->_registers.x == 0;
		this->_registers.p.n = this->_registers.x & negativeMask;
		return 0;
	}

	int CPU::DEY(uint24_t, AddressingMode)
	{
		unsigned negativeMask = this->_registers.p.x_b ? 0x80 : 0x8000;

		this->_registers.y--;
		if (this->_registers.p.x_b)
			this->_registers.yh = 0;
		this->_registers.p.z = this->_registers.y == 0;
		this->_registers.p.n = this->_registers.y & negativeMask;
		return 0;
	}

	int CPU::CMP(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		unsigned result = this->_registers.a - value;
		if (this->_registers.p.m)
			result %= 0x100;

		this->_registers.p.n = result & negativeMask;
		this->_registers.p.z = result == 0;
		this->_registers.p.c = this->_registers.a >= result;

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
		unsigned value = this->getBus().read(valueAddr++);

		if (this->_registers.p.x_b) {
			uint8_t x = this->_registers.x;
			x -= value;
			this->_registers.p.z = x == 0;
			this->_registers.p.n = x & 0x80u;
		} else {
			value += this->getBus().read(valueAddr) << 8u;
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
		unsigned value = this->getBus().read(valueAddr++);

		this->_registers.p.c = this->_registers.y >= value;
		if (this->_registers.p.x_b) {
			uint8_t y = this->_registers.y;
			y -= value;
			this->_registers.p.z = y == 0;
			this->_registers.p.n = y & 0x80u;
		} else {
			value += this->getBus().read(valueAddr) << 8u;
			uint16_t y = this->_registers.y;
			y -= value;
			this->_registers.p.z = y == 0;
			this->_registers.p.n = y & 0x8000u;
		}
		return !this->_registers.p.x_b + (mode == DirectPage && this->_registers.dl != 0);
	}

	int CPU::AND(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		this->_registers.a &= value;
		this->_registers.p.n = this->_registers.a & negativeMask;
		this->_registers.p.z = this->_registers.a == 0;

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

	int CPU::INC(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;

		unsigned result;
		if (mode == Implied) {
			this->_registers.a++;
			if (this->_registers.p.m)
				this->_registers.ah = 0;
			result = this->_registers.a;
		} else if (!this->_registers.p.m) {
			result = this->getBus().read(valueAddr);
			result += this->getBus().read(valueAddr + 1) << 8u;
			result = (uint16_t)(result + 1);
			this->getBus().write(valueAddr, result);
			this->getBus().write(valueAddr + 1, result << 8u);
		} else {
			result = this->getBus().read(valueAddr);
			result = (uint8_t)(result + 1);
			this->getBus().write(valueAddr, result);
		}

		this->_registers.p.z = result == 0;
		this->_registers.p.n = result & negativeMask;

		switch (mode) {
		case Implied:
			return 0;
		case Absolute:
			return this->_registers.p.m == 0 ? 2 : 0;
		case DirectPage:
		case DirectPageIndexedByX:
			return (this->_registers.p.m == 0 ? 2 : 0) + this->_registers.dl != 0 ;
		case AbsoluteIndexedByX:
			return (this->_registers.p.m == 0 ? 2 : 0) + this->_hasIndexCrossedPageBoundary;
		default:
			return 0;
		}
	}

	int CPU::DEC(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;

		unsigned result;
		if (mode == Implied) {
			this->_registers.a--;
			if (this->_registers.p.m)
				this->_registers.ah = 0;
			result = this->_registers.a;
		} else if (!this->_registers.p.m) {
			result = this->getBus().read(valueAddr);
			result += this->getBus().read(valueAddr + 1) << 8u;
			result = (uint16_t)(result - 1);
			this->getBus().write(valueAddr, result);
			this->getBus().write(valueAddr + 1, result << 8u);
		} else {
			result = this->getBus().read(valueAddr);
			result = (uint8_t)(result - 1);
			this->getBus().write(valueAddr, result);
		}

		this->_registers.p.z = result == 0;
		this->_registers.p.n = result & negativeMask;

		switch (mode) {
		case Implied:
			return 0;
		case Absolute:
			return this->_registers.p.m == 0 ? 2 : 0;
		case DirectPage:
		case DirectPageIndexedByX:
			return (this->_registers.p.m == 0 ? 2 : 0) + this->_registers.dl != 0 ;
		case AbsoluteIndexedByX:
			return (this->_registers.p.m == 0 ? 2 : 0) + this->_hasIndexCrossedPageBoundary;
		default:
			return 0;
		}
	}

	int CPU::EOR(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		this->_registers.a ^= value;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;

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

	int CPU::XBA(uint24_t, AddressingMode)
	{
		int tmp = this->_registers.ah;
		this->_registers.ah = this->_registers.al;
		this->_registers.al = tmp;
		this->_registers.p.n = this->_registers.al & 0x80u;
		this->_registers.p.z = this->_registers.al == 0;
		return 0;
	}
}