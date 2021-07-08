//
// Created by Melefo on 25/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::ASL(uint24_t operand, int cycles, bool accumulator)
	{
		uint8_t value = accumulator ? operand : this->_internalRead(operand);

		this->_internalRegisters.c = value & 0x80u;
		value <<= 1u;
		if (accumulator)
			this->_internalRegisters.a = value;
		else
			this->_internalWrite(operand, value);
		this->_setNZflags(value);
		return cycles;
	}

	int APU::LSR(uint24_t operand, int cycles, bool accumulator)
	{
		uint8_t value = accumulator ? operand : this->_internalRead(operand);

		this->_internalRegisters.c = value & 0x01u;
		value >>= 1u;
		if (accumulator)
			this->_internalRegisters.a = value;
		else
			this->_internalWrite(operand, value);
		this->_internalRegisters.n = value & 0x01u;
		this->_internalRegisters.z = !value;
		return cycles;
	}

	int APU::ROL(uint24_t operand, int cycles, bool accumulator)
	{
		uint8_t value = accumulator ? operand : this->_internalRead(operand);
		uint8_t result = (value << 1u) + this->_internalRegisters.c;

		this->_internalRegisters.c = value & 0x80u;
		if (accumulator)
			this->_internalRegisters.a = result;
		else
			this->_internalWrite(operand, result);
		this->_setNZflags(result);
		return cycles;
	}

	int APU::ROR(uint24_t operand, int cycles, bool accumulator)
	{
		uint8_t value = accumulator ? operand : this->_internalRead(operand);
		uint8_t result = (value >> 1u) + this->_internalRegisters.c;

		this->_internalRegisters.c = value & 0x01u;
		if (accumulator)
			this->_internalRegisters.a = result;
		else
			this->_internalWrite(operand, result);
		this->_internalRegisters.n = result & 0x01u;
		this->_internalRegisters.z = !result;
		return cycles;
	}

	int APU::XCN()
	{
		this->_internalRegisters.a = (this->_internalRegisters.a >> 4u) | (this->_internalRegisters.a << 4u);
		this->_setNZflags(this->_internalRegisters.a);
		return 5;
	}
}