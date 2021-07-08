//
// Created by Melefo on 27/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::ADC(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data1 = this->_internalRead(operand1);
		uint8_t data2 = this->_internalRead(operand2);
		uint24_t result = data1 + data2 + this->_internalRegisters.c;

		this->_internalRegisters.v = (~(data1 ^ data2) & (data1 ^ result)) & 0x80u;
		this->_internalRegisters.h = ((data1 & 0x0Fu) + (data2 & 0x0Fu) + this->_internalRegisters.c) > 0x0Fu;
		this->_internalRegisters.c = result > 0xFF;
		this->_setNZflags(result);
		this->_internalWrite(operand1, result);
		return cycles;
	}

	int APU::ADCacc(uint24_t addr, int cycles)
	{
		uint8_t data = this->_internalRead(addr);
		uint24_t result = this->_internalRegisters.a + data + this->_internalRegisters.c;

		this->_internalRegisters.v = (~(this->_internalRegisters.a ^ data) & (this->_internalRegisters.a ^ result)) & 0x80u;
		this->_internalRegisters.h = ((this->_internalRegisters.a & 0x0Fu) + (data & 0x0Fu) + this->_internalRegisters.c) > 0x0Fu;
		this->_internalRegisters.c = result > 0xFF;
		this->_setNZflags(result);
		this->_internalRegisters.a = result;
		return cycles;
	}

	int APU::SBC(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data1 = this->_internalRead(operand1);
		uint8_t data2 = this->_internalRead(operand2);
		uint24_t result = data1 - data2 - (this->_internalRegisters.c ^ 0x01u);

		this->_internalRegisters.v = ((data1 ^ data2) & (data1 ^ result)) & 0x80u;
		this->_internalRegisters.h = ((result & 0x0Fu) - (data1 & 0x0Fu) + this->_internalRegisters.c) > 0x0Fu;
		this->_internalRegisters.c = result <= 0xFF;
		this->_setNZflags(result);
		this->_internalWrite(operand1, result);
		return cycles;
	}

	int APU::SBCacc(uint24_t addr, int cycles)
	{
		uint8_t data = this->_internalRead(addr);
		uint24_t result = this->_internalRegisters.a - data - (this->_internalRegisters.c ^ 0x01);

		this->_internalRegisters.v = ((this->_internalRegisters.a ^ data) & (this->_internalRegisters.a ^ result)) & 0x80u;
		this->_internalRegisters.h = ((result & 0x0Fu) - (this->_internalRegisters.a & 0x0Fu) + this->_internalRegisters.c) > 0x0Fu;
		this->_internalRegisters.c = result <= 0xFF;
		this->_setNZflags(result);
		this->_internalRegisters.a = result;
		return cycles;
	}

	int APU::CMP(uint24_t operand1, uint24_t operand2, int cycles)
	{
		uint8_t data1 = this->_internalRead(operand1);

		this->_internalRegisters.c = data1 >= operand2;
		this->_setNZflags(data1 - operand2);
		return cycles;
	}

	int APU::CMPreg(uint8_t &reg, uint24_t addr, int cycles)
	{
		uint8_t data = this->_internalRead(addr);

		this->_internalRegisters.c = reg >= data;
		this->_setNZflags(reg - data);
		return cycles;
	}
}