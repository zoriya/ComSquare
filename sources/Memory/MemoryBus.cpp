//
// Created by anonymus-raccoon on 1/23/20.
//

#include <algorithm>
#include <iostream>
#include "MemoryBus.hpp"
#include "../SNES.hpp"
#include "MemoryShadow.hpp"

namespace ComSquare
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
			return this->_openbus;
		}
		uint8_t data =  handler->read(addr - handler->getStart());
		this->_openbus = data;
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

	void MemoryBus::_mirrorComponents(SNES &console, int i)
	{
		this->_memoryAccessors.push_back(Memory::MemoryShadow::createShadow(console.wram, i, i + 0x2000));
		this->_memoryAccessors.push_back(Memory::MemoryShadow::createShadow(console.cpu, i + 0x4200, i + 0x4220));
	}

	void MemoryBus::mapComponents(SNES &console)
	{
		// The WRam is always mapped in the bank 7E and 7F, no matter the memory mapping mode.
		console.wram->setMemoryRegion(0x7E0000, 0x7FFFFF);
		this->_memoryAccessors.push_back(console.wram);

		console.ppu->setMemoryRegion(0x2100, 0x2140);
		this->_memoryAccessors.push_back(console.ppu);

		console.apu->setMemoryRegion(0x2140, 0x2144);
		this->_memoryAccessors.push_back(console.apu);

		console.cpu->setMemoryRegion(0x4200, 0x4220);
		this->_memoryAccessors.push_back(console.cpu);

		// TODO implement DMA & HDMA (4220 to 4300)

		//Mirror the first $2000 bits of the WRam to all banks of the Q1 & Q3.
		for (uint24_t i = 0; i < 0x400000; i += 0x10000) {
			this->_mirrorComponents(console, i);
		}
		for (uint24_t i = 0x800000; i < 0xC00000; i += 0x10000) {
			this->_memoryAccessors.push_back(Memory::MemoryShadow::createShadow(console.wram, i, i + 0x2000));
			this->_memoryAccessors.push_back(Memory::MemoryShadow::createShadow(console.cpu, i + 0x4200, i + 0x4220));
		}
	}
}