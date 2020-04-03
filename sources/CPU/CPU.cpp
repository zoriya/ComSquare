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

	uint8_t CPU::readPC()
	{
		uint8_t ret = this->_bus->read(this->_registers.pac);
		this->_registers.pc++;
		return ret;
	}

	unsigned CPU::update()
	{
		unsigned cycles = 0;

		for (int i = 0; i < 0xFF; i++)
			cycles += this->_executeInstruction(this->readPC());
		return cycles;
	}

	unsigned CPU::_executeInstruction(uint8_t opcode)
	{
		Instruction instruction = this->_instructions[opcode];
		uint24_t valueAddr = 0;

		this->_hasIndexCrossedPageBoundary = false;

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

		case Absolute:
			valueAddr = this->_getAbsoluteAddr();
			break;
		case AbsoluteLong:
			valueAddr = this->_getAbsoluteLongAddr();
			break;
		case AbsoluteIndirect:
			valueAddr = this->_getAbsoluteIndirectAddr();
			break;
		case AbsoluteIndirectLong:
			valueAddr = this->_getAbsoluteIndirectLongAddr();
			break;

		case DirectPage:
			valueAddr = this->_getDirectAddr();
			break;
		case DirectPageIndirect:
			valueAddr = this->_getDirectIndirectAddr();
			break;
		case DirectPageIndirectLong:
			valueAddr = this->_getDirectIndirectLongAddr();
			break;

		case DirectPageIndexedByX:
			valueAddr = this->_getDirectIndexedByXAddr();
			break;
		case DirectPageIndexedByY:
			valueAddr = this->_getDirectIndexedByYAddr();
			break;
		case DirectPageIndirectIndexedByX:
			valueAddr = this->_getDirectIndirectIndexedXAddr();
			break;
		case DirectPageIndirectIndexedByY:
			valueAddr = this->_getDirectIndirectIndexedYAddr();
			break;
		case DirectPageIndirectIndexedByYLong:
			valueAddr = this->_getDirectIndirectIndexedYLongAddr();
			break;

		case AbsoluteIndexedByX:
			valueAddr = this->_getAbsoluteIndexedByXAddr();
			break;
		case AbsoluteIndexedByXLong:
			valueAddr = this->_getAbsoluteIndexedByXLongAddr();
			break;
		case AbsoluteIndexedByY:
			valueAddr = this->_getAbsoluteIndexedByYAddr();
			break;

		case StackRelative:
			valueAddr = this->_getStackRelativeAddr();
			break;
		case StackRelativeIndirectIndexedByY:
			valueAddr = this->_getStackRelativeIndirectIndexedYAddr();
			break;

		case AbsoluteIndirectIndexedByX:
			valueAddr = this->_getAbsoluteIndirectIndexedByXAddr();
			break;
		}

		return instruction.cycleCount + (this->*instruction.call)(valueAddr, instruction.addressingMode);
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