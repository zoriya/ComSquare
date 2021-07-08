//
// Created by cbihan on 2/1/21.
//

#include <catch2/catch_test_macros.hpp>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"

using namespace ComSquare;

TEST_CASE("vram_data_read_full PPU_read_1", "[PPU_read_1]")
{
	Init()
	snes.bus.write(0x2115, 0b10000000);
	snes.bus.write(0x2116, 0);
	snes.bus.write(0x2117, 0);
	snes.ppu.vram->write(0, 0b11111111);
	snes.ppu.vram->write(1, 0b11111111);

	snes.bus.write(0x2116, 0);
	snes.bus.write(0x2117, 0);
	uint8_t tmp = snes.bus.read(0x2139);
	CHECK(tmp == 0b11111111);
	tmp = snes.bus.read(0x213a);
	CHECK(tmp == 0b11111111);
}

TEST_CASE("vram_data_read_half PPU_read_1", "[PPU_read_1]")
{
	Init()
	snes.bus.write(0x2116, 0);
	snes.bus.write(0x2117, 0);
	snes.ppu.vram->write(0, 0b01101001);
	snes.ppu.vram->write(1, 0b11111111);

	snes.bus.write(0x2116, 0);
	snes.bus.write(0x2117, 0);
	uint8_t tmp = snes.bus.read(0x2139);
	CHECK(tmp == 0b01101001);
	tmp = snes.bus.read(0x213a);
	CHECK(tmp == 0b11111111);
}