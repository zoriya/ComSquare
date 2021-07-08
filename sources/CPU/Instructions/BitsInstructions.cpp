//
// Created by anonymus-raccoon on 2/20/20.
//

#include <iostream>
#include "Models/Ints.hpp"
#include "CPU/CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::TSB(uint24_t valueAddr, AddressingMode mode)
	{
		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;
		value |= this->_registers.a;
		this->getBus().write(valueAddr, value);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, value >> 8u);

		this->_registers.p.z = value == 0;

		int cycles = 0;
		if (!this->_registers.p.m)
			cycles += 2;
		if (mode == DirectPage)
			cycles += this->_registers.dl != 0;
		return cycles;
	}

	int CPU::TRB(uint24_t valueAddr, AddressingMode mode)
	{
		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		uint16_t newValue = value & ~this->_registers.a;
		this->getBus().write(valueAddr, newValue);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, newValue >> 8u);

		this->_registers.p.z = (value & this->_registers.a) == 0;

		int cycles = 0;
		if (!this->_registers.p.m)
			cycles += 2;
		if (mode == DirectPage)
			cycles += this->_registers.dl != 0;
		return cycles;
	}

	int CPU::BIT(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned negativeMask = this->_registers.p.m ? 0x80u : 0x8000u;
		unsigned value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		if (mode != ImmediateForA) {
			this->_registers.p.n = value & negativeMask;
			this->_registers.p.v = value & (negativeMask >> 1u);
		}
		this->_registers.p.z = (value & this->_registers.a) == 0;

		int cycles = !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByX:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::ASL(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned highByte = this->_registers.p.m ? 0x80u : 0x8000u;

		if (mode == Implied) {
			this->_registers.p.c = this->_registers.a & highByte;
			this->_registers.a <<= 1u;
			this->_registers.p.n = this->_registers.a & highByte;
			this->_registers.p.z = this->_registers.a == 0;
			return 0;
		}

		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		this->_registers.p.c = value & highByte;
		value <<= 1u;
		this->_registers.p.n = value & highByte;
		this->_registers.p.z = value == 0;

		this->getBus().write(valueAddr, value);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, value >> 8u);

		int cycles = 2 * !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByX:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::LSR(uint24_t valueAddr, AddressingMode mode)
	{
		this->_registers.p.n = false;

		if (mode == Implied) {
			this->_registers.p.c = this->_registers.a & 1u;
			this->_registers.a >>= 1u;
			this->_registers.p.z = this->_registers.a == 0;
			return 0;
		}

		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		this->_registers.p.c = value & 1u;
		value >>= 1u;
		this->_registers.p.z = value == 0;

		this->getBus().write(valueAddr, value);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, value >> 8u);

		int cycles = 2 * !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByX:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::ROL(uint24_t valueAddr, AddressingMode mode)
	{
		unsigned highByte = this->_registers.p.m ? 0x80u : 0x8000u;
		bool oldCarry = this->_registers.p.c;

		if (mode == Implied) {
			this->_registers.p.c = this->_registers.a & highByte;
			this->_registers.a <<= 1u;
			this->_registers.a |= oldCarry;
			this->_registers.p.n = this->_registers.a & highByte;
			this->_registers.p.z = this->_registers.a == 0;
			return 0;
		}

		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		this->_registers.p.c = value & highByte;
		value <<= 1u;
		value |= oldCarry;
		this->_registers.p.n = value & highByte;
		this->_registers.p.z = value == 0;

		this->getBus().write(valueAddr, value);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, value >> 8u);

		int cycles = 2 * !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByX:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}

	int CPU::ROR(uint24_t valueAddr, AddressingMode mode)
	{
		this->_registers.p.n = false;
		bool oldCarry = this->_registers.p.c;
		unsigned highByteIndex = this->_registers.p.m ? 7 : 15;

		if (mode == Implied) {
			this->_registers.p.c = this->_registers.a & 1u;
			this->_registers.a >>= 1u;
			this->_registers.a |= oldCarry << highByteIndex;
			this->_registers.p.z = this->_registers.a == 0;
			return 0;
		}

		uint16_t value = this->getBus().read(valueAddr);
		if (!this->_registers.p.m)
			value += this->getBus().read(valueAddr + 1) << 8u;

		this->_registers.p.c = value & 1u;
		value >>= 1u;
		value |= oldCarry << highByteIndex;
		this->_registers.p.z = value == 0;

		this->getBus().write(valueAddr, value);
		if (!this->_registers.p.m)
			this->getBus().write(valueAddr + 1, value >> 8u);

		int cycles = 2 * !this->_registers.p.m;
		switch (mode) {
		case DirectPage:
		case DirectPageIndexedByX:
			cycles += this->_registers.dl != 0;
			break;
		case AbsoluteIndexedByX:
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		default:
			break;
		}
		return cycles;
	}
}