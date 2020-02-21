//
// Created by Melefo on 21/02/2020.
//

#include "../APU.hpp"

namespace ComSquare::APU
{
	int APU::BRA()
	{
		int8_t offset = this->_internalRead(this->_internalRegisters.pc++);

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
}