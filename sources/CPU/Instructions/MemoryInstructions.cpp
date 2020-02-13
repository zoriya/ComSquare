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

	void CPU::STY(uint24_t addr)
	{
		if (this->_registers.p.x_b)
			this->_bus->write(addr, this->_registers.yl);
		else {
			this->_bus->write(addr, this->_registers.yl);
			this->_bus->write(addr + 1, this->_registers.yh);
		}
	}

	void CPU::STZ(uint24_t addr)
	{
		this->_bus->write(addr, 0x00);
		if (!this->_registers.p.m)
			this->_bus->write(addr + 1, 0x00);
	}

	void CPU::LDA(uint24_t addr)
	{
		if (this->_registers.p.m) {
			this->_registers.a = this->_bus->read(addr);
			this->_registers.p.n = this->_registers.al & 0xF0u;
		} else {
			this->_registers.al = this->_bus->read(addr);
			this->_registers.ah = this->_bus->read(addr + 1);
			this->_registers.p.n = this->_registers.a & 0xF000u;
		}
		this->_registers.p.z = this->_registers.a == 0x0;
	}
}