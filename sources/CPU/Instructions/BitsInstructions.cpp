//
// Created by anonymus-raccoon on 2/20/20.
//

#include "../../Models/Int24.hpp"
#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::AND(uint24_t valueAddr)
	{
		unsigned negativeMask = this->_isEmulationMode ? 0x80u : 0x8000u;
		unsigned value = this->_bus->read(valueAddr);
		if (!this->_registers.p.m)
			value += this->_bus->read(valueAddr + 1) << 8u;

		this->_registers.a &= value;
		this->_registers.p.n = this->_registers.a & negativeMask;
		this->_registers.p.z = this->_registers.a == 0;
	}
}