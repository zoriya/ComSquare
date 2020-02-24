//
// Created by Melefo on 24/02/2020.
//

#include "../Models/Int24.hpp"
#include "APU.hpp"

namespace ComSquare::APU
{
	uint8_t APU::_getDirectValue()
	{
		return this->_internalRead(this->_internalRegisters.pc++);
	}

	uint24_t APU::_getDirectAddr()
	{
		uint24_t addr = this->_getDirectValue();

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

	uint24_t APU::_getAbsoluteAddr()
	{
		uint24_t addr1 = this->_getDirectValue();
		uint24_t addr2 = this->_getDirectValue();

		return (addr2 << 8u) | addr1;
	}

	uint24_t APU::_getAbsoluteAddrByX()
	{
		uint24_t addr1 = this->_getDirectValue() + this->_internalRegisters.x;
		uint24_t addr2 = this->_getDirectValue() + this->_internalRegisters.x++;

		return (addr2 << 8u) | addr1;
	}

	std::pair<uint24_t, uint24_t> APU::_getAbsoluteBit()
	{
		uint24_t addr1 = _getDirectValue();
		uint24_t addr2 = this->_getDirectValue();

		uint24_t operandA = (addr2 << 8u) | addr1;
		uint24_t operandB = operandA >> 13u;

		operandA = operandA & 0x1FFFu;
		return std::make_pair(operandA, operandB);
	}
}