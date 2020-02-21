//
// Created by Melefo on 27/01/2020.
//

#include <cstring>
#include <iostream>
#include "APU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

namespace ComSquare::APU
{
	APU::APU(std::shared_ptr<MemoryMap> &map) :
		_map(map),
		_dsp(new DSP::DSP)
	{
		this->reset();
	}

	uint8_t APU::_internalRead(uint24_t addr) {
		switch (addr) {
		case 0x0000 ... 0x00EF:
			return this->_map->Page0.read_internal(addr);
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
			return this->_map->Page1.read_internal(addr - 0x0100);
		case 0x0200 ... 0xFFBF:
			return this->_map->Memory.read_internal(addr - 0x200);
		case 0xFFC0 ... 0xFFFF:
			return this->_map->IPL.read_internal(addr - 0xFFC0);
		default:
			throw InvalidAddress("APU Registers read", addr);
		}
	}

	void APU::_internalWrite(uint24_t addr, uint8_t data) {
		switch (addr) {
		case 0x0000 ... 0x00EF:
			this->_map->Page0.write_internal(addr, data);
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
			this->_map->Page1.write_internal(addr - 0x0100, data);
			break;
		case 0x0200 ... 0xFFBF:
			this->_map->Memory.write_internal(addr - 0x200, data);
			break;
		case 0xFFC0 ... 0xFFFF:
			this->_map->IPL.write_internal(addr - 0xFFC0, data);
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

	void APU::reset()
	{
	}

	int APU::_executeInstruction()
	{
		uint8_t opcode = this->_internalRead(this->_internalRegisters.pc);

		switch (opcode) {
		case 0x00:
			return this->NOP();
		case 0x02:
			return this->SET1(this->_getDirectAddr(), 0);
		case 0x0A:
			return this->OR1(this->_getAbsoluteBit());
		case 0x0D:
			return this->PUSH(this->_internalRegisters.psw);
		case 0x0E:
			return this->TSET1(this->_getAbsoluteAddr());
		case 0x12:
			return this->CLR1(this->_getDirectAddr(), 0);
		case 0x20:
			return this->CLRP();
		case 0x22:
			return this->SET1(this->_getDirectAddr(), 1);
		case 0x2A:
			return this->OR1(this->_getAbsoluteBit(), true);
		case 0x2D:
			return this->PUSH(this->_internalRegisters.a);
		case 0x32:
			return this->CLR1(this->_getDirectAddr(), 1);
		case 0x40:
			return this->SETP();
		case 0x42:
			return this->SET1(this->_getDirectAddr(), 2);
		case 0x4A:
			return this->AND1(this->_getAbsoluteBit());
		case 0x4D:
			return this->PUSH(this->_internalRegisters.x);
		case 0x4E:
			return this->TCLR1(this->_getAbsoluteAddr());
		case 0x52:
			return this->CLR1(this->_getDirectAddr(), 2);
		case 0x60:
			return this->CLRC();
		case 0x62:
			return this->SET1(this->_getDirectAddr(), 3);
		case 0x6A:
			return this->AND1(this->_getAbsoluteBit(), true);
		case 0x6D:
			return this->PUSH(this->_internalRegisters.y);
		case 0x72:
			return this->CLR1(this->_getDirectAddr(), 3);
		case 0x80:
			return this->SETC();
		case 0x82:
			return this->SET1(this->_getDirectAddr(), 4);
		case 0x8A:
			return this->EOR1(this->_getAbsoluteBit());
		case 0x8E:
			return this->POP(this->_internalRegisters.psw);
		case 0x92:
			return this->CLR1(this->_getDirectAddr(), 4);
		case 0xA0:
			return this->EI();
		case 0xA2:
			return this->SET1(this->_getDirectAddr(), 5);
		case 0xAA:
			return this->MOV1(this->_getAbsoluteBit(), true);
		case 0xAE:
			return this->POP(this->_internalRegisters.a);
		case 0xB2:
			return this->CLR1(this->_getDirectAddr(), 5);
		case 0xC0:
			return this->DI();
		case 0xC2:
			return this->SET1(this->_getDirectAddr(), 6);
		case 0xC1:
			return this->MOV1(this->_getAbsoluteBit());
		case 0xCE:
			return this->POP(this->_internalRegisters.x);
		case 0xD2:
			return this->CLR1(this->_getDirectAddr(), 6);
		case 0xE2:
			return this->SET1(this->_getDirectAddr(), 7);
		case 0xEE:
			return this->POP(this->_internalRegisters.y);
		case 0xF2:
			return this->CLR1(this->_getDirectAddr(), 7);
		case 0xEA:
			return this->NOT1(this->_getAbsoluteBit());
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
			total += this->_executeInstruction();
		if (this->_state == Running)
			this->_paddingCycles = total - cycles;
	}

	uint24_t APU::_getDirectAddr()
	{
		uint24_t addr = this->_internalRead(this->_internalRegisters.pc++);

		if (this->_internalRegisters.p)
			addr += 0x100;
		return addr;
	}

	uint24_t APU::_getAbsoluteAddr()
	{
		uint24_t addr1 = this->_internalRead(this->_internalRegisters.pc++);
		uint24_t addr2 = this->_internalRead(this->_internalRegisters.pc++);

		return (addr2 << 8u) | addr1;
	}

	std::pair<uint24_t, uint24_t> APU::_getAbsoluteBit()
	{
		uint24_t addr1 = this->_internalRead(this->_internalRegisters.pc++);
		uint24_t addr2 = this->_internalRead(this->_internalRegisters.pc++);

		uint24_t operandA = (addr2 << 8u) | addr1;
		uint24_t operandB = operandA >> 13u;

		operandA = operandA & 0x1FFFu;
		return std::make_pair(operandA, operandB);
	}

	MemoryMap::MemoryMap() :
		Page0(0x00F0),
		Page1(0x0100),
		Memory(0xFDC0),
	    IPL(0x0040)
	{

	}
}