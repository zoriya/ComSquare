//
// Created by anonymus-raccoon on 1/24/20.
//

#include "CPU.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include "Exceptions/InvalidOpcode.hpp"
#include "Utility/Utility.hpp"
#include <iostream>

namespace ComSquare::CPU
{
	CPU::CPU(Memory::IMemoryBus &bus, Cartridge::Header &cartridgeHeader)
	    : _bus(bus),
	      _cartridgeHeader(cartridgeHeader),
	      _dmaChannels({DMA(bus), DMA(bus), DMA(bus), DMA(bus), DMA(bus), DMA(bus), DMA(bus), DMA(bus)})
	{
		this->RESB();
	}

	void CPU::setBus(Memory::IMemoryBus &bus)
	{
		this->_bus = bus;
		for (auto &dma : this->_dmaChannels)
			dma.setBus(bus);
	}

	//! @bref The CPU's internal registers starts at $4200	and finish at $421F.
	uint8_t CPU::read(uint24_t addr)
	{
		uint8_t tmp = 0;

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
			for (int i = 0; i < 8; i++)
				tmp |= this->_dmaChannels[i].enabled << i;
			return tmp;
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
		case 0x100 ... 0x180:
			return this->_dmaChannels[(addr - 0x100) >> 8u].read(addr & 0xF);
		default:
			throw InvalidAddress("CPU Internal Registers read", addr + this->_start);
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
			for (int i = 0; i < 8; i++)
				this->_dmaChannels[i].enabled = data & (0b1 << i);
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
		case 0x100 ... 0x180:
			this->_dmaChannels[(addr - 0x100) >> 8u].write(addr & 0xF, data);
			break;
		default:
			throw InvalidAddress("CPU Internal Registers write", addr + this->_start);
		}
	}

	uint24_t CPU::getSize() const
	{
		return 0x180;
	}

	unsigned CPU::update(unsigned maxCycles)
	{
		if (this->isDisabled)
			return 0xFF;
		unsigned cycles = this->runDMA(maxCycles);

		while (cycles < maxCycles) {
			if (this->_isStopped) {
				cycles += 1;
				continue;
			}

			this->_checkInterrupts();

			if (!this->_isWaitingForInterrupt)
				cycles += this->executeInstruction();
			else
				return 0xFF;
		}
		return cycles;
	}

	void CPU::_checkInterrupts()
	{
		if (!this->IsNMIRequested && !this->IsIRQRequested && !this->IsAbortRequested)
			return;
		this->_isWaitingForInterrupt = false;

		if (this->IsNMIRequested) {
			this->_runInterrupt(
			    this->_cartridgeHeader.nativeInterrupts.nmi,
			    this->_cartridgeHeader.emulationInterrupts.nmi);
			return;
		}
		if (this->IsIRQRequested && !this->_registers.p.i) {
			this->_runInterrupt(
			    this->_cartridgeHeader.nativeInterrupts.irq,
			    this->_cartridgeHeader.emulationInterrupts.irq);
			return;
		}
	}

	unsigned CPU::runDMA(unsigned maxCycles)
	{
		unsigned cycles = 0;

		for (DMA &channel : this->_dmaChannels) {
			if (!channel.enabled)
				continue;
			cycles += channel.run(maxCycles - cycles);
		}
		return cycles;
	}

	uint24_t CPU::_getValueAddr(const Instruction &instruction)
	{
		switch (instruction.addressingMode) {
		case Implied:
			return 0;
		case Immediate8bits:
			return this->_getImmediateAddr8Bits();
		case Immediate16bits:
			return this->_getImmediateAddr16Bits();
		case ImmediateForA:
			return this->_getImmediateAddrForA();
		case ImmediateForX:
			return this->_getImmediateAddrForX();

		case Absolute:
			return this->_getAbsoluteAddr();
		case AbsoluteLong:
			return this->_getAbsoluteLongAddr();
		case AbsoluteIndirect:
			return this->_getAbsoluteIndirectAddr();
		case AbsoluteIndirectLong:
			return this->_getAbsoluteIndirectLongAddr();

		case DirectPage:
			return this->_getDirectAddr();
		case DirectPageIndirect:
			return this->_getDirectIndirectAddr();
		case DirectPageIndirectLong:
			return this->_getDirectIndirectLongAddr();

		case DirectPageIndexedByX:
			return this->_getDirectIndexedByXAddr();
		case DirectPageIndexedByY:
			return this->_getDirectIndexedByYAddr();
		case DirectPageIndirectIndexedByX:
			return this->_getDirectIndirectIndexedXAddr();
		case DirectPageIndirectIndexedByY:
			return this->_getDirectIndirectIndexedYAddr();
		case DirectPageIndirectIndexedByYLong:
			return this->_getDirectIndirectIndexedYLongAddr();

		case AbsoluteIndexedByX:
			return this->_getAbsoluteIndexedByXAddr();
		case AbsoluteIndexedByXLong:
			return this->_getAbsoluteIndexedByXLongAddr();
		case AbsoluteIndexedByY:
			return this->_getAbsoluteIndexedByYAddr();

		case StackRelative:
			return this->_getStackRelativeAddr();
		case StackRelativeIndirectIndexedByY:
			return this->_getStackRelativeIndirectIndexedYAddr();

		case AbsoluteIndirectIndexedByX:
			return this->_getAbsoluteIndirectIndexedByXAddr();
		}
		throw InvalidOpcode("Unknown addressing mode for.");
	}

	unsigned CPU::executeInstruction()
	{
		const Instruction &instruction = this->instructions[this->_readPC()];
		this->_hasIndexCrossedPageBoundary = false;
		uint24_t valueAddr = this->_getValueAddr(instruction);

		return instruction.cycleCount + (this->*instruction.call)(valueAddr, instruction.addressingMode);
	}

	void CPU::_push(uint8_t data)
	{
		this->getBus().write(this->_registers.s--, data);
	}

	void CPU::_push(uint16_t data)
	{
		this->getBus().write(this->_registers.s--, data >> 8u);
		this->getBus().write(this->_registers.s--, data);
	}

	uint8_t CPU::_pop()
	{
		return this->getBus().read(++this->_registers.s);
	}

	uint16_t CPU::_pop16()
	{
		uint16_t value = this->getBus().read(++this->_registers.s);
		value += this->getBus().read(++this->_registers.s) << 8u;
		return value;
	}

	std::string CPU::getName() const
	{
		return "CPU";
	}

	Component CPU::getComponent() const
	{
		return Cpu;
	}
}// namespace ComSquare::CPU