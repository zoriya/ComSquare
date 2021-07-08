//
// Created by Melefo on 25/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::INCW(uint24_t addr)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);
		uint16_t value = ((this->_internalRead(addr2) << 8u) | this->_internalRead(addr)) + 1u;

		this->_internalWrite(addr, value);
		this->_internalWrite(addr2, value >> 8u);
		this->_internalRegisters.n = value & 0x8000u;
		this->_internalRegisters.z = !value;
		return 6;
	}

	int APU::DECW(uint24_t addr)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);
		uint16_t value = ((this->_internalRead(addr2) << 8u) | this->_internalRead(addr)) - 1u;

		this->_internalWrite(addr, value);
		this->_internalWrite(addr2, value >> 8u);
		this->_internalRegisters.n = value & 0x8000u;
		this->_internalRegisters.z = !value;
		return 6;
	}

	int APU::ADDW(uint24_t addr)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);
		uint16_t value = ((this->_internalRead(addr2) << 8u) | this->_internalRead(addr));
		uint24_t result = this->_internalRegisters.ya + value;

		this->_internalRegisters.v = (~(this->_internalRegisters.ya ^ value) & (this->_internalRegisters.ya ^ result)) & 0x8000u;
		this->_internalRegisters.c = result > 0xFFFFu;
		this->_internalRegisters.h = ((this->_internalRegisters.ya & 0x0FFFu) + (value & 0x0FFFu)) > 0x0FFFu;
		this->_internalRegisters.z = !result;
		this->_internalRegisters.n = result & 0x8000u;
		this->_internalRegisters.ya = result;
		return 5;
	}

	int APU::SUBW(uint24_t addr)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);
		uint16_t value = ((this->_internalRead(addr2) << 8u) | this->_internalRead(addr));
		uint24_t result = this->_internalRegisters.ya - value;
		uint16_t tmp = ((this->_internalRegisters.ya & 0x0F00u) - (value & 0x0F00u)) >> 8u;

		if ((this->_internalRegisters.ya & 0x00FFu) < (value & 0x00FFu))
			tmp--;
		this->_internalRegisters.v = ((this->_internalRegisters.ya ^ value) & (this->_internalRegisters.ya ^ result)) & 0x8000u;
		this->_internalRegisters.c = result <= 0xFFFFu;
		this->_internalRegisters.h = tmp <= 0x000F;
		this->_internalRegisters.z = !result;
		this->_internalRegisters.n = result & 0x8000u;
		this->_internalRegisters.ya = result;
		return 5;
	}

	int APU::CMPW(uint24_t addr)
	{
		uint24_t addr2 = addr + 1 + (this->_internalRegisters.p ? 0x0100 : 0);
		uint16_t value = ((this->_internalRead(addr2) << 8u) | this->_internalRead(addr));
		uint24_t result = this->_internalRegisters.ya - value;

		this->_internalRegisters.z = !result;
		this->_internalRegisters.n = result & 0x8000u;
		this->_internalRegisters.c = this->_internalRegisters.ya >= value;
		return 4;
	}
}