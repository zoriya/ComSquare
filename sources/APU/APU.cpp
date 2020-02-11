//
// Created by Melefo on 27/01/2020.
//

#include "APU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

namespace ComSquare::APU
{
	APU::APU() : _dsp(new DSP::DSP)
	{ }

	uint8_t APU::read(uint24_t addr)
	{
		switch (addr) {
		case 0x00:
			return this->_registers.port0;
		case 0x01:
			return this->_registers.port1;
		case 0x02:
			return this->_registers.port2;
		case 0x03:
			return this->_registers.port3;
		default:
			throw InvalidAddress("APU Registers read", addr);
		}
	}

	void APU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00:
			this->_registers.port0 = data;
			break;
		case 0x01:
			this->_registers.port1 = data;
			break;
		case 0x02:
			this->_registers.port2 = data;
			break;
		case 0x03:
			this->_registers.port3 = data;
			break;
		default:
			throw InvalidAddress("APU Registers write", addr);
		}
	}

	int APU::executeInstruction()
	{
		uint8_t opcode = read(this->_internalRegisters.pc++);

		switch (opcode) {
		case 0x00:
			return NOP();
		case 0x20:
			return CLRP();
		case 0x40:
			return SETP();
		case 0x60:
			return CLRC();
		case 0x80:
			return SETC();
		case 0xA0:
			return EI();
		case 0xC0:
			return DI();
		case 0xED:
			return NOTC();
		case 0xEF:
			return SLEEP();
		case 0xFF:
			return STOP();
		default:
			throw InvalidOpcode("APU", opcode);
		}
	}

	void APU::update(uint8_t cycles)
	{
		int total = 0;

		cycles -= this->_paddingCycles;
		while (total < cycles && this->_state == Running)
			total += executeInstruction();
		if (this->_state == Running)
			this->_paddingCycles = total - cycles;
	}
}
