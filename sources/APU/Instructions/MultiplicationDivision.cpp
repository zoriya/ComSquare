//
// Created by Melefo on 25/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::MUL()
	{
		this->_internalRegisters.ya = this->_internalRegisters.y * this->_internalRegisters.a;
		this->_internalRegisters.n = this->_internalRegisters.a & 0x80u;
		this->_internalRegisters.z = !this->_internalRegisters.y;
		return 9;
	}

	int APU::DIV()
	{
		uint16_t ya = this->_internalRegisters.ya;

		this->_internalRegisters.v = this->_internalRegisters.y >= this->_internalRegisters.x;
		this->_internalRegisters.h = (this->_internalRegisters.y & 0x0Fu) >= (this->_internalRegisters.x & 0x0Fu);
		if (this->_internalRegisters.y < (this->_internalRegisters.x << 1u)) {
			this->_internalRegisters.a = ya / this->_internalRegisters.x;
			this->_internalRegisters.y = ya % this->_internalRegisters.x;
		}
		else {
			this->_internalRegisters.a = 0xFF - (ya - (this->_internalRegisters.x << 9u)) / (0x100 - this->_internalRegisters.x);
			this->_internalRegisters.y = this->_internalRegisters.x + (ya - (this->_internalRegisters.x << 9u)) % (0x100 - this->_internalRegisters.x);
		}
		this->_setNZflags(this->_internalRegisters.a);
		return 12;
	}
}