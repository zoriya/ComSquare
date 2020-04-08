//
// Created by anonymus-raccoon on 2/20/20.
//

#include <iostream>
#include "../../Models/Int24.hpp"
#include "../CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::TSB(uint24_t valueAddr, AddressingMode mode)
	{
		uint16_t value = this->_bus->read(valueAddr);
		if (!this->_registers.p.m)
			value += this->_bus->read(valueAddr + 1) << 8u;
		value |= this->_registers.a;
		this->_bus->write(valueAddr, value);
		if (!this->_registers.p.m)
			this->_bus->write(valueAddr + 1, value >> 8u);

		this->_registers.p.z = value == 0;

		int cycles = 0;
		if (!this->_registers.p.m)
			cycles += 2;
		if (mode == DirectPage)
			cycles += this->_registers.dl != 0;
		return cycles;
	}
}