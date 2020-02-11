//
// Created by anonymus-raccoon on 1/23/20.
//

#include <algorithm>
#include <iostream>
#include "MemoryBus.hpp"
#include "../SNES.hpp"
#include "MemoryShadow.hpp"
#include "RectangleShadow.hpp"

namespace ComSquare::Memory
{
	std::shared_ptr<IMemory> MemoryBus::getAccessor(uint24_t addr)
	{
		auto it = std::find_if(this->_memoryAccessors.begin(), this->_memoryAccessors.end(), [addr](std::shared_ptr<IMemory> &accessor)
		{
			return accessor->hasMemoryAt(addr);
		});
		if (it == this->_memoryAccessors.end())
			return nullptr;
		return *it;
	}

	uint8_t MemoryBus::read(uint24_t addr)
	{
		std::shared_ptr<IMemory> handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Using open bus." << std::endl;
			return this->_openBus;
		}
		uint8_t data =  handler->read(addr - handler->getStart());
		this->_openBus = data;
		return data;
	}

	void MemoryBus::write(uint24_t addr, uint8_t data)
	{
		std::shared_ptr<IMemory> handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Warning, it was a write." << std::endl;
			return;
		}
		handler->write(addr - handler->getStart(), data);
	}

	void MemoryBus::_mirrorComponents(SNES &console, unsigned i)
	{
		this->_memoryAccessors.emplace_back(new Memory::RectangleShadow(console.wram, i, i, 0x0000, 0x1FFF));
		this->_memoryAccessors.emplace_back(new Memory::MemoryShadow(console.ppu, (i << 16u) + 0x2100, (i << 16u) + 0x213F));
		this->_memoryAccessors.emplace_back(new Memory::MemoryShadow(console.apu, (i << 16u) + 0x2140, (i << 16u) + 0x2143));
		this->_memoryAccessors.emplace_back(new Memory::MemoryShadow(console.cpu, (i << 16u) + 0x4200, (i << 16u) + 0x421F));
	}

	void MemoryBus::mapComponents(SNES &console)
	{
		// The WRam and PU registers are always mapped at the same address no matter the mapping mode.
		console.wram->setMemoryRegion(0x7E, 0x7F, 0x0000, 0xFFFF);
		this->_memoryAccessors.push_back(console.wram);

		console.ppu->setMemoryRegion(0x2100, 0x213F);
		this->_memoryAccessors.push_back(console.ppu);

		console.apu->setMemoryRegion(0x2140, 0x2143);
		this->_memoryAccessors.push_back(console.apu);

		console.cpu->setMemoryRegion(0x4200, 0x421F);
		this->_memoryAccessors.push_back(console.cpu);

		// TODO implement DMA & HDMA (4220 to 4300)
		// TODO implement Joys.

		// Mirror to the quarter 1.
		for (uint8_t i = 0x00; i < 0x40; i += 0x01)
			this->_mirrorComponents(console, i);
		// Mirror to the quarter 3.
		for (uint8_t i = 0x80; i < 0xC0; i += 0x01)
			this->_mirrorComponents(console, i);

		if (console.cartridge->header.mappingMode & Cartridge::LoRom) {
			console.cartridge->setMemoryRegion(0x80, 0xFF, 0x8000, 0xFFFF);
			this->_memoryAccessors.push_back(console.cartridge);
			// Mirror on the Q1 and Q2.
			this->_memoryAccessors.emplace_back(new Memory::RectangleShadow(console.cartridge, 0x00, 0x7D, 0x8000, 0xFFFF));
			// Mirror on the lower half of the Q2.
			this->_memoryAccessors.emplace_back((new Memory::RectangleShadow(console.cartridge, 0x40, 0x6F, 0x0000, 0x7FFF))->setBankOffset(0x40));
			// Mirror on the lower half of the Q4
			this->_memoryAccessors.emplace_back((new Memory::RectangleShadow(console.cartridge, 0xC0, 0xEF, 0x0000, 0x7FFF))->setBankOffset(0x40));

			console.sram->setMemoryRegion(0xF0, 0xFD, 0x0000, 0x7FFF);
			this->_memoryAccessors.push_back(console.sram);
			this->_memoryAccessors.emplace_back(new Memory::RectangleShadow(console.sram, 0x70, 0x7D, 0x0000, 0x7FFF));

			// TODO implement the SRam accessor for the FE/FF
		}
		// TODO should implement HiRom.
	}
}