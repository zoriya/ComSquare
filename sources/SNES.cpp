//
// Created by anonymus-raccoon on 1/27/20.
//

#include "SNES.hpp"

namespace ComSquare
{
	SNES::SNES(const std::shared_ptr<Memory::MemoryBus> &bus, const std::string &romPath, Renderer::IRenderer &renderer) :
		cpu(new CPU::CPU(bus)),
		ppu(new PPU::PPU()),
		apu(new APU::APU()),
		cartridge(new Cartridge::Cartridge(romPath)),
		wram(new Ram::Ram(16384)),
		sram(new Ram::Ram(this->cartridge->header.sramSize))
	{
		bus->mapComponents(*this);
		renderer.setWindowName(this->cartridge->header.gameName);
	}
}
