//
// Created by cbihan on 25/07/2021.
//

#include <catch2/catch_test_macros.hpp>
#include "PPU/PPUUtils.hpp"

TEST_CASE("Various color conversions CGRAMColorToRGBA", "[PPU][Utils]")
{
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x7210) == 0x8484E7FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x7398) == 0xC6E7E7FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x019C) == 0xE76300FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x521C) == 0xE784A5FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x039C) == 0xE7E700FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x0000) == 0x000000FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x001C) == 0xE70000FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x5284) == 0x21A5A5FF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x7E20) == 0x008CFFFF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x37BF) == 0xFFEF6BFF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x3E78) == 0xC69C7BFF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x428B) == 0x5AA584FF);
}
