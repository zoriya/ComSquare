//
// Created by Melefo on 25/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::MOVW(uint24_t addr, bool to_ya)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);

		if (to_ya) {
			uint8_t tmp = this->_internalRead(addr2);
			uint16_t value = (tmp << 8) | this->_internalRead(addr);

			this->_internalRegisters.a = value;
			this->_internalRegisters.y = (value >> 8);
			this->_setNZflags(value);
		}
		else {
			this->_internalWrite(addr, this->_internalRegisters.a);
			this->_internalWrite(addr2, this->_internalRegisters.y);
		}
		return 5;
	}
}