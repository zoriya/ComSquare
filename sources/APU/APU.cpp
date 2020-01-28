//
// Created by Melefo on 27/01/2020.
//

#include "APU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU
{
	APU::APU() : _dsp(new DSP::DSP)
	{ }

	uint8_t APU::read(uint24_t addr)
	{
		switch (addr) {
		case 0xF0:
			return this->_registers.unknown;
		case 0xF2:
			return this->_registers.dspregAddr;
		case 0xF3:
			return this->_registers.dspregData;
		case 0xF4:
			return this->_registers.port0;
		case 0xF5:
			return this->_registers.port1;
		case 0xF6:
			return this->_registers.port2;
		case 0xF7:
			return this->_registers.port3;
		case 0xF8:
			return this->_registers.regmem1;
		case 0xF9:
			return this->_registers.regmem2;
		case 0xFD:
			return this->_registers.counter0;
		case 0xFE:
			return this->_registers.counter1;
		case 0xFF:
			return this->_registers.counter2;
		default:
			throw InvalidAddress("APU Internal Registers read", addr);
		}
	}

	void APU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0xF0:
			this->_registers.unknown = data;
			break;
		case 0xF1:
			this->_registers.ctrlreg = data;
			break;
		case 0xF2:
			this->_registers.dspregAddr = data;
			break;
		case 0xF3:
			this->_registers.dspregData = data;
			break;
		case 0xF4:
			this->_registers.port0 = data;
			break;
		case 0xF5:
			this->_registers.port1 = data;
			break;
		case 0xF6:
			this->_registers.port2 = data;
			break;
		case 0xF7:
			this->_registers.port3 = data;
			break;
		case 0xF8:
			this->_registers.regmem1 = data;
			break;
		case 0xF9:
			this->_registers.regmem2 = data;
			break;
		case 0xFA:
			this->_registers.timer0 = data;
			break;
		case 0xFB:
			this->_registers.timer1 = data;
			break;
		case 0xFC:
			this->_registers.timer2 = data;
			break;
		default:
			throw InvalidAddress("APU Internal Registers write", addr);
		}
	}

	bool APU::update()
	{
		throw NotImplementedException();
	}
}
