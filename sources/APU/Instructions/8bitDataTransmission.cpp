//
// Created by Melefo on 27/02/2020.
//

#include "../APU.hpp"
#include "../../Utility/Utility.hpp"

namespace ComSquare::APU
{
	int APU::MOV(uint24_t memFrom, uint8_t &regTo, int cycles, bool incrementX)
	{
		regTo = memFrom;
		if (incrementX)
			this->_internalRegisters.x++;
		this->_setNZflags(regTo);
		return cycles;
	}

	int APU::MOV(uint8_t &regFrom, uint24_t memTo, int cycles, bool incrementX)
	{
		this->_internalWrite(memTo, regFrom);
		if (incrementX)
			this->_internalRegisters.x++;
		return cycles;
	}

	int APU::MOV(uint8_t &regFrom, uint8_t &regTo, bool setFlags)
	{
		regTo = regFrom;
		if (setFlags)
			this->_setNZflags(regFrom);
		return 2;
	}

	int APU::MOV(uint24_t memFrom, uint24_t memTo)
	{
		this->_internalWrite(memTo, this->_internalRead(memFrom));
		return 5;
	}
}