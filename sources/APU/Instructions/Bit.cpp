//
// Created by Melefo on 12/02/2020.
//

#include "../APU.hpp"

namespace ComSquare::APU
{
	int APU::SET1(uint24_t dp, uint8_t bit)
	{
		uint8_t data = this->_internalRead(dp);

		this->_internalWrite(dp, data | (1u << bit));
		return 4;
	}

	int APU::CLR1(uint24_t dp, uint8_t bit)
	{
		uint8_t data = this->_internalRead(dp);

		this->_internalWrite(dp, data & ~(1u << bit));
		return 4;
	}

	int APU::TSET1(uint24_t abs)
	{
		uint8_t data = this->_internalRead(abs);

		this->_internalWrite(abs, data | this->_internalRegisters.a);
		this->_internalRegisters.n = data & 0x80u;
		this->_internalRegisters.z = !data;
		return 6;
	}
}