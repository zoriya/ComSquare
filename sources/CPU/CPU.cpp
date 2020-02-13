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
			cycles += this->executeInstruction();
		return cycles;
	}

	unsigned CPU::executeInstruction()
	{
		uint8_t opcode = this->_bus->read(this->_registers.pc);

		this->_extraMemoryCycles = 0;

		switch (opcode) {
		case Instructions::BRK:      return 7 + this->BRK();

		case Instructions::RTI:	     return 6 + this->RTI();

		case Instructions::ADC_IM:   return 2 + this->ADC(this->_getImmediateAddr());
		case Instructions::ADC_ABS:  return 4 + this->ADC(this->_getAbsoluteAddr());
		case Instructions::ADC_ABSl: return 5 + this->ADC(this->_getAbsoluteLongAddr());
		case Instructions::ADC_DP:   return 3 + this->ADC(this->_getDirectAddr());
		case Instructions::ADC_DPi:  return 5 + this->ADC(this->_getDirectIndirectAddr());
		case Instructions::ADC_DPil: return 6 + this->ADC(this->_getDirectIndirectLongAddr());
		case Instructions::ADC_ABSX: return 4 + this->ADC(this->_getAbsoluteIndexedByXAddr());
		case Instructions::ADC_ABSXl:return 5 + this->ADC(this->_getAbsoluteIndexedByXLongAddr());
		case Instructions::ADC_ABSY: return 4 + this->ADC(this->_getAbsoluteIndexedByYAddr());
		case Instructions::ADC_DPX:  return 4 + this->ADC(this->_getDirectIndexedByXAddr());
		case Instructions::ADC_DPXi: return 6 + this->ADC(this->_getDirectIndirectIndexedXAddr());
		case Instructions::ADC_DPYi: return 5 + this->ADC(this->_getDirectIndirectIndexedYAddr());
		case Instructions::ADC_DPYil:return 6 + this->ADC(this->_getDirectIndirectIndexedYLongAddr());
		case Instructions::ADC_SR:   return 4 + this->ADC(this->_getStackRelativeAddr());
		case Instructions::ADC_SRYi: return 7 + this->ADC(this->_getStackRelativeIndirectIndexedYAddr());

		default:
			throw InvalidOpcode("CPU", opcode);
		}
	}

	void CPU::push(uint8_t data)
	{
		this->_bus->write(this->_registers.s--, data);
	}

	void CPU::push(uint16_t data)
	{
		this->_bus->write(this->_registers.s--, data);
		this->_bus->write(this->_registers.s--, data << 8u);
	}

	uint8_t CPU::pop()
	{
		return this->_bus->read(this->_registers.s++);
	}

	uint16_t CPU::pop16()
	{
		return this->_bus->read(this->_registers.s++) + (this->_bus->read(this->_registers.s++) << 8u);
	}

	////////////////////////////////////////////////////////////////////
	/// Addressing modes
	////////////////////////////////////////////////////////////////////

	uint24_t CPU::_getImmediateAddr()
	{
		uint24_t effective = this->_registers.pac++;
		if (this->_registers.p.m)
			this->_registers.pac++;
		return effective;
	}

	uint24_t CPU::_getDirectAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint8_t addr = this->_bus->read(this->_registers.pac++);
		return this->_registers.d + addr;
	}

	uint24_t CPU::_getAbsoluteAddr()
	{
		uint24_t addr = this->_registers.dbr << 16u;
		addr += this->_bus->read(this->_registers.pac++);
		addr += this->_bus->read(this->_registers.pac++) << 8u;
		return addr;
	}

	uint24_t CPU::_getAbsoluteLongAddr()
	{
		uint24_t addr = this->_bus->read(this->_registers.pac++);
		addr += this->_bus->read(this->_registers.pac++) << 8u;
		addr += this->_bus->read(this->_registers.pac++) << 16u;
		return addr;
	}

	uint24_t CPU::_getDirectIndirectIndexedYAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		if ((base & 0xF0000000u) == (((base + this->_registers.y) & 0xF0000000u)))
			this->_extraMemoryCycles++;
		return base + this->_registers.y;
	}

	uint24_t CPU::_getDirectIndirectIndexedYLongAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_bus->read(dp + 2) << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndirectIndexedXAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.x;
		uint24_t base = this->_bus->read(dp);
		base += this->_bus->read(dp + 1) << 8u;
		base += this->_registers.dbr << 16u;
		return base;
	}

	uint24_t CPU::_getDirectIndexedByXAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.x;
		return dp;
	}

	uint24_t CPU::_getDirectIndexedByYAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		dp += this->_registers.y;
		return dp;
	}

	uint24_t CPU::_getAbsoluteIndexedByXAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0xF0000000u) == (((effective + this->_registers.x) & 0xF0000000u)))
			this->_extraMemoryCycles++;
		return effective + this->_registers.x;
	}

	uint24_t CPU::_getAbsoluteIndexedByYAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = abs + (this->_registers.dbr << 16u);
		if ((effective & 0xF0000000u) == (((effective + this->_registers.y) & 0xF0000000u)))
			this->_extraMemoryCycles++;
		return effective + this->_registers.y;
	}

	uint24_t CPU::_getAbsoluteIndexedByXLongAddr()
	{
		uint24_t lng = this->_bus->read(this->_registers.pac++);
		lng += this->_bus->read(this->_registers.pac++) << 8u;
		lng += this->_bus->read(this->_registers.pac++) << 16u;
		return lng + this->_registers.x;
	}

	uint24_t CPU::_getProgramCounterRelativeAddr()
	{
		uint24_t pc = this->_registers.pac;
		int8_t mod = this->_bus->read(this->_registers.pac++);
		return pc + mod;
	}

	uint24_t CPU::_getProgramCounterRelativeLongAddr()
	{
		uint24_t pc = this->_registers.pac;
		uint8_t val1 = this->_bus->read(this->_registers.pac++);
		uint8_t val2 = this->_bus->read(this->_registers.pac++);
		int16_t mod = val2 > 0x7F ? (static_cast<char>(val2) * 256 - val1) : (val1 | val2 << 8u);
		return pc + mod;
	}

	uint24_t CPU::_getAbsoluteIndirectAddr()
	{
		uint16_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		uint24_t effective = this->_bus->read(abs);
		effective += this->_bus->read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getAbsoluteIndexedIndirectAddr()
	{
		uint24_t abs = this->_bus->read(this->_registers.pac++);
		abs += this->_bus->read(this->_registers.pac++) << 8u;
		abs += this->_registers.x;
		uint24_t effective = this->_bus->read(abs);
		effective += this->_bus->read(abs + 1) << 8u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t effective = this->_bus->read(dp);
		effective += this->_bus->read(dp + 1) << 8u;
		effective += this->_registers.dbr << 16u;
		return effective;
	}

	uint24_t CPU::_getDirectIndirectLongAddr()
	{
		if (this->_registers.dl != 0)
			this->_extraMemoryCycles++;

		uint16_t dp = this->_bus->read(this->_registers.pac++) + this->_registers.d;
		uint24_t effective = this->_bus->read(dp);
		effective += this->_bus->read(++dp) << 8u;
		effective += this->_bus->read(++dp) << 16u;
		return effective;
	}

	uint24_t CPU::_getStackRelativeAddr()
	{
		return this->_bus->read(this->_registers.pac++) + this->_registers.s;
	}

	uint24_t CPU::_getStackRelativeIndirectIndexedYAddr()
	{
		uint24_t base = this->_bus->read(this->_registers.pac++) + this->_registers.s;
		base += this->_registers.dbr << 16u;
		return base + this->_registers.y;
	}

	unsigned CPU::STA(uint24_t addr)
	{
		if (this->_registers.p.m)
			this->_bus->write(addr, this->_registers.al);
		else {
			this->_bus->write(addr, this->_registers.al);
			this->_bus->write(addr + 1, this->_registers.ah);
		}
		return 0;
	}
}