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
	{
		this->_map.Page0 = std::make_shared<Ram::Ram>(Ram::Ram(0x00F0));
		this->_map.Page1 = std::make_shared<Ram::Ram>(Ram::Ram(0x0100));
		this->_map.Memory = std::make_shared<Ram::Ram>(Ram::Ram(0xFDC0));
		this->_map.IPL = std::make_shared<Ram::Ram>(Ram::Ram(0x0040));
	}

	uint8_t APU::_internalRead(uint24_t addr) {
		switch (addr) {
		case 0x0000 ... 0x00EF:
			return this->_map.Page0->read_internal(addr);
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
		case 0x0100 ... 0x01FF:
			return this->_map.Page1->read_internal(addr - 0x0100);
		case 0x0200 ... 0xFFBF:
			return this->_map.Memory->read_internal(addr - 0x200);
		case 0xFFC0 ... 0xFFFF:
			return this->_map.IPL->read_internal(addr - 0xFFC0);
		default:
			throw InvalidAddress("APU Registers read", addr);
		}
	}

	void APU::_internalWrite(uint24_t addr, uint8_t data) {
		switch (addr) {
		case 0x0000 ... 0x00EF:
			this->_map.Page0->write_internal(addr, data);
			break;
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
		case 0x0100 ... 0x01FF:
			this->_map.Page1->write_internal(addr - 0x0100, data);
			break;
		case 0x0200 ... 0xFFBF:
			this->_map.Memory->write_internal(addr - 0x200, data);
			break;
		case 0xFFC0 ... 0xFFFF:
			this->_map.IPL->write_internal(addr - 0xFFC0, data);
			break;
		default:
			throw InvalidAddress("APU Registers write", addr);
		}
	}

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
		uint8_t opcode = this->_internalRead(this->_internalRegisters.pc++);

		switch (opcode) {
		case 0x00:
			return this->NOP();
		case 0x02:
			return SET1(_getDirectAddr(), 0);
		case 0x20:
			return this->CLRP();
		case 0x22:
			return SET1(_getDirectAddr(), 1);
		case 0x40:
			return this->SETP();
		case 0x42:
			return SET1(_getDirectAddr(), 2);
		case 0x60:
			return this->CLRC();
		case 0x62:
			return SET1(_getDirectAddr(), 3);
		case 0x80:
			return this->SETC();
		case 0x82:
			return SET1(_getDirectAddr(), 4);
		case 0xA0:
			return this->EI();
		case 0xA2:
			return SET1(_getDirectAddr(), 5);
		case 0xC0:
			return this->DI();
		case 0xC2:
			return SET1(_getDirectAddr(), 6);
		case 0x32:
			return SET1(_getDirectAddr(), 7);
		case 0xED:
			return this->NOTC();
		case 0xEF:
			return this->SLEEP();
		case 0xFF:
			return this->STOP();
		default:
			throw InvalidOpcode("APU", opcode);
		}
	}

	void APU::update(unsigned cycles)
	{
		unsigned total = 0;

		cycles -= this->_paddingCycles;
		while (total < cycles && this->_state == Running)
			total += this->executeInstruction();
		if (this->_state == Running)
			this->_paddingCycles = total - cycles;
	}

	uint24_t APU::_getDirectAddr()
	{
		uint8_t addr = read(this->_internalRegisters.pc++);

		if (this->_internalRegisters.p)
			addr += 0x100;
		return addr;
	}
}