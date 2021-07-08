//
// Created by Melefo on 12/02/2020.
//

#include "APU/APU.hpp"
#include "Utility/Utility.hpp"

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
		this->_setNZflags(data);
		return 6;
	}

	int APU::TCLR1(uint24_t abs)
	{
		uint8_t data = this->_internalRead(abs);

		this->_internalWrite(abs, data & ~this->_internalRegisters.a);
		this->_setNZflags(data);
		return 6;
	}

	int APU::AND1(std::pair<uint24_t, uint24_t> operand, bool invert)
	{
		if (invert)
			this->_internalRegisters.c &= ~this->_internalRead(operand.first) & (1u << operand.second);
		else
			this->_internalRegisters.c &= this->_internalRead(operand.first) & (1u << operand.second);
		return 4;
	}

	int APU::OR1(std::pair<uint24_t, uint24_t> operand, bool invert)
	{
		if (invert)
			this->_internalRegisters.c |= ~this->_internalRead(operand.first) & (1u << operand.second);
		else
			this->_internalRegisters.c |= this->_internalRead(operand.first) & (1u << operand.second);
		return 5;
	}

	int APU::EOR1(std::pair<uint24_t, uint24_t> operand)
	{
		this->_internalRegisters.c ^= this->_internalRead(operand.first) & (1u << operand.second);
		return 5;
	}

	int APU::NOT1(std::pair<uint24_t, uint24_t> operand)
	{
		this->_internalWrite(operand.first, this->_internalRead(operand.first) ^ (1u << operand.second));
		return 5;
	}

	int APU::MOV1(std::pair<uint24_t, uint24_t> operand, bool to_carry)
	{
		if (to_carry) {
			this->_internalRegisters.c = this->_internalRead(operand.first) & (1u << operand.second);
			return 4;
		}
		else {
			uint24_t mask = (1u << operand.second);

			if (this->_internalRegisters.c)
				this->_internalWrite(operand.first, this->_internalRead(operand.first) | mask);
			else
				this->_internalWrite(operand.first, this->_internalRead(operand.first) & ~mask);
			return 6;
		}
	}
}