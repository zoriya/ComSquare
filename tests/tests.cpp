//
// Created by anonymus-raccoon on 2/10/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <zconf.h>
#include "tests.hpp"
#include "../sources/Renderer/NoRenderer.hpp"
#include "../sources/SNES.hpp"

using namespace ComSquare;

std::pair<std::shared_ptr<Memory::MemoryBus>, SNES> Init()
{
	Renderer::NoRenderer norenderer(0, 0, 0);
	SNES snes("../tests/my_cartridge", norenderer);
	snes.cartridge->_size = 100;
	snes.cartridge->_data = new uint8_t[snes.cartridge->_size];
	snes.cartridge->header.mappingMode = Cartridge::LoRom;
	snes.sram->_size = 100;
	snes.sram->_data = new uint8_t[snes.cartridge->_size];
	return std::make_pair(snes._bus, snes);
}