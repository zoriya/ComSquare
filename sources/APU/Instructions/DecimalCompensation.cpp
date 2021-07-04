//
// Created by Melefo on 25/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::DAA()
	{
		if (this->_internalRegisters.c || this->_internalRegisters.a > 0x99) {
			this->_internalRegisters.c = true;
			this->_internalRegisters.a += 0x60;
		}
		if (this->_internalRegisters.h || (this->_internalRegisters.a & 0x0Fu) > 0x09) {
			this->_internalRegisters.a += 0x06;
		}
		this->_setNZflags(this->_internalRegisters.a);
		return 3;
	}

	int APU::DAS()
	{
		if (!this->_internalRegisters.c || this->_internalRegisters.a > 0x99) {
			this->_internalRegisters.c = false;
			this->_internalRegisters.a -= 0x60;
		}
		if (!this->_internalRegisters.h || (this->_internalRegisters.a & 0x0Fu) > 0x09) {
			this->_internalRegisters.a -= 0x06;
		}
		this->_setNZflags(this->_internalRegisters.a);
		return 3;
	}
	std::string APU::getValueName(uint24_t) const
	{
		// TODO implement this
		return "???";
	}
}