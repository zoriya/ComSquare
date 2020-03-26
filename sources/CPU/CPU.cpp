//
// Created by anonymus-raccoon on 1/24/20.
//

#include "CPU.hpp"

#include <utility>
#include <iostream>
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

namespace ComSquare::CPU
{
	CPU::CPU(std::shared_ptr<Memory::MemoryBus> bus, Cartridge::Header &cartridgeHeader)
		: _bus(std::move(bus)), _cartridgeHeader(cartridgeHeader)
	{
		this->RESB();
	}

	bool CPU::isDebugger()
	{
		return false;
	}

	void CPU::setMemoryBus(std::shared_ptr<Memory::MemoryBus> bus)
	{
		this->_bus = std::move(bus);
	}

	//! @bref The CPU's internal registers starts at $4200	and finish at $421F.
	uint8_t CPU::read(uint24_t addr)
	{
		switch (addr) {
		case 0x0:
			return this->_internalRegisters.nmitimen;
		case 0x1:
			return this->_internalRegisters.wrio;
		case 0x2:
			return this->_internalRegisters.wrmpya;
		case 0x3:
			return this->_internalRegisters.wrmpyb;
		case 0x4:
			return this->_internalRegisters.wrdivl;
		case 0x5:
			return this->_internalRegisters.wrdivh;
		case 0x6:
			return this->_internalRegisters.wrdivb;
		case 0x7:
			return this->_internalRegisters.htimel;
		case 0x8:
			return this->_internalRegisters.htimeh;
		case 0x9:
			return this->_internalRegisters.vtimel;
		case 0xA:
			return this->_internalRegisters.vtimeh;
		case 0xB:
			return this->_internalRegisters.mdmaen;
		case 0xC:
			return this->_internalRegisters.hdmaen;
		case 0xD:
			return this->_internalRegisters.memsel;
		case 0x10:
			return this->_internalRegisters.rdnmi;
		case 0x11:
			return this->_internalRegisters.timeup;
		case 0x12:
			return this->_internalRegisters.hvbjoy;
		case 0x13:
			return this->_internalRegisters.rdio;
		case 0x14:
			return this->_internalRegisters.rddivl;
		case 0x15:
			return this->_internalRegisters.rddivh;
		case 0x16:
			return this->_internalRegisters.rdmpyl;
		case 0x17:
			return this->_internalRegisters.rdmpyh;
		case 0x18:
			return this->_internalRegisters.joy1l;
		case 0x19:
			return this->_internalRegisters.joy1h;
		case 0x1A:
			return this->_internalRegisters.joy2l;
		case 0x1B:
			return this->_internalRegisters.joy2h;
		case 0x1C:
			return this->_internalRegisters.joy3l;
		case 0x1D:
			return this->_internalRegisters.joy3h;
		case 0x1E:
			return this->_internalRegisters.joy4l;
		case 0x1F:
			return this->_internalRegisters.joy4h;
		default:
			throw InvalidAddress("CPU Internal Registers read", addr);
		}
	}

