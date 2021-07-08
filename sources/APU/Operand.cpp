//
// Created by Melefo on 24/02/2020.
//

#include "Models/Ints.hpp"
#include "APU.hpp"

namespace ComSquare::APU
{
	uint8_t APU::_getImmediateData()
	{
		return this->_internalRead(this->_internalRegisters.pc++);
	}

	uint24_t APU::_getDirectAddr()
	{
		uint24_t addr = this->_getImmediateData();

		if (this->_internalRegisters.p)
			addr += 0x100;
		return addr;
	}

	uint24_t APU::_getIndexXAddr()
	{
		uint24_t addr = this->_internalRegisters.x;

		if (this->_internalRegisters.p)
			addr += 0x100;
		return addr;
	}

	uint24_t APU::_getIndexYAddr()
	{
		uint24_t addr = this->_internalRegisters.y;

		if (this->_internalRegisters.p)
			addr += 0x100;
		return addr;
	}

	uint24_t APU::_getDirectAddrByX()
	{
		uint24_t addr = this->_getDirectAddr();

		addr += this->_internalRegisters.x;
		return addr;
	}

	uint24_t APU::_getDirectAddrByY()
	{
		uint24_t addr = this->_getDirectAddr();

		addr += this->_internalRegisters.y;
		return addr;
	}

	uint24_t APU::_getAbsoluteAddr()
	{
		uint24_t addr1 = this->_getImmediateData();
		uint24_t addr2 = this->_getImmediateData();

		return (addr2 << 8u) | addr1;
	}

	uint24_t APU::_getAbsoluteByXAddr()
	{
		uint24_t addr1 = this->_getImmediateData();
		uint24_t addr2 = this->_getImmediateData();
		uint24_t full = (addr2 << 8) | addr1;

		addr1 = this->_internalRead(full + this->_internalRegisters.x);
		addr2 = this->_internalRead(full + this->_internalRegisters.x + 1);

		return (addr2 << 8u) | addr1;
	}

	uint24_t APU::_getAbsoluteAddrByX()
	{
		uint24_t addr = _getAbsoluteAddr();

		return addr + this->_internalRegisters.x;
	}

	uint24_t APU::_getAbsoluteAddrByY()
	{
		uint24_t addr = _getAbsoluteAddr();

		return addr + this->_internalRegisters.y;
	}

	std::pair<uint24_t, uint24_t> APU::_getAbsoluteBit()
	{
		uint24_t addr1 = this->_getImmediateData();
		uint24_t addr2 = this->_getImmediateData();

		uint24_t operandA = (addr2 << 8u) | addr1;
		uint24_t operandB = operandA >> 13u;

		operandA = operandA & 0x1FFFu;
		return std::make_pair(operandA, operandB);
	}

	uint24_t APU::_getAbsoluteDirectByXAddr()
	{
		uint24_t DirectIndexX = this->_getDirectAddr() + this->_internalRegisters.x;

		uint24_t low = this->_internalRead(DirectIndexX++);
		if (this->_internalRegisters.p)
			DirectIndexX += 0x100;
		uint24_t high = this->_internalRead(DirectIndexX);

		return (high << 8u) | low;
	}

	uint24_t APU::_getAbsoluteDirectAddrByY()
	{
		uint24_t DirectIndexX = this->_getDirectAddr();

		uint24_t low = this->_internalRead(DirectIndexX);
		DirectIndexX += 1;
		if (this->_internalRegisters.p)
			DirectIndexX += 0x100;
		uint24_t high = this->_internalRead(DirectIndexX);

		return ((high << 8u) | low) + this->_internalRegisters.y;
	}
}