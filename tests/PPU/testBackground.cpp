//
// Created by cbihan on 9/29/20.
//

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"

using namespace ComSquare;

TEST_CASE("basicTest backgroundGetTilePixelReference", "[backgroundGetTilePixelReference]")
{
	Init()
	snes.bus.write(0x2100, 0b11111111);
	REQUIRE(snes.ppu._registers._inidisp.fblank == true);
	REQUIRE(snes.ppu._registers._inidisp.brightness == 0xF);
}