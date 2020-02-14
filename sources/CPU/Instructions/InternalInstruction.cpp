//
// Created by anonymus-raccoon on 2/13/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::SEP(uint24_t valueAddr)
	{
		this->_registers.p.flags |= this->_bus->read(valueAddr);
	}

	void CPU::REP(uint24_t valueAddr)
	{
		this->_registers.p.flags &= ~this->_bus->read(valueAddr);
		if (this->_isEmulationMode) {
			this->_registers.p.x_b = true;
			this->_registers.p.m = true;
		}
	}
}