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
}