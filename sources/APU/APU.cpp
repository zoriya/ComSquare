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
		case 0x01:
			return this->TCALL(0);
		case 0x02:
			return this->SET1(this->_getDirectAddr(), 0);
		case 0x03:
			return this->BBS(this->_getDirectAddr(), 0);
		case 0x0A:
			return this->OR1(this->_getAbsoluteBit());
		case 0x0B:
			return this->ASL(this->_getDirectAddr(), 4);
		case 0x0C:
			return this->ASL(this->_getAbsoluteAddr(), 5);
		case 0x0D:
			return this->PUSH(this->_internalRegisters.psw);
		case 0x0E:
			return this->TSET1(this->_getAbsoluteAddr());
		case 0x0F:
			return this->BRK();
		case 0x10:
			return this->BPL();
		case 0x11:
			return this->TCALL(1);
		case 0x12:
			return this->CLR1(this->_getDirectAddr(), 0);
		case 0x13:
			return this->BBC(this->_getDirectAddr(), 0);
		case 0x1A:
			return this->DECW(this->_getDirectAddr());
		case 0x1B:
			return this->ASL(this->_getDirectAddrByX(), 5);
		case 0x1C:
			return this->ASL(this->_internalRegisters.a, 2, true);
		case 0x1D:
			return this->DECreg(this->_internalRegisters.x);
		case 0x1F:
			return this->JMP(this->_getAbsoluteAddrByX(), true);
		case 0x20:
			return this->CLRP();
		case 0x21:
			return this->TCALL(2);
		case 0x22:
			return this->SET1(this->_getDirectAddr(), 1);
		case 0x23:
			return this->BBS(this->_getDirectAddr(), 1);
		case 0x2A:
			return this->OR1(this->_getAbsoluteBit(), true);
		case 0x2B:
			return this->ROL(this->_getDirectAddr(), 4);
		case 0x2C:
			return this->ROL(this->_getAbsoluteAddr(), 5);
		case 0x2D:
			return this->PUSH(this->_internalRegisters.a);
		case 0x2E:
			return this->CBNE(this->_getDirectValue());
		case 0x2F:
			return this->BRA();
		case 0x30:
			return this->BMI();
		case 0x31:
			return this->TCALL(3);
		case 0x32:
			return this->CLR1(this->_getDirectAddr(), 1);
		case 0x33:
			return this->BBC(this->_getDirectAddr(), 1);
		case 0x3A:
			return this->INCW(this->_getDirectAddr());
		case 0x3B:
			return this->ROL(this->_getAbsoluteAddrByX(), 5);
		case 0x3C:
			return this->ROL(this->_internalRegisters.a, 2, true);
		case 0x3D:
			return this->INCreg(this->_internalRegisters.x);
		case 0x3F:
			return this->CALL(this->_getAbsoluteAddr());
		case 0x40:
			return this->SETP();
		case 0x41:
			return this->TCALL(4);
		case 0x42:
			return this->SET1(this->_getDirectAddr(), 2);
		case 0x43:
			return this->BBS(this->_getDirectAddr(), 2);
		case 0x4A:
			return this->AND1(this->_getAbsoluteBit());
		case 0x4B:
			return this->LSR(this->_getDirectAddr(), 4);
		case 0x4C:
			return this->LSR(this->_getAbsoluteAddr(), 5);
		case 0x4D:
			return this->PUSH(this->_internalRegisters.x);
		case 0x4E:
			return this->TCLR1(this->_getAbsoluteAddr());
		case 0x4F:
			return this->PCALL();
		case 0x50:
			return this->BVC();
		case 0x51:
			return this->TCALL(5);
		case 0x52:
			return this->CLR1(this->_getDirectAddr(), 2);
		case 0x53:
			return this->BBC(this->_getDirectAddr(), 2);
		case 0x5A:
			return this->CMPW(this->_getDirectAddr());
		case 0x5B:
			return this->LSR(this->_getDirectAddrByX(), 5);
		case 0x5C:
			return this->LSR(this->_internalRegisters.a, 2, true);
		case 0x5F:
			return this->JMP(this->_getAbsoluteAddr());
		case 0x60:
			return this->CLRC();
		case 0x61:
			return this->TCALL(6);
		case 0x62:
			return this->SET1(this->_getDirectAddr(), 3);
		case 0x63:
			return this->BBS(this->_getDirectAddr(), 3);
		case 0x6A:
			return this->AND1(this->_getAbsoluteBit(), true);
		case 0x6B:
			return this->ROR(this->_getDirectAddr(), 4);
		case 0x6C:
			return this->ROR(this->_getAbsoluteAddr(), 5);
		case 0x6D:
			return this->PUSH(this->_internalRegisters.y);
		case 0x6E:
			return this->DBNZ(true);
		case 0x6F:
			return this->RET();
		case 0x70:
			return this->BVS();
		case 0x71:
			return this->TCALL(7);
		case 0x72:
			return this->CLR1(this->_getDirectAddr(), 3);
		case 0x73:
			return this->BBC(this->_getDirectAddr(), 3);
		case 0x7A:
			return this->ADDW(this->_getDirectAddr());
		case 0x7B:
			return this->ROR(this->_getDirectAddrByX(), 5);
		case 0x7C:
			return this->ROR(this->_internalRegisters.a, 2, true);
		case 0x7F:
			return this->RETI();
		case 0x80:
			return this->SETC();
		case 0x81:
			return this->TCALL(8);
		case 0x82:
			return this->SET1(this->_getDirectAddr(), 4);
		case 0x83:
			return this->BBS(this->_getDirectAddr(), 4);
		case 0x8A:
			return this->EOR1(this->_getAbsoluteBit());
		case 0x8B:
			return this->DEC(this->_getDirectAddr(), 4);
		case 0x8C:
			return this->DEC(this->_getAbsoluteAddr(), 5);
		case 0x8E:
			return this->POP(this->_internalRegisters.psw);
		case 0x90:
			return this->BCC();
		case 0x91:
			return this->TCALL(9);
		case 0x92:
			return this->CLR1(this->_getDirectAddr(), 4);
		case 0x93:
			return this->BBC(this->_getDirectAddr(), 4);
		case 0x9A:
			return this->SUBW(this->_getDirectAddr());
		case 0x9B:
			return this->DEC(this->_getDirectAddrByX(), 5);
		case 0x9C:
			return this->DECreg(this->_internalRegisters.a);
		case 0x9E:
			return this->DIV();
		case 0x9F:
			return this->XCN();
		case 0xA0:
			return this->EI();
		case 0xA1:
			return this->TCALL(10);
		case 0xA2:
			return this->SET1(this->_getDirectAddr(), 5);
		case 0xA3:
			return this->BBS(this->_getDirectAddr(), 5);
		case 0xAA:
			return this->MOV1(this->_getAbsoluteBit(), true);
		case 0xAB:
			return this->INC(this->_getDirectAddr(), 4);
		case 0xAC:
			return this->INC(this->_getAbsoluteAddr(), 5);
		case 0xAE:
			return this->POP(this->_internalRegisters.a);
		case 0xB0:
			return this->BCS();
		case 0xB1:
			return this->TCALL(11);
		case 0xB2:
			return this->CLR1(this->_getDirectAddr(), 5);
		case 0xB3:
			return this->BBC(this->_getDirectAddr(), 5);
		case 0xBA:
			return this->MOVW(this->_getDirectAddr(), true);
		case 0xBB:
			return this->INC(this->_getDirectAddrByX(), 5);
		case 0xBC:
			return this->INCreg(this->_internalRegisters.a);
		case 0xBE:
			return this->DAS();
		case 0xC0:
			return this->DI();
		case 0xC1:
			return this->TCALL(12);
		case 0xC2:
			return this->SET1(this->_getDirectAddr(), 6);
		case 0xC3:
			return this->BBS(this->_getDirectAddr(), 6);
		case 0xCA:
			return this->MOV1(this->_getAbsoluteBit());
		case 0xCE:
			return this->POP(this->_internalRegisters.x);
		case 0xCF:
			return this->MUL();
		case 0xD0:
			return this->BNE();
		case 0xD1:
			return this->TCALL(13);
		case 0xD2:
			return this->CLR1(this->_getDirectAddr(), 6);
		case 0xD3:
			return this->BBC(this->_getDirectAddr(), 6);
		case 0xDA:
			return this->MOVW(this->_getDirectAddr());
		case 0xDC:
			return this->DECreg(this->_internalRegisters.y);
		case 0xDE:
			return this->CBNE(this->_getDirectAddrByX(), true);
		case 0xDF:
			return this->DAA();
		case 0xE1:
			return this->TCALL(14);
		case 0xE2:
			return this->SET1(this->_getDirectAddr(), 7);
		case 0xE3:
			return this->BBS(this->_getDirectAddr(), 7);
		case 0xEE:
			return this->POP(this->_internalRegisters.y);
		case 0xF0:
			return BEQ();
		case 0xF1:
			return this->TCALL(15);
		case 0xF2:
			return this->CLR1(this->_getDirectAddr(), 7);
		case 0xF3:
			return this->BBC(this->_getDirectAddr(), 7);
		case 0xFC:
			return this->INCreg(this->_internalRegisters.y);
		case 0xEA:
			return this->NOT1(this->_getAbsoluteBit());
		case 0xED:
			return this->NOTC();
		case 0xEF:
			return this->SLEEP();
		case 0xFE:
			return this->DBNZ();
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

	void APU::_setNZflags(uint8_t value)
	{
		this->_internalRegisters.n = value & 0x80u;
		this->_internalRegisters.z = !value;
	}

	MemoryMap::MemoryMap() :
		Page0(0x00F0),
		Page1(0x0100),
		Memory(0xFDC0),
	    IPL(0x0040)
	{ }
}
