//
// Created by Melefo on 05/02/2020.
//

#pragma once

#include <cstring>
// The include here is to prevent successive includes of this file to come after the define.
#include <filesystem>

#define private public
#define protected public
#define class struct

#include "Memory/MemoryBus.hpp"
#include "Renderer/NoRenderer.hpp"
#include "SNES.hpp"

#define Init() \
	Renderer::NoRenderer norenderer(0, 0, 0);                   \
	SNES snes(norenderer);             \
	snes.cartridge->_size = 100;                                \
    delete[] snes.cartridge->_data;                             \
	snes.cartridge->_data = new uint8_t[snes.cartridge->_size]; \
    memset(snes.cartridge->_data, 0, snes.cartridge->_size);    \
	snes.cartridge->header.mappingMode = Cartridge::LoRom;      \
	snes.sram->_size = 100;                                     \
    delete[] snes.sram->_data;                                  \
	snes.sram->_data = new uint8_t[snes.sram->_size];           \
	memset(snes.sram->_data, 0, snes.cartridge->_size);
