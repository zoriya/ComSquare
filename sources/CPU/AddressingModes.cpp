//
// Created by anonymus-raccoon on 3/20/20.
//

#include "Models/Ints.hpp"
#include "CPU.hpp"

namespace ComSquare::CPU
{
	uint24_t CPU::_getImmediateAddr8Bits()
	{
		uint24_t ret = this->_registers.pac;
		this->_registers.pc++;
		return ret;
	}

	uint24_t CPU::_getImmediateAddr16Bits()
	{
		uint24_t ret = this->_registers.pac;
		this->_registers.pc += 2;
		return ret;
	}

	uint24_t CPU::_getImmediateAddrForA()
	{
		uint24_t effective = this->_registers.pac;
		this->_registers.pc++;
		if (!this->_registers.p.m)
			this->_registers.pc++;
		return effective;
	}

	uint24_t CPU::_getImmediateAddrForX()
	{
		uint24_t effective = this->_registers.pac;
		this->_registers.pc++;
		if (!this->_registers.p.x_b)
			this->_registers.pc++;
		return effective;
	}

	uint24_t CPU::_getDirectAddr()
	{
		uint8_t addr = this->_readPC();
		return this->_registers.d + addr;
	}

	uint24_t CPU::_getAbsoluteAddr()
	{
		uint24_t addr = this->_registers.dbr << 16u;
		addr += this->_readPC();
		addr += this->_readPC() << 8u;
		return addr;
	}

	uint24_t CPU::_getAbsoluteLongAddr()
	{
		uint24_t addr = this->_readPC();
		addr += this->_readPC() << 8u;
		addr += this->_readPC() << 16u;
		return addr;
	}

	uint24_t CPU::_getDirectIndirectIndexedYAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		uint24_t base = this->getBus().read(dp);
		base += this->getBus().read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		if ((base & 0x80000000u) == (((base + this->_registers.y) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return base + this->_registers.y;
	}

	uint24_t CPU::_getDirectIndirectIndexedYLongAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		uint24_t base = this->getBus().read(dp);
		base += this->getBus().read(dp + 1) << 8u;
		base += this->getBus().read(dp + 2) << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndirectIndexedXAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		dp += this->_registers.x;
		uint24_t base = this->getBus().read(dp);
		base += this->getBus().read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndexedByXAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		dp += this->_registers.x;
		return dp;
	}

	uint24_t CPU::_getDirectIndexedByYAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		dp += this->_registers.y;
		return dp;
	}

	uint24_t CPU::_getAbsoluteIndexedByXAddr()
	{
		uint16_t abs = this->_readPC();
		abs += this->_readPC() << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0x80000000u) == (((effective + this->_registers.x) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return effective + this->_registers.x;
	}

	uint24_t CPU::_getAbsoluteIndexedByYAddr()
	{
		uint16_t abs = this->_readPC();
		abs += this->_readPC() << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0x80000000u) == (((effective + this->_registers.y) & 0x80000000u)))
			this->_hasIndexCrossedPageBoundary = true;
		return effective + this->_registers.y;
	}

	uint24_t CPU::_getAbsoluteIndexedByXLongAddr()
	{
		uint24_t lng = this->_readPC();
		lng += this->_readPC() << 8u;
		lng += this->_readPC() << 16u;
		return lng + this->_registers.x;
	}

	uint24_t CPU::_getAbsoluteIndirectAddr()
	{
		uint16_t abs = this->_readPC();
		abs += this->_readPC() << 8u;
		uint24_t effective = this->getBus().read(abs);
		effective += this->getBus().read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getAbsoluteIndirectLongAddr()
	{
		uint16_t abs = this->_readPC();
		abs += this->_readPC() << 8u;
		uint24_t effective = this->getBus().read(abs);
		effective += this->getBus().read(abs + 1) << 8u;
		effective += this->getBus().read(abs + 2) << 16u;
		return effective;
	}

	uint24_t CPU::_getAbsoluteIndirectIndexedByXAddr()
	{
		uint24_t abs = this->_readPC();
		abs += this->_readPC() << 8u;
		abs += this->_registers.x;
		uint24_t effective = this->getBus().read(abs);
		effective += this->getBus().read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		uint24_t effective = this->getBus().read(dp);
		effective += this->getBus().read(dp + 1) << 8u;
		effective += this->_registers.dbr << 16u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectLongAddr()
	{
		uint16_t dp = this->_readPC() + this->_registers.d;
		uint24_t effective = this->getBus().read(dp);
		effective += this->getBus().read(++dp) << 8u;
		effective += this->getBus().read(++dp) << 16u;
		return effective;
	}

	uint24_t CPU::_getStackRelativeAddr()
	{
		return this->_readPC() + this->_registers.s;
	}

	uint24_t CPU::_getStackRelativeIndirectIndexedYAddr()
	{
		uint24_t base = this->_readPC() + this->_registers.s;
		base += this->_registers.dbr << 16u;
		return base + this->_registers.y;
	}
}