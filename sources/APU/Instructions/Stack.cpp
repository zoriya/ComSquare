//
// Created by Melefo on 21/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::PUSH(uint8_t value)
	{
		this->_internalWrite(this->_internalRegisters.sp-- + 0x0100u, value);
		return 4;
	}

	int APU::POP(uint8_t &destination)
	{
		destination = this->_internalRead(++this->_internalRegisters.sp + 0x100u);
		return 4;
	}
}