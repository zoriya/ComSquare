//
// Created by Melefo on 27/01/2020.
//

#include "APU.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include "Exceptions/InvalidOpcode.hpp"
#include <cstring>
#include <iostream>
#include <algorithm>

namespace ComSquare::APU
{
	APU::APU(Renderer::IRenderer &renderer)
		: _dsp(renderer, this->_map)
	{
		this->reset();
	}

	std::string APU::getName() const
	{
		return "APU";
	}

	Component APU::getComponent() const
	{
		return Apu;
	}

	uint8_t APU::_internalRead(uint24_t addr) const
	{
		switch (addr) {
		case 0x0000 ... 0x00EF:
			return this->_map.Page0[addr];
		case 0xF0:
			return this->_registers.unknown;
		case 0xF2:
			return this->_registers.dspregAddr;
		case 0xF3:
			return this->_dsp.read(this->_registers.dspregAddr);
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
			return this->_map.Page1[addr - 0x0100];
		case 0x0200 ... 0xFFBF:
			return this->_map.Memory[addr - 0x200];
		case 0xFFC0 ... 0xFFFF:
			return this->_map.IPL[addr - 0xFFC0];
		default:
			throw InvalidAddress("APU Registers read", addr);
		}
	}

	void APU::_internalWrite(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0000 ... 0x00EF:
			this->_map.Page0.write(addr, data);
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
			this->_dsp.write(this->_registers.dspregAddr, data);
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
			this->_map.Page1.write(addr - 0x0100, data);
			break;
		case 0x0200 ... 0xFFBF:
			this->_map.Memory.write(addr - 0x200, data);
			break;
		case 0xFFC0 ... 0xFFFF:
			this->_map.IPL.write(addr - 0xFFC0, data);
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

	uint24_t APU::getSize() const
	{
		return 0x3;
	}

	void APU::reset()
	{
		this->_registers.port0 = 0x00;
		this->_registers.port1 = 0x00;
		this->_registers.port2 = 0x00;
		this->_registers.port3 = 0x00;

		this->_paddingCycles = 0;
		this->_internalRegisters.ya = 0x0000;
		this->_internalRegisters.x = 0x00;
		this->_internalRegisters.sp = 0xEF;
		this->_internalRegisters.pc = 0xFFC0;
	}

	int APU::_executeInstruction()
	{
		uint8_t opcode = this->_getImmediateData();

		switch (opcode) {
		case 0x00:
			return this->NOP();
		case 0x01:
			return this->TCALL(0);
		case 0x02:
			return this->SET1(this->_getDirectAddr(), 0);
		case 0x03: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 0);
		}
		case 0x04:
			return this->ORacc(this->_getDirectAddr(), 3);
		case 0x05:
			return this->ORacc(this->_getAbsoluteAddr(), 4);
		case 0x06:
			return this->ORacc(this->_getIndexXAddr(), 3);
		case 0x07:
			return this->ORacc(this->_getAbsoluteDirectByXAddr(), 6);
		case 0x08:
			return this->ORacc(this->_getImmediateData(), 2);
		case 0x09: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->OR(ope1, ope2, 6);
		}
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
			return this->BPL(this->_getImmediateData());
		case 0x11:
			return this->TCALL(1);
		case 0x12:
			return this->CLR1(this->_getDirectAddr(), 0);
		case 0x13: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->BBC(ope1, ope2, 0);
		}
		case 0x14:
			return this->ORacc(this->_getDirectAddrByX(), 4);
		case 0x15:
			return this->ORacc(this->_getAbsoluteAddrByX(), 5);
		case 0x16:
			return this->ORacc(this->_getAbsoluteAddrByY(), 5);
		case 0x17:
			return this->ORacc(this->_getAbsoluteDirectAddrByY(), 6);
		case 0x18: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->OR(ope1, ope2, 5);
		}
		case 0x19:
			return this->OR(this->_getIndexXAddr(), this->_getIndexYAddr(), 5);
		case 0x1A:
			return this->DECW(this->_getDirectAddr());
		case 0x1B:
			return this->ASL(this->_getDirectAddrByX(), 5);
		case 0x1C:
			return this->ASL(this->_internalRegisters.a, 2, true);
		case 0x1D:
			return this->DECreg(this->_internalRegisters.x);
		case 0x1E:
			return this->CMPreg(this->_internalRegisters.x, this->_getAbsoluteAddr(), 4);
		case 0x1F:
			return this->JMP(this->_getAbsoluteByXAddr(), true);
		case 0x20:
			return this->CLRP();
		case 0x21:
			return this->TCALL(2);
		case 0x22:
			return this->SET1(this->_getDirectAddr(), 1);
		case 0x23: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->BBS(ope1, ope2, 1);
		}
		case 0x24:
			return this->ANDacc(this->_getDirectAddr(), 3);
		case 0x25:
			return this->ANDacc(this->_getAbsoluteAddr(), 4);
		case 0x26:
			return this->ANDacc(this->_getIndexXAddr(), 3);
		case 0x27:
			return this->ANDacc(this->_getAbsoluteDirectByXAddr(), 6);
		case 0x28:
			return this->ANDacc(this->_getImmediateData(), 2);
		case 0x29: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->AND(ope1, ope2, 6);
		}
		case 0x2A:
			return this->OR1(this->_getAbsoluteBit(), true);
		case 0x2B:
			return this->ROL(this->_getDirectAddr(), 4);
		case 0x2C:
			return this->ROL(this->_getAbsoluteAddr(), 5);
		case 0x2D:
			return this->PUSH(this->_internalRegisters.a);
		case 0x2E: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->CBNE(addr, offset);
		}
		case 0x2F:
			return this->BRA(this->_getImmediateData());
		case 0x30:
			return this->BMI(this->_getImmediateData());
		case 0x31:
			return this->TCALL(3);
		case 0x32:
			return this->CLR1(this->_getDirectAddr(), 1);
		case 0x33: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 1);
		}
		case 0x34:
			return this->ANDacc(this->_getDirectAddrByX(), 4);
		case 0x35:
			return this->ANDacc(this->_getAbsoluteAddrByX(), 5);
		case 0x36:
			return this->ANDacc(this->_getAbsoluteAddrByY(), 5);
		case 0x37:
			return this->ANDacc(this->_getAbsoluteDirectAddrByY(), 6);
		case 0x38: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->AND(ope1, ope2, 5);
		}
		case 0x39:
			return this->AND(this->_getIndexXAddr(), this->_getIndexYAddr(), 5);
		case 0x3A:
			return this->INCW(this->_getDirectAddr());
		case 0x3B:
			return this->ROL(this->_getAbsoluteAddrByX(), 5);
		case 0x3C:
			return this->ROL(this->_internalRegisters.a, 2, true);
		case 0x3D:
			return this->INCreg(this->_internalRegisters.x);
		case 0x3E:
			return this->CMPreg(this->_internalRegisters.x, this->_getDirectAddr(), 3);
		case 0x3F:
			return this->CALL(this->_getAbsoluteAddr());
		case 0x40:
			return this->SETP();
		case 0x41:
			return this->TCALL(4);
		case 0x42:
			return this->SET1(this->_getDirectAddr(), 2);
		case 0x43: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 2);
		}
		case 0x44:
			return this->EORacc(this->_getDirectAddr(), 3);
		case 0x45:
			return this->EORacc(this->_getAbsoluteAddr(), 4);
		case 0x46:
			return this->EORacc(this->_getIndexXAddr(), 3);
		case 0x47:
			return this->EORacc(this->_getAbsoluteDirectByXAddr(), 6);
		case 0x48:
			return this->EORacc(this->_getImmediateData(), 2);
		case 0x49: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->EOR(ope1, ope2, 6);
		}
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
			return this->BVC(this->_getImmediateData());
		case 0x51:
			return this->TCALL(5);
		case 0x52:
			return this->CLR1(this->_getDirectAddr(), 2);
		case 0x53: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 2);
		}
		case 0x54:
			return this->EORacc(this->_getDirectAddrByX(), 4);
		case 0x55:
			return this->EORacc(this->_getAbsoluteAddrByX(), 5);
		case 0x56:
			return this->EORacc(this->_getAbsoluteAddrByY(), 5);
		case 0x57:
			return this->EORacc(this->_getAbsoluteDirectAddrByY(), 6);
		case 0x58: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->EOR(ope1, ope2, 5);
		}
		case 0x59:
			return this->EOR(this->_getIndexXAddr(), this->_getIndexYAddr(), 5);
		case 0x5A:
			return this->CMPW(this->_getDirectAddr());
		case 0x5B:
			return this->LSR(this->_getDirectAddrByX(), 5);
		case 0x5C:
			return this->LSR(this->_internalRegisters.a, 2, true);
		case 0x5D:
			return this->MOV(this->_internalRegisters.a, this->_internalRegisters.x);
		case 0x5E:
			return this->CMPreg(this->_internalRegisters.y, this->_getAbsoluteAddr(), 4);
		case 0x5F:
			return this->JMP(this->_getAbsoluteAddr());
		case 0x60:
			return this->CLRC();
		case 0x61:
			return this->TCALL(6);
		case 0x62:
			return this->SET1(this->_getDirectAddr(), 3);
		case 0x63: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 3);
		}
		case 0x64:
			return this->CMPreg(this->_internalRegisters.a, this->_getDirectAddr(), 3);
		case 0x65:
			return this->CMPreg(this->_internalRegisters.a, this->_getAbsoluteAddr(), 4);
		case 0x66:
			return this->CMPreg(this->_internalRegisters.a, this->_getIndexXAddr(), 3);
		case 0x67:
			return this->CMPreg(this->_internalRegisters.a, this->_getAbsoluteDirectByXAddr(), 6);
		case 0x68:
			return this->CMPreg(this->_internalRegisters.a, this->_getImmediateData(), 2);
		case 0x69: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->CMP(ope1, ope2, 6);
		}
		case 0x6A:
			return this->AND1(this->_getAbsoluteBit(), true);
		case 0x6B:
			return this->ROR(this->_getDirectAddr(), 4);
		case 0x6C:
			return this->ROR(this->_getAbsoluteAddr(), 5);
		case 0x6D:
			return this->PUSH(this->_internalRegisters.y);
		case 0x6E:
			return this->DBNZ(this->_getImmediateData(), true);
		case 0x6F:
			return this->RET();
		case 0x70:
			return this->BVS(this->_getImmediateData());
		case 0x71:
			return this->TCALL(7);
		case 0x72:
			return this->CLR1(this->_getDirectAddr(), 3);
		case 0x73: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 3);
		}
		case 0x74:
			return this->CMPreg(this->_internalRegisters.a, this->_getDirectAddrByX(), 4);
		case 0x75:
			return this->CMPreg(this->_internalRegisters.a, this->_getAbsoluteAddrByX(), 5);
		case 0x76:
			return this->CMPreg(this->_internalRegisters.a, this->_getAbsoluteAddrByY(), 5);
		case 0x77:
			return this->CMPreg(this->_internalRegisters.a, this->_getAbsoluteDirectAddrByY(), 6);
		case 0x78: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->CMP(ope1, ope2, 5);
		}
		case 0x79:
			return this->CMP(this->_getIndexXAddr(), this->_getIndexYAddr(), 5);
		case 0x7A:
			return this->ADDW(this->_getDirectAddr());
		case 0x7B:
			return this->ROR(this->_getDirectAddrByX(), 5);
		case 0x7C:
			return this->ROR(this->_internalRegisters.a, 2, true);
		case 0x7D:
			return this->MOV(this->_internalRegisters.x, this->_internalRegisters.a);
		case 0x7E:
			return this->CMPreg(this->_internalRegisters.y, this->_getDirectAddr(), 3);
		case 0x7F:
			return this->RETI();
		case 0x80:
			return this->SETC();
		case 0x81:
			return this->TCALL(8);
		case 0x82:
			return this->SET1(this->_getDirectAddr(), 4);
		case 0x83: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 4);
		}
		case 0x84:
			return this->ADCacc(this->_getDirectAddr(), 3);
		case 0x85:
			return this->ADCacc(this->_getAbsoluteAddr(), 5);
		case 0x86:
			return this->ADCacc(this->_getIndexXAddr(), 3);
		case 0x87:
			return this->ADCacc(this->_getAbsoluteDirectByXAddr(), 6);
		case 0x88:
			return this->ADCacc(this->_getImmediateData(), 2);
		case 0x89: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->ADC(ope1, ope2, 6);
		}
		case 0x8A:
			return this->EOR1(this->_getAbsoluteBit());
		case 0x8B:
			return this->DEC(this->_getDirectAddr(), 4);
		case 0x8C:
			return this->DEC(this->_getAbsoluteAddr(), 5);
		case 0x8D:
			return this->MOV(this->_getImmediateData(), this->_internalRegisters.y, 2);
		case 0x8E:
			return this->POP(this->_internalRegisters.psw);
		case 0x8F: {
			auto to = this->_getDirectAddr();
			auto from = this->_getImmediateData();
			return this->MOV(to, from);
		}
		case 0x90:
			return this->BCC(this->_getImmediateData());
		case 0x91:
			return this->TCALL(9);
		case 0x92:
			return this->CLR1(this->_getDirectAddr(), 4);
		case 0x93: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 4);
		}
		case 0x94:
			return this->ADCacc(this->_getDirectAddrByX(), 4);
		case 0x95:
			return this->ADCacc(this->_getAbsoluteAddrByX(), 5);
		case 0x96:
			return this->ADCacc(this->_getAbsoluteAddrByY(), 5);
		case 0x97:
			return this->ADCacc(this->_getAbsoluteDirectAddrByY(), 6);
		case 0x98: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->ADC(ope1, ope2, 5);
		}
		case 0x99:
			return this->ADC(this->_getIndexXAddr(), this->_getIndexYAddr(), 3);
		case 0x9A:
			return this->SUBW(this->_getDirectAddr());
		case 0x9B:
			return this->DEC(this->_getDirectAddrByX(), 5);
		case 0x9C:
			return this->DECreg(this->_internalRegisters.a);
		case 0x9D:
			return this->MOV(this->_internalRegisters.sp, this->_internalRegisters.x);
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
		case 0xA3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 5);
		}
		case 0xA4:
			return this->SBCacc(this->_getDirectAddr(), 3);
		case 0xA5:
			return this->SBCacc(this->_getAbsoluteAddr(), 4);
		case 0xA6:
			return this->SBCacc(this->_getIndexXAddr(), 3);
		case 0xA7:
			return this->SBCacc(this->_getAbsoluteDirectByXAddr(), 6);
		case 0xA8:
			return this->SBCacc(this->_getImmediateData(), 2);
		case 0xA9: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getDirectAddr();
			return this->SBC(ope1, ope2, 6);
		}
		case 0xAA:
			return this->MOV1(this->_getAbsoluteBit(), true);
		case 0xAB:
			return this->INC(this->_getDirectAddr(), 4);
		case 0xAC:
			return this->INC(this->_getAbsoluteAddr(), 5);
		case 0xAD:
			return this->CMPreg(this->_internalRegisters.y, this->_getImmediateData(), 2);
		case 0xAE:
			return this->POP(this->_internalRegisters.a);
		case 0xAF:
			return this->MOV(this->_internalRegisters.a, this->_getIndexXAddr(), 4, true);
		case 0xB0:
			return this->BCS(this->_getImmediateData());
		case 0xB1:
			return this->TCALL(11);
		case 0xB2:
			return this->CLR1(this->_getDirectAddr(), 5);
		case 0xB3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 5);
		}
		case 0xB4:
			return this->SBCacc(this->_getDirectAddrByX(), 4);
		case 0xB5:
			return this->SBCacc(this->_getAbsoluteAddrByX(), 5);
		case 0xB6:
			return this->SBCacc(this->_getAbsoluteAddrByY(), 5);
		case 0xB7:
			return this->SBCacc(this->_getAbsoluteDirectAddrByY(), 6);
		case 0xB8: {
			auto ope1 = this->_getDirectAddr();
			auto ope2 = this->_getImmediateData();
			return this->SBC(ope1, ope2, 5);
		}
		case 0xB9:
			return this->SBC(this->_getIndexXAddr(), this->_getIndexYAddr(), 5);
		case 0xBA:
			return this->MOVW(this->_getDirectAddr(), true);
		case 0xBB:
			return this->INC(this->_getDirectAddrByX(), 5);
		case 0xBC:
			return this->INCreg(this->_internalRegisters.a);
		case 0xBD:
			return this->MOV(this->_internalRegisters.x, this->_internalRegisters.sp, false);
		case 0xBE:
			return this->DAS();
		case 0xBF:
			return this->MOV(this->_getIndexXAddr(), this->_internalRegisters.a, 4, true);
		case 0xC0:
			return this->DI();
		case 0xC1:
			return this->TCALL(12);
		case 0xC2:
			return this->SET1(this->_getDirectAddr(), 6);
		case 0xC3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 6);
		}
		case 0xC4:
			return this->MOV(this->_internalRegisters.a, this->_getDirectAddr(), 4);
		case 0xC5:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteAddr());
		case 0xC6:
			return this->MOV(this->_internalRegisters.a, this->_getIndexXAddr(), 4);
		case 0xC7:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteDirectByXAddr(), 7);
		case 0xC8:
			return this->CMPreg(this->_internalRegisters.x, this->_getImmediateData(), 2);
		case 0xC9:
			return this->MOV(this->_internalRegisters.x, this->_getAbsoluteAddr(), 5);
		case 0xCA:
			return this->MOV1(this->_getAbsoluteBit());
		case 0xCB:
			return this->MOV(this->_internalRegisters.y, this->_getDirectAddr(), 4);
		case 0xCC:
			return this->MOV(this->_internalRegisters.y, this->_getAbsoluteAddr(), 5);
		case 0xCD:
			return this->MOV(this->_getImmediateData(), this->_internalRegisters.x, 2);
		case 0xCE:
			return this->POP(this->_internalRegisters.x);
		case 0xCF:
			return this->MUL();
		case 0xD0:
			return this->BNE(this->_getImmediateData());
		case 0xD1:
			return this->TCALL(13);
		case 0xD2:
			return this->CLR1(this->_getDirectAddr(), 6);
		case 0xD3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 6);
		}
		case 0xD4:
			return this->MOV(this->_internalRegisters.a, this->_getDirectAddrByX(), 5);
		case 0xD5:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteAddrByX(), 6);
		case 0xD6:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteAddrByY(), 6);
		case 0xD7:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteDirectAddrByY(), 7);
		case 0xD8:
			return this->MOV(this->_internalRegisters.x, this->_getDirectAddr(), 4);
		case 0xD9:
			return this->MOV(this->_internalRegisters.x, this->_getDirectAddrByY(), 5);
		case 0xDA:
			return this->MOVW(this->_getDirectAddr());
		case 0xDB:
			return this->MOV(this->_internalRegisters.y, this->_getDirectAddrByX(), 5);
		case 0xDC:
			return this->DECreg(this->_internalRegisters.y);
		case 0xDD:
			return this->MOV(this->_internalRegisters.y, this->_internalRegisters.a);
		case 0xDE: {
			auto addr = this->_getDirectAddrByX();
			auto offset = this->_getImmediateData();
			return this->CBNE(addr, offset, true);
		}
		case 0xDF:
			return this->DAA();
		case 0xE0:
			return this->CLRV();
		case 0xE1:
			return this->TCALL(14);
		case 0xE2:
			return this->SET1(this->_getDirectAddr(), 7);
		case 0xE3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBS(addr, offset, 7);
		}
		case 0xE4:
			return this->MOV(this->_internalRegisters.a, this->_internalRead(this->_getDirectAddr()), 3);
		case 0xE5:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteAddrByX(), 5);
		case 0xE6:
			return this->MOV(this->_internalRegisters.a, this->_getIndexXAddr(), 3);
		case 0xE7:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteDirectByXAddr(), 6);
		case 0xE8:
			return this->MOV(this->_internalRegisters.a, this->_getImmediateData(), 2);
		case 0xE9:
			return this->MOV(this->_internalRegisters.x, this->_getAbsoluteAddr(), 4);
		case 0xEA:
			return this->NOT1(this->_getAbsoluteBit());
		case 0xEB:
			return this->MOV(this->_internalRegisters.y, this->_getDirectAddr(), 3);
		case 0xEC:
			return this->MOV(this->_internalRegisters.y, this->_getAbsoluteAddr(), 4);
		case 0xED:
			return this->NOTC();
		case 0xEE:
			return this->POP(this->_internalRegisters.y);
		case 0xEF:
			return this->SLEEP();
		case 0xF0:
			return BEQ(this->_getImmediateData());
		case 0xF1:
			return this->TCALL(15);
		case 0xF2:
			return this->CLR1(this->_getDirectAddr(), 7);
		case 0xF3: {
			auto addr = this->_getDirectAddr();
			auto offset = this->_getImmediateData();
			return this->BBC(addr, offset, 7);
		}
		case 0xF4:
			return this->MOV(this->_internalRegisters.a, this->_internalRead(this->_getDirectAddrByX()), 4);
		case 0xF5:
			return this->MOV(this->_internalRegisters.a, this->_internalRead(this->_getAbsoluteAddrByX()), 5);
		case 0xF6:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteAddrByY(), 5);
		case 0xF7:
			return this->MOV(this->_internalRegisters.a, this->_getAbsoluteDirectAddrByY(), 6);
		case 0xF8:
			return this->MOV(this->_internalRegisters.x, this->_getDirectAddr(), 3);
		case 0xF9:
			return this->MOV(this->_internalRegisters.x, this->_getDirectAddrByY(), 4);
		case 0xFA:
			return this->MOV(this->_getDirectAddr(), this->_getDirectAddr());
		case 0xFB:
			return this->MOV(this->_internalRegisters.y, this->_getDirectAddrByX(), 4);
		case 0xFC:
			return this->INCreg(this->_internalRegisters.y);
		case 0xFD:
			return this->MOV(this->_internalRegisters.y, this->_internalRegisters.a);
		case 0xFE:
			return this->DBNZ(this->_getImmediateData());
		case 0xFF:
			return this->STOP();
		default:
			throw InvalidOpcode("APU", opcode);
		}
	}

	void APU::update(unsigned cycles)
	{
		if (this->isDisabled)
			return;

		unsigned total = 0;

		if (this->_paddingCycles > cycles) {
			this->_paddingCycles -= cycles;
			return;
		}
		cycles -= this->_paddingCycles;
		while (total < cycles && this->_state == Running)
			total += this->_executeInstruction();
		if (this->_state == Running)
			this->_paddingCycles = total - cycles;

		this->_dsp.update();
	}

	void APU::loadFromSPC(Cartridge::Cartridge &cartridge)
	{
		std::span<const uint8_t> data = cartridge.getData();
		uint24_t size = cartridge.getSize();
		if (size < 0x101C0)
			throw InvalidAddress("Cartridge is not the right size", size);

		std::string song = std::string(reinterpret_cast<const char *>(data.data() + 0x2E), 0x20);
		std::string game = std::string(reinterpret_cast<const char *>(data.data() + 0x4E), 0x20);
		std::string dumper = std::string(reinterpret_cast<const char *>(data.data() + 0x6E), 0x10);
		std::string comment = std::string(reinterpret_cast<const char *>(data.data() + 0x7E), 0x20);
		std::string date = std::string(reinterpret_cast<const char *>(data.data() + 0x9E), 0x0B);
		std::string artist = std::string(reinterpret_cast<const char *>(data.data() + 0xB1), 0x20);

		this->_internalRegisters.pcl = cartridge.read(0x25);
		this->_internalRegisters.pch = cartridge.read(0x26);
		this->_internalRegisters.a = cartridge.read(0x27);
		this->_internalRegisters.x = cartridge.read(0x28);
		this->_internalRegisters.y = cartridge.read(0x29);
		this->_internalRegisters.psw = cartridge.read(0x2A);
		this->_internalRegisters.sp = cartridge.read(0x2B);

		std::copy_n(data.begin() + 0x100, this->_map.Page0.getSize(), this->_map.Page0.getData().begin());
		std::copy_n(data.begin() + 0x200, this->_map.Page1.getSize(), this->_map.Page1.getData().begin());
		std::copy_n(data.begin() + 0x300, this->_map.Memory.getSize(), this->_map.Memory.getData().begin());

		this->_registers.unknown = cartridge.read(0x100 + 0xF0);
		this->_registers.ctrlreg = cartridge.read(0x100 + 0xF1);
		this->_registers.dspregAddr = cartridge.read(0x100 + 0xF2);
		this->_dsp.write(this->_registers.dspregAddr, cartridge.read(0x100 + 0xF3));
		this->_registers.port0 = cartridge.read(0x100 + 0xF4);
		this->_registers.port1 = cartridge.read(0x100 + 0xF5);
		this->_registers.port2 = cartridge.read(0x100 + 0xF6);
		this->_registers.port3 = cartridge.read(0x100 + 0xF7);
		this->_registers.regmem1 = cartridge.read(0x100 + 0xF8);
		this->_registers.regmem2 = cartridge.read(0x100 + 0xF9);
		this->_registers.timer0 = cartridge.read(0x100 + 0xFA);
		this->_registers.timer1 = cartridge.read(0x100 + 0xFB);
		this->_registers.timer2 = cartridge.read(0x100 + 0xFC);
		this->_registers.counter0 = cartridge.read(0x100 + 0xFD);
		this->_registers.counter1 = cartridge.read(0x100 + 0xFE);
		this->_registers.counter2 = cartridge.read(0x100 + 0xFF);

		for (int i = 0x00; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x01; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x02; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x03; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x04; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x05; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x06; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x07; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x08; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x09; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x0C; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x0D; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
		for (int i = 0x0F; i < 0x80; i += 0x10)
			this->_dsp.write(i, cartridge.read(0x10100 + i));
	}

	void APU::_setNZflags(uint8_t value)
	{
		this->_internalRegisters.n = value & 0x80u;
		this->_internalRegisters.z = !value;
	}

	MemoryMap::MemoryMap()
		: Page0(0x00F0, Apu, "APU's Page 0"),
		  Page1(0x0100, Apu, "APU's Page 1"),
		  Memory(0xFDC0, Apu, "APU's Ram"),
		  IPL(Apu, "IPL Rom")
	{}
}// namespace ComSquare::APU