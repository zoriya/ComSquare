//
// Created by anonymus-raccoon on 2/13/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::STA(uint24_t addr)
	{
		if (this->_registers.p.m)
			this->_bus->write(addr, this->_registers.al);
		else {
			this->_bus->write(addr, this->_registers.al);
			this->_bus->write(addr + 1, this->_registers.ah);
		}
	}

	void CPU::STX(uint24_t addr)
	{
		if (this->_registers.p.x_b)
			this->_bus->write(addr, this->_registers.xl);
		else {
			this->_bus->write(addr, this->_registers.xl);
			this->_bus->write(addr + 1, this->_registers.xh);
		}
	}
}