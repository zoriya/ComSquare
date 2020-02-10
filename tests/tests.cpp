//
// Created by anonymus-raccoon on 2/10/20.
//

#include <criterion/criterion.h>
#include "tests.hpp"
#include "../sources/Renderer/NoRenderer.hpp"
#include "../sources/SNES.hpp"

using namespace ComSquare;

std::pair<Memory::MemoryBus, SNES> Init()
{
	Memory::MemoryBus bus;
	Renderer::NoRenderer norenderer(0, 0, 0);
	SNES snes(std::make_shared<Memory::MemoryBus>(bus), "", norenderer);
	snes.cartridge->_size = 100;
	snes.cartridge->_data = new uint8_t[snes.cartridge->_size];
	snes.cartridge->header.mappingMode = Cartridge::LoRom;
	snes.sram->_size = 10;
	snes.sram->_data = new uint8_t[snes.cartridge->_size];
	bus.mapComponents(snes);
	return std::make_pair(bus, snes);
}