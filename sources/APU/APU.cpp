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
		case 0xEF:
			return SLEEP();
		case 0xFF:
			return STOP();
		default:
			throw InvalidOpcode("APU", opcode);
		}
	}

	int APU::update()
	{
		int cycles = 0;

		while (this->_state == Running)
			cycles += executeInstruction();
		return cycles;
	}
}
