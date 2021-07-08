//
// Created by Melefo on 26/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::AND(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data = this->_internalRead(operand1) & this->_internalRead(operand2);

		this->_internalWrite(operand1, data);
		this->_setNZflags(data);
		return cycles;
	}

	int APU::ANDacc(uint24_t addr, int cycles)
	{
		this->_internalRegisters.a &= this->_internalRead(addr);
		this->_setNZflags(this->_internalRegisters.a);
		return cycles;
	}

	int APU::OR(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data = this->_internalRead(operand1) | this->_internalRead(operand2);

		this->_internalWrite(operand1, data);
		this->_setNZflags(data);
		return cycles;
	}

	int APU::ORacc(uint24_t addr, int cycles)
	{
		this->_internalRegisters.a |= this->_internalRead(addr);
		this->_setNZflags(this->_internalRegisters.a);
		return cycles;
	}

	int APU::EOR(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data = this->_internalRead(operand1) ^ this->_internalRead(operand2);

		this->_internalWrite(operand1, data);
		this->_setNZflags(data);
		return cycles;
	}

	int APU::EORacc(uint24_t addr, int cycles)
	{
		this->_internalRegisters.a ^= this->_internalRead(addr);
		this->_setNZflags(this->_internalRegisters.a);
		return cycles;
	}
}