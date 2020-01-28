//
// Created by Melefo on 27/01/2020.
//

#include "APU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU
{
	APU::APU()
	{
	}

	uint8_t APU::read(uint24_t addr)
	{
		switch (addr) {
		case 0xF0:
			return this->_internalRegisters.unknown;
		case 0xF2:
			return this->_internalRegisters.dspregAddr;
		case 0xF3:
			return this->_internalRegisters.dspregData;
		case 0xF4:
			return this->_internalRegisters.port0;
		case 0xF5:
			return this->_internalRegisters.port1;
		case 0xF6:
			return this->_internalRegisters.port2;
		case 0xF7:
			return this->_internalRegisters.port3;
		case 0xF8:
			return this->_internalRegisters.regmem1;
		case 0xF9:
			return this->_internalRegisters.regmem2;
		case 0xFD:
			return this->_internalRegisters.counter0;
		case 0xFE:
			return this->_internalRegisters.counter1;
		case 0xFF:
			return this->_internalRegisters.counter2;
		default:
			throw InvalidAddress("APU Internal Registers read", addr);
		}
	}

	void APU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0xF0:
			this->_internalRegisters.unknown = data;
			break;
		case 0xF1:
			this->_internalRegisters.ctrlreg = data;
			break;
		case 0xF2:
			this->_internalRegisters.dspregAddr = data;
			break;
		case 0xF3:
			this->_internalRegisters.dspregData = data;
			break;
		case 0xF4:
			this->_internalRegisters.port0 = data;
			break;
		case 0xF5:
			this->_internalRegisters.port1 = data;
			break;
		case 0xF6:
			this->_internalRegisters.port2 = data;
			break;
		case 0xF7:
			this->_internalRegisters.port3 = data;
			break;
		case 0xF8:
			this->_internalRegisters.regmem1 = data;
			break;
		case 0xF9:
			this->_internalRegisters.regmem2 = data;
			break;
		case 0xFA:
			this->_internalRegisters.timer0 = data;
			break;
		case 0xFB:
			this->_internalRegisters.timer1 = data;
			break;
		case 0xFC:
			this->_internalRegisters.timer2 = data;
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
