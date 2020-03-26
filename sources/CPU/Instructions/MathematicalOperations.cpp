//
// Created by anonymus-raccoon on 2/10/20.
//

#include <iostream>
#include "../CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::ADC(uint24_t valueAddr)
	{
		unsigned value = this->_bus->read(valueAddr) + this->_registers.p.c;
		if (!this->_registers.p.m)
			value += this->_bus->read(valueAddr + 1) << 8u;
		unsigned negativeMask = this->_isEmulationMode ? 0x80u : 0x8000u;
		unsigned maxValue = this->_isEmulationMode ? UINT8_MAX : UINT16_MAX;

		this->_registers.p.c = static_cast<unsigned>(this->_registers.a) + value > maxValue;
		if ((this->_registers.a & negativeMask) == (value & negativeMask))
			this->_registers.p.v = (this->_registers.a & negativeMask) != ((this->_registers.a + value) & negativeMask);
		else
			this->_registers.p.v = false;
		this->_registers.a += value;
		if (this->_isEmulationMode)
			this->_registers.a %= 0x100;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;
		return 0;
	}

	int CPU::SBC(uint24_t valueAddr)
	{
		unsigned negativeMask = this->_isEmulationMode ? 0x80u : 0x8000u;
		unsigned value = this->_bus->read(valueAddr);
		if (!this->_registers.p.m)
			value += this->_bus->read(valueAddr + 1) << 8u;
		bool oldCarry = this->_registers.p.c;

		this->_registers.p.c = this->_registers.a >= value;
		if ((this->_registers.a & negativeMask) == (value & negativeMask))
			this->_registers.p.v = (this->_registers.a & negativeMask) != ((this->_registers.a + value) & negativeMask);
		else
			this->_registers.p.v = false;
		this->_registers.a += ~value + oldCarry;
		if (this->_isEmulationMode)
			this->_registers.a %= 0x100;
		this->_registers.p.z = this->_registers.a == 0;
		this->_registers.p.n = this->_registers.a & negativeMask;
		return 0;
	}
}