//
// Created by anonymus-raccoon on 1/23/20.
//

#include <iostream>
#include "SNES.hpp"
#include "Memory/MemoryBus.hpp"
#include "Memory/MemoryShadow.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include "Models/Logger.hpp"

namespace ComSquare::Memory
{
	IMemory *MemoryBus::getAccessor(uint24_t addr)
	{
		auto it = std::find_if(this->_memoryAccessors.begin(), this->_memoryAccessors.end(), [addr](IMemory &accessor)
		{
			return accessor.hasMemoryAt(addr);
		});
		if (it == this->_memoryAccessors.end())
			return nullptr;
		return &it->get();
	}

	uint8_t MemoryBus::read(uint24_t addr)
	{
		IMemory *handler = this->getAccessor(addr);

		if (!handler) {
			logMsg(LogLevel::WARNING, "Unknown memory accessor for address $" << std::hex << addr << ". Using open bus.");
			return this->_openBus;
		}

		uint8_t data = handler->read(handler->getRelativeAddress(addr));
		this->_openBus = data;
		return data;
	}

	std::optional<uint8_t> MemoryBus::peek(uint24_t addr)
	{
		IMemory *handler = this->getAccessor(addr);

		if (!handler)
			return this->_openBus;
		try {
			return handler->read(handler->getRelativeAddress(addr));
		} catch (const InvalidAddress &) {
			return std::nullopt;
		}
	}

	uint8_t MemoryBus::peek_v(uint24_t addr)
	{
		auto value = this->peek(addr);
		if (value.has_value())
			return value.value();
		return 0;
	}

	void MemoryBus::write(uint24_t addr, uint8_t data)
	{
		IMemory *handler = this->getAccessor(addr);

		if (!handler) {
			logMsg(LogLevel::ERROR, "Unknown memory accessor for address " << std::hex << addr << ". Warning, it was a write.");
			return;
		}
		handler->write(handler->getRelativeAddress(addr), data);
	}

	void MemoryBus::_mirrorComponents(SNES &console, unsigned i)
	{
		this->_rectangleShadows.emplace_back(console.wram, i, i, 0x0000, 0x1FFF);
		this->_shadows.emplace_back(console.ppu, (i << 16u) + 0x2100, (i << 16u) + 0x213F);
		this->_shadows.emplace_back(console.apu, (i << 16u) + 0x2140, (i << 16u) + 0x2143);
		this->_shadows.emplace_back(console.cpu, (i << 16u) + 0x4200, (i << 16u) + 0x421F);
	}

	void MemoryBus::mapComponents(SNES &console)
	{
		this->_memoryAccessors.clear();
		this->_shadows.clear();
		this->_rectangleShadows.clear();

		// The WRam and PU registers are always mapped at the same address no matter the mapping mode.
		console.wram.setMemoryRegion(0x7E, 0x7F, 0x0000, 0xFFFF);
		this->_memoryAccessors.emplace_back(console.wram);

		console.ppu.setMemoryRegion(0x2100, 0x213F);
		this->_memoryAccessors.emplace_back(console.ppu);

		console.apu.setMemoryRegion(0x2140, 0x2143);
		this->_memoryAccessors.emplace_back(console.apu);

		console.cpu.setMemoryRegion(0x4200, 0x44FF);
		this->_memoryAccessors.emplace_back(console.cpu);

		// TODO implement Joys.

		// Mirror to the quarter 1.
		for (uint8_t i = 0x00; i < 0x40; i += 0x01)
			this->_mirrorComponents(console, i);
		// Mirror to the quarter 3.
		for (uint8_t i = 0x80; i < 0xC0; i += 0x01)
			this->_mirrorComponents(console, i);

		if (console.cartridge.header.mappingMode & Cartridge::LoRom) {
			console.cartridge.setMemoryRegion(0x80, 0xFF, 0x8000, 0xFFFF);
			this->_memoryAccessors.emplace_back(console.cartridge);
			// Mirror on the Q1 and Q2.
			this->_rectangleShadows.emplace_back(console.cartridge, 0x00, 0x7D, 0x8000, 0xFFFF);
			// Mirror on the lower half of the Q2.
			this->_rectangleShadows.emplace_back(console.cartridge, 0x40, 0x6F, 0x0000, 0x7FFF).setBankOffset(0x40);
			// Mirror on the lower half of the Q4
			this->_rectangleShadows.emplace_back(console.cartridge, 0xC0, 0xEF, 0x0000, 0x7FFF).setBankOffset(0x40);

			console.sram.setMemoryRegion(0xF0, 0xFD, 0x0000, 0x7FFF);
			this->_memoryAccessors.emplace_back(console.sram);
			this->_rectangleShadows.emplace_back(console.sram, 0x70, 0x7D, 0x0000, 0x7FFF);

			// TODO implement the SRam accessor for the FE/FF
		}
		// TODO should implement HiRom.

		for (auto &shadow : this->_shadows)
			this->_memoryAccessors.emplace_back(shadow);
		for (auto &shadow : this->_rectangleShadows)
			this->_memoryAccessors.emplace_back(shadow);
	}
}