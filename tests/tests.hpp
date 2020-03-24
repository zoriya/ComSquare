//
// Created by Melefo on 05/02/2020.
//

#ifndef COMSQUARE_TESTS_HPP
#define COMSQUARE_TESTS_HPP

#define private public
#define protected public
#define class struct

#include "../sources/Memory/MemoryBus.hpp"
#include <criterion/criterion.h>
#include <iostream>
#include <zconf.h>
#include "tests.hpp"
#include "../sources/Renderer/NoRenderer.hpp"
#include "../sources/SNES.hpp"

#define Init() \
	Renderer::NoRenderer norenderer(0, 0, 0); \
	SNES snes("../tests/my_cartridge", norenderer); \
	snes.cartridge->_size = 100; \
	snes.cartridge->_data = new uint8_t[snes.cartridge->_size]; \
	snes.cartridge->header.mappingMode = Cartridge::LoRom; \
	snes.sram->_size = 100; \
	snes.sram->_data = new uint8_t[snes.cartridge->_size];

#endif //COMSQUARE_TESTS_HPP
