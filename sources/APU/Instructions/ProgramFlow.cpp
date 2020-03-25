//
// Created by Melefo on 21/02/2020.
//

#include "../APU.hpp"

namespace ComSquare::APU
{
	int APU::BRA()
	{
		int8_t offset = this->_getImmediateData();

		this->_internalRegisters.pc += offset;
		return 4;
	}

	int APU::BEQ()
	{
		if (!this->_internalRegisters.z)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BNE()
	{
		if (this->_internalRegisters.z)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BCS()
	{
		if (!this->_internalRegisters.c)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BCC()
	{
		if (this->_internalRegisters.c)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BVS()
	{
		if (!this->_internalRegisters.v)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BVC()
	{
		if (this->_internalRegisters.v)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BMI()
	{
		if (!this->_internalRegisters.n)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BPL()
	{
		if (this->_internalRegisters.n)
			return 2;
		this->BRA();
		return 4;
	}

	int APU::BBS(uint24_t addr, uint8_t bit)
	{
		uint8_t data = this->_internalRead(addr);

		if (!(data & (1u << bit)))
			return 5;
		this->BRA();
		return 7;
	}

	int APU::BBC(uint24_t addr, uint8_t bit)
	{
		uint8_t data = this->_internalRead(addr);

		if (data & (1u << bit))
			return 5;
		this->BRA();
		return 7;
	}

	int APU::CBNE(uint24_t addr, bool by_x)
	{
		uint8_t data = this->_internalRead(addr);

		if (this->_internalRegisters.a == data)
			return 5 + by_x;
		this->BRA();
		return 7 + by_x;
	}

	int APU::DBNZ(bool direct_addr)
	{
		uint8_t data;

		if (direct_addr) {
			uint24_t addr = this->_getDirectAddr();

			data = this->_internalRead(addr);
			this->_internalWrite(addr, --data);
		}
		else
			data = --this->_internalRegisters.y;
		if (!data)
			return 4 + direct_addr;
		this->BRA();
		return 6 + direct_addr;
	}

	int APU::JMP(uint24_t addr, bool by_x)
	{
		this->_internalRegisters.pc = addr;
		if (by_x)
			return 6;
		else
			return 3;
	}
}