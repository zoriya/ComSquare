//
// Created by Melefo on 05/02/2020.
//

#pragma once

#include <cstring>
#include <memory>
// The include here is to prevent successive includes of this file to come after the define.
#include <filesystem>

#define private public
#define protected public

#include "Renderer/NoRenderer.hpp"
#include "SNES.hpp"

#define Init() \
	Renderer::NoRenderer norenderer(0, 0, 0);                  \
	auto snesPtr = std::make_unique<SNES>(norenderer);         \
	SNES &snes = *snesPtr;                                     \
	snes.cartridge._data.resize(100);                          \
	snes.cartridge.header.mappingMode = Cartridge::LoRom;      \
	snes.sram._data.resize(100);                               \
	snes.bus.mapComponents(snes);
