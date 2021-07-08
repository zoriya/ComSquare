//
// Created by Melefo on 26/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::INC(uint24_t addr, int cycles)
	{
		uint8_t value = this->_internalRead(addr);

		this->_internalWrite(addr, ++value);
		this->_setNZflags(value);
		return cycles;
	}

	int APU::INCreg(uint8_t &value)
	{
		value++;
		this->_setNZflags(value);
		return 2;
	}

	int APU::DEC(uint24_t addr, int cycles)
	{
		uint8_t value = this->_internalRead(addr);

		this->_internalWrite(addr, --value);
		this->_setNZflags(value);
		return cycles;
	}

	int APU::DECreg(uint8_t &value)
	{
		value--;
		this->_setNZflags(value);
		return 2;
	}
}