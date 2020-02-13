//
// Created by anonymus-raccoon on 1/27/20.
//

#include <ios>
#include <iostream>
#include "SNES.hpp"

namespace ComSquare
{
	SNES::SNES(const std::shared_ptr<Memory::MemoryBus> &bus, const std::string &romPath, Renderer::IRenderer &renderer) :
		cartridge(new Cartridge::Cartridge(romPath)),
		cpu(new CPU::CPU(bus, cartridge->header)),
		ppu(new PPU::PPU(bus, renderer)),
		apu(new APU::APU()),
		wram(new Ram::Ram(16384)),
		sram(new Ram::Ram(this->cartridge->header.sramSize))
	{
		bus->mapComponents(*this);
		renderer.setWindowName(this->cartridge->header.gameName);
	}
}