	void CPU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0:
			this->_internalRegisters.nmitimen = data;
			break;
		case 0x1:
			this->_internalRegisters.wrio = data;
			break;
		case 0x2:
			this->_internalRegisters.wrmpya = data;
			break;
		case 0x3:
			this->_internalRegisters.wrmpyb = data;
			break;
		case 0x4:
			this->_internalRegisters.wrdivl = data;
			break;
		case 0x5:
			this->_internalRegisters.wrdivh = data;
			break;
		case 0x6:
			this->_internalRegisters.wrdivb = data;
			break;
		case 0x7:
			this->_internalRegisters.htimel = data;
			break;
		case 0x8:
			this->_internalRegisters.htimeh = data;
			break;
		case 0x9:
			this->_internalRegisters.vtimel = data;
			break;
		case 0xA:
			this->_internalRegisters.vtimeh = data;
			break;
		case 0xB:
			this->_internalRegisters.mdmaen = data;
			break;
		case 0xC:
			this->_internalRegisters.hdmaen = data;
			break;
		case 0xD:
			this->_internalRegisters.memsel = data;
			break;
		case 0x10:
			this->_internalRegisters.rdnmi = data;
			break;
		case 0x11:
			this->_internalRegisters.timeup = data;
			break;
		case 0x12:
			this->_internalRegisters.hvbjoy = data;
			break;
		case 0x13:
			this->_internalRegisters.rdio = data;
			break;
		case 0x14:
			this->_internalRegisters.rddivl = data;
			break;
		case 0x15:
			this->_internalRegisters.rddivh = data;
			break;
		case 0x16:
			this->_internalRegisters.rdmpyl = data;
			break;
		case 0x17:
			this->_internalRegisters.rdmpyh = data;
			break;
		case 0x18:
			this->_internalRegisters.joy1l = data;
			break;
		case 0x19:
			this->_internalRegisters.joy1h = data;
			break;
		case 0x1A:
			this->_internalRegisters.joy2l = data;
			break;
		case 0x1B:
			this->_internalRegisters.joy2h = data;
			break;
		case 0x1C:
			this->_internalRegisters.joy3l = data;
			break;
		case 0x1D:
			this->_internalRegisters.joy3h = data;
			break;
		case 0x1E:
			this->_internalRegisters.joy4l = data;
			break;
		case 0x1F:
			this->_internalRegisters.joy4h = data;
			break;
		default:
			throw InvalidAddress("CPU Internal Registers write", addr);
		}
	}

	unsigned CPU::update()
	{
		unsigned cycles = 0;

		for (int i = 0; i < 0xFF; i++)
			cycles += this->_executeInstruction(this->_bus->read(this->_registers.pac++));
		return cycles;
	}

	unsigned CPU::_executeInstruction(uint8_t opcode)
	{
		Instruction instruction = this->_instructions[opcode];

		this->_hasIndexCrossedPageBoundary = false;

		uint24_t valueAddr = 0;
		int cycles = instruction.cycleCount + !this->_registers.p.m;

		switch (instruction.addressingMode) {
		case Implied:
			break;
		case Immediate8bits:
			valueAddr = this->_getImmediateAddr8Bits();
			break;
		case ImmediateForA:
			valueAddr = this->_getImmediateAddrForA();
			break;
		case ImmediateForX:
			valueAddr = this->_getImmediateAddrForX();
			break;

			// TODO implement the relative addressing mode
			// TODO implement the relative long addressing mode

		case Absolute:
			valueAddr = this->_getAbsoluteAddr();
			break;
		case AbsoluteLong:
			valueAddr = this->_getAbsoluteLongAddr();
			break;
		case AbsoluteIndirect:
			valueAddr = this->_getAbsoluteIndirectAddr();
			break;

			//TODO implement absolute indirect long addressing mode

		case DirectPage:
			valueAddr = this->_getDirectAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndirect:
			valueAddr = this->_getDirectIndirectAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndirectLong:
			valueAddr = this->_getDirectIndirectLongAddr();
			cycles += this->_registers.dl != 0;
			break;

		case DirectPageIndexedByX:
			valueAddr = this->_getDirectIndexedByXAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndexedByY:
			valueAddr = this->_getDirectIndexedByYAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndirectIndexedByX:
			valueAddr = this->_getDirectIndirectIndexedXAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndirectIndexedByY:
			valueAddr = this->_getDirectIndirectIndexedYAddr();
			cycles += this->_registers.dl != 0;
			break;
		case DirectPageIndirectIndexedByYLong:
			valueAddr = this->_getDirectIndirectIndexedYLongAddr();
			cycles += this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
			break;

		case AbsoluteIndexedByX:
			valueAddr = this->_getAbsoluteIndexedByXAddr();
			cycles += this->_hasIndexCrossedPageBoundary;
			break;
		case AbsoluteIndexedByXLong:
			valueAddr = this->_getAbsoluteIndexedByXLongAddr();
			break;
		case AbsoluteIndexedByY:
			valueAddr = this->_getAbsoluteIndexedByYAddr();
			cycles += this->_hasIndexCrossedPageBoundary;
			break;

		case StackRelative:
			valueAddr = this->_getStackRelativeAddr();
			break;
		case StackRelativeIndirectIndexedByY:
			valueAddr = this->_getStackRelativeIndirectIndexedYAddr();
			break;

		case ProgramCounterRelative:
			valueAddr = this->_getProgramCounterRelativeAddr();
			break;
		case ProgramCounterRelativeLong:
			valueAddr = this->_getProgramCounterRelativeLongAddr();
			break;
		case AbsoluteIndirectIndexedByX:
			valueAddr = this->_getAbsoluteIndirectIndexedByXAddr();
			break;
		default:
			break;
		}

		return cycles + (this->*instruction.call)(valueAddr);
//		uint24_t addr;
//
//		switch (opcode) {
//		case Instructions::BRK:      this->BRK(0x0); return 7 + !this->_isEmulationMode;
//
//		case Instructions::COP:      this->COP(); return 7 + !this->_isEmulationMode;
//
//		case Instructions::RTI:	     this->RTI(); return 6 + !this->_isEmulationMode;
//
//		case Instructions::ADC_IM:   this->ADC(this->_getImmediateAddrForA()); 					return 2 + !this->_registers.p.m;
//		case Instructions::ADC_ABS:  this->ADC(this->_getAbsoluteAddr()); 						return 4 + !this->_registers.p.m;
//		case Instructions::ADC_ABSl: this->ADC(this->_getAbsoluteLongAddr()); 					return 5 + !this->_registers.p.m;
//		case Instructions::ADC_DP:   this->ADC(this->_getDirectAddr()); 							return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_DPi:  this->ADC(this->_getDirectIndirectAddr()); 					return 5 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_DPil: this->ADC(this->_getDirectIndirectLongAddr()); 				return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_ABSX: this->ADC(this->_getAbsoluteIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::ADC_ABSXl:this->ADC(this->_getAbsoluteIndexedByXLongAddr()); 			return 5 + !this->_registers.p.m;
//		case Instructions::ADC_ABSY: this->ADC(this->_getAbsoluteIndexedByYAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::ADC_DPX:  this->ADC(this->_getDirectIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_DPXi: this->ADC(this->_getDirectIndirectIndexedXAddr()); 			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_DPYi: this->ADC(this->_getDirectIndirectIndexedYAddr()); 			return 5 + !this->_registers.p.m + this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
//		case Instructions::ADC_DPYil:this->ADC(this->_getDirectIndirectIndexedYLongAddr()); 		return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::ADC_SR:   this->ADC(this->_getStackRelativeAddr()); 					return 4 + !this->_registers.p.m;
//		case Instructions::ADC_SRYi: this->ADC(this->_getStackRelativeIndirectIndexedYAddr()); 	return 7 + !this->_registers.p.m;
//
//		case Instructions::STA_ABS:  this->STA(this->_getAbsoluteAddr()); 						return 4 + !this->_registers.p.m;
//		case Instructions::STA_ABSl: this->STA(this->_getAbsoluteLongAddr()); 					return 5 + !this->_registers.p.m;
//		case Instructions::STA_DP:   this->STA(this->_getDirectAddr()); 							return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_DPi:  this->STA(this->_getDirectIndirectAddr()); 					return 5 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_DPil: this->STA(this->_getDirectIndirectLongAddr()); 				return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_ABSX: this->STA(this->_getAbsoluteIndexedByXAddr()); 				return 5 + !this->_registers.p.m;
//		case Instructions::STA_ABSXl:this->STA(this->_getAbsoluteIndexedByXLongAddr()); 			return 5 + !this->_registers.p.m;
//		case Instructions::STA_ABSY: this->STA(this->_getAbsoluteIndexedByYAddr()); 				return 5 + !this->_registers.p.m;
//		case Instructions::STA_DPX:  this->STA(this->_getDirectIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_DPXi: this->STA(this->_getDirectIndirectIndexedXAddr());			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_DPYi: this->STA(this->_getDirectIndirectIndexedYAddr()); 			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_DPYil:this->STA(this->_getDirectIndirectIndexedYLongAddr()); 		return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STA_SR:   this->STA(this->_getStackRelativeAddr()); 					return 4 + !this->_registers.p.m;
//		case Instructions::STA_SRYi: this->STA(this->_getStackRelativeIndirectIndexedYAddr()); 	return 7 + !this->_registers.p.m;
//
//		case Instructions::STX_ABS:  this->STX(this->_getAbsoluteAddr()); 		return 4 + !this->_registers.p.m;
//		case Instructions::STX_DP:   this->STX(this->_getDirectAddr()); 			return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STX_DPY:  this->STX(this->_getDirectIndexedByYAddr());	return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::STY_ABS:  this->STX(this->_getAbsoluteAddr()); 		return 4 + !this->_registers.p.m;
//		case Instructions::STY_DP:   this->STX(this->_getDirectAddr()); 			return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STY_DPX:  this->STX(this->_getDirectIndexedByXAddr());	return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::STZ_ABS:  this->STX(this->_getAbsoluteAddr()); 			return 4 + !this->_registers.p.m;
//		case Instructions::STZ_DP:   this->STX(this->_getDirectAddr()); 				return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STZ_ABSX: this->STX(this->_getAbsoluteIndexedByXAddr()); 	return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::STZ_DPX:  this->STX(this->_getDirectIndexedByXAddr());		return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::LDA_IM:   this->LDA(this->_getImmediateAddrForA()); 					return 2 + !this->_registers.p.m;
//		case Instructions::LDA_ABS:  this->LDA(this->_getAbsoluteAddr()); 						return 4 + !this->_registers.p.m;
//		case Instructions::LDA_ABSl: this->LDA(this->_getAbsoluteLongAddr()); 					return 5 + !this->_registers.p.m;
//		case Instructions::LDA_DP:   this->LDA(this->_getDirectAddr()); 							return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_DPi:  this->LDA(this->_getDirectIndirectAddr()); 					return 5 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_DPil: this->LDA(this->_getDirectIndirectLongAddr()); 				return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_ABSX: this->LDA(this->_getAbsoluteIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::LDA_ABSXl:this->LDA(this->_getAbsoluteIndexedByXLongAddr()); 			return 5 + !this->_registers.p.m;
//		case Instructions::LDA_ABSY: this->LDA(this->_getAbsoluteIndexedByYAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::LDA_DPX:  this->LDA(this->_getDirectIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_DPXi: this->LDA(this->_getDirectIndirectIndexedXAddr());			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_DPYi: this->LDA(this->_getDirectIndirectIndexedYAddr()); 			return 5 + !this->_registers.p.m + this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
//		case Instructions::LDA_DPYil:this->LDA(this->_getDirectIndirectIndexedYLongAddr()); 		return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDA_SR:   this->LDA(this->_getStackRelativeAddr()); 					return 4 + !this->_registers.p.m;
//		case Instructions::LDA_SRYi: this->LDA(this->_getStackRelativeIndirectIndexedYAddr()); 	return 7 + !this->_registers.p.m;
//
//		case Instructions::LDX_IM:   this->LDX(this->_getImmediateAddrForX()); 		return 2 + !this->_registers.p.m;
//		case Instructions::LDX_ABS:  this->LDX(this->_getAbsoluteAddr()); 			return 4 + !this->_registers.p.m;
//		case Instructions::LDX_DP:   this->LDX(this->_getDirectAddr()); 				return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDX_ABSY: this->LDX(this->_getAbsoluteIndexedByYAddr()); 	return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::LDX_DPY:  this->LDX(this->_getDirectIndexedByYAddr()); 	return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::LDY_IM:   this->LDY(this->_getImmediateAddrForX()); 		return 2 + !this->_registers.p.m;
//		case Instructions::LDY_ABS:  this->LDY(this->_getAbsoluteAddr()); 			return 4 + !this->_registers.p.m;
//		case Instructions::LDY_DP:   this->LDY(this->_getDirectAddr()); 				return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::LDY_ABSY: this->LDY(this->_getAbsoluteIndexedByYAddr()); 	return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::LDY_DPY:  this->LDY(this->_getDirectIndexedByYAddr()); 	return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::SEP: this->SEP(this->_bus->read(this->_registers.pc++)); return 3;
//
//		case Instructions::REP: this->REP(this->_bus->read(this->_registers.pc++)); return 3;
//
//		case Instructions::PHA: this->PHA(); return 3 + !this->_registers.p.m;
//		case Instructions::PHB: this->PHB(); return 3;
//		case Instructions::PHD: this->PHD(); return 4;
//		case Instructions::PHK: this->PHK(); return 3;
//		case Instructions::PHP: this->PHP(); return 3;
//		case Instructions::PHX: this->PHX(); return 3 + !this->_registers.p.x_b;
//		case Instructions::PHY: this->PHY(); return 3 + !this->_registers.p.x_b;
//
//		case Instructions::PLA: this->PLA(); return 4 + !this->_registers.p.m;
//		case Instructions::PLB: this->PLB(); return 4;
//		case Instructions::PLD: this->PLD(); return 5;
//		case Instructions::PLP: this->PLP(); return 4;
//		case Instructions::PLX: this->PLX(); return 4 + !this->_registers.p.x_b;
//		case Instructions::PLY: this->PLY(); return 4 + !this->_registers.p.x_b;
//
//		case Instructions::JSR_ABS:   this->JSR(this->_getAbsoluteAddr()); 				   return 6;
//		case Instructions::JSR_ABSXi: this->JSR(this->_getAbsoluteIndirectIndexedByXAddr()); return 8;
//
//		case Instructions::JSL: this->JSL(this->_getAbsoluteLongAddr()); return 8;
//
//		case Instructions::CLC: this->CLC(); return 2;
//		case Instructions::CLI: this->CLI(); return 2;
//		case Instructions::CLD: this->CLD(); return 2;
//		case Instructions::CLV: this->CLV(); return 2;
//
//		case Instructions::SEC: this->SEC(); return 2;
//		case Instructions::SED: this->SED(); return 2;
//		case Instructions::SEI: this->SEI(); return 2;
//
//		case Instructions::AND_IM:   this->AND(this->_getImmediateAddrForA()); 					return 2 + !this->_registers.p.m;
//		case Instructions::AND_ABS:  this->AND(this->_getAbsoluteAddr()); 						return 4 + !this->_registers.p.m;
//		case Instructions::AND_ABSl: this->AND(this->_getAbsoluteLongAddr()); 					return 5 + !this->_registers.p.m;
//		case Instructions::AND_DP:   this->AND(this->_getDirectAddr()); 							return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_DPi:  this->AND(this->_getDirectIndirectAddr()); 					return 5 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_DPil: this->AND(this->_getDirectIndirectLongAddr()); 				return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_ABSX: this->AND(this->_getAbsoluteIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::AND_ABSXl:this->AND(this->_getAbsoluteIndexedByXLongAddr()); 			return 5 + !this->_registers.p.m;
//		case Instructions::AND_ABSY: this->AND(this->_getAbsoluteIndexedByYAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::AND_DPX:  this->AND(this->_getDirectIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_DPXi: this->AND(this->_getDirectIndirectIndexedXAddr());			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_DPYi: this->AND(this->_getDirectIndirectIndexedYAddr()); 			return 5 + !this->_registers.p.m + this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
//		case Instructions::AND_DPYil:this->AND(this->_getDirectIndirectIndexedYLongAddr()); 		return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::AND_SR:   this->AND(this->_getStackRelativeAddr()); 					return 4 + !this->_registers.p.m;
//		case Instructions::AND_SRYi: this->AND(this->_getStackRelativeIndirectIndexedYAddr()); 	return 7 + !this->_registers.p.m;
//
//		case Instructions::XCE: this->XCE(); return 2;
//
//		case Instructions::SBC_IM:   this->SBC(this->_getImmediateAddrForA()); 					return 2 + !this->_registers.p.m;
//		case Instructions::SBC_ABS:  this->SBC(this->_getAbsoluteAddr()); 						return 4 + !this->_registers.p.m;
//		case Instructions::SBC_ABSl: this->SBC(this->_getAbsoluteLongAddr()); 					return 5 + !this->_registers.p.m;
//		case Instructions::SBC_DP:   this->SBC(this->_getDirectAddr()); 							return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_DPi:  this->SBC(this->_getDirectIndirectAddr()); 					return 5 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_DPil: this->SBC(this->_getDirectIndirectLongAddr()); 				return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_ABSX: this->SBC(this->_getAbsoluteIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::SBC_ABSXl:this->SBC(this->_getAbsoluteIndexedByXLongAddr()); 			return 5 + !this->_registers.p.m;
//		case Instructions::SBC_ABSY: this->SBC(this->_getAbsoluteIndexedByYAddr()); 				return 4 + !this->_registers.p.m + this->_hasIndexCrossedPageBoundary;
//		case Instructions::SBC_DPX:  this->SBC(this->_getDirectIndexedByXAddr()); 				return 4 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_DPXi: this->SBC(this->_getDirectIndirectIndexedXAddr());			return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_DPYi: this->SBC(this->_getDirectIndirectIndexedYAddr()); 			return 5 + !this->_registers.p.m + this->_registers.dl != 0 + this->_hasIndexCrossedPageBoundary;
//		case Instructions::SBC_DPYil:this->SBC(this->_getDirectIndirectIndexedYLongAddr()); 		return 6 + !this->_registers.p.m + this->_registers.dl != 0;
//		case Instructions::SBC_SR:   this->SBC(this->_getStackRelativeAddr()); 					return 4 + !this->_registers.p.m;
//		case Instructions::SBC_SRYi: this->SBC(this->_getStackRelativeIndirectIndexedYAddr()); 	return 7 + !this->_registers.p.m;
//
//		case Instructions::TAX: 	this->TAX();	return 2;
//		case Instructions::TAY: 	this->TAY();	return 2;
//		case Instructions::TXS: 	this->TXS();	return 2;
//
//		case Instructions::INX: 	this->INX();	return 2;
//		case Instructions::INY: 	this->INY();	return 2;
//
//		case Instructions::CPX_IM: 	this->CPX(this->_getImmediateAddrForX()); return 2 + !this->_registers.p.m;
//		case Instructions::CPX_ABS: this->CPX(this->_getAbsoluteAddr());		 return 4 + !this->_registers.p.m;
//		case Instructions::CPX_DP: 	this->CPX(this->_getDirectAddr());		 return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::CPY_IM: 	this->CPY(this->_getImmediateAddrForX()); return 2 + !this->_registers.p.m;
//		case Instructions::CPY_ABS: this->CPY(this->_getAbsoluteAddr());		 return 4 + !this->_registers.p.m;
//		case Instructions::CPY_DP: 	this->CPY(this->_getDirectAddr());		 return 3 + !this->_registers.p.m + this->_registers.dl != 0;
//
//		case Instructions::BCC: return this->BCC(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BCS: return this->BCS(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BEQ: return this->BEQ(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BNE: return this->BNE(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BMI: return this->BMI(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BPL: return this->BPL(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BVC: return this->BVC(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BVS: return this->BVS(this->_registers.pc++) + 2 + this->_isEmulationMode;
//		case Instructions::BRA: this->BRA(this->_registers.pc++);  return 3 + this->_isEmulationMode;
//		case Instructions::BRL: this->BRL(this->_registers.pc); this->_registers.pc += 2;  return 4;
//
//		case Instructions::JMP_ABS: 	addr = this->_getAbsoluteAddr();           this->JMP(addr); 	   return 3;
//		case Instructions::JMP_ABSi: 	addr = this->_getAbsoluteIndirectAddr();   this->JMP(addr); 	   return 3;
//		case Instructions::JMP_ABSXi: 	addr = this->_getAbsoluteIndexedByXAddr(); this->JMP(addr); 	   return 3;
//
//		case Instructions::JML_ABSl: 	addr = this->_getAbsoluteLongAddr();       this->JML(addr);        return 3;
//		//case Instructions::JML_ABSil: 	this->JML(this->_getAbsoluteLong()); 	return 3;
//
//		default:
//			throw InvalidOpcode("CPU", opcode);
//		}
	}

	void CPU::_push(uint8_t data)
	{
		this->_bus->write(this->_registers.s--, data);
	}

	void CPU::_push(uint16_t data)
	{
		this->_bus->write(this->_registers.s--, data >> 8u);
		this->_bus->write(this->_registers.s--, data);
	}

	uint8_t CPU::_pop()
	{
		return this->_bus->read(++this->_registers.s);
	}

	uint16_t CPU::_pop16()
	{
		uint16_t value = this->_bus->read(++this->_registers.s);
		value +=this->_bus->read(++this->_registers.s) << 8u;
		return value;
	}

	std::string CPU::getName()
	{
		return "CPU";
	}

	Component CPU::getComponent()
	{
		return Cpu;
	}
}