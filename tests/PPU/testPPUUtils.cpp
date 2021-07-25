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
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0x7FFF) == 0xFFFFFFFF);
	CHECK(ComSquare::PPU::Utils::CGRAMColorToRGBA(0xFFFF) == 0xFFFF07FF);
}

TEST_CASE("Horizontal flip buffer simple use", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                            {1, 2, 3, 4},
		                                            {5, 6, 7, 8},
		                                            {9, 10, 11, 12},
		                                            {13, 14, 15, 16}
	                                            }};

	std::array<std::array<uint32_t, 4>, 4> bufferHFlipped = {{
		                                                    {13, 14, 15, 16},
		                                                    {9, 10, 11, 12},
		                                                    {5, 6, 7, 8},
		                                                    {1, 2, 3, 4}
	                                                    }};

	ComSquare::PPU::Utils::hFlip2DBuffer(buffer);
	CHECK(buffer == bufferHFlipped);
}

TEST_CASE("Vertical flip buffer simple use", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                                 {1, 2, 3, 4},
		                                                 {5, 6, 7, 8},
		                                                 {9, 10, 11, 12},
		                                                 {13, 14, 15, 16}
	                                                 }};

	std::array<std::array<uint32_t, 4>, 4> bufferVFlipped = {{
		                                                         {4, 3, 2, 1},
		                                                         {8, 7, 6, 5},
		                                                         {12, 11, 10, 9},
		                                                         {16, 15, 14, 13}
	                                                         }};

	ComSquare::PPU::Utils::vFlip2DBuffer(buffer);
	CHECK(buffer == bufferVFlipped);
}

TEST_CASE("Vertical flip buffer simple size use", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                                 {1, 2, 3, 4},
		                                                 {5, 6, 7, 8},
		                                                 {9, 10, 11, 12},
		                                                 {13, 14, 15, 16}
	                                                 }};

	std::array<std::array<uint32_t, 4>, 4> bufferVFlipped = {{
		                                                         {2, 1, 3, 4},
		                                                         {6, 5, 7, 8},
		                                                         {9, 10, 11, 12},
		                                                         {13, 14, 15, 16}
	                                                         }};

	ComSquare::PPU::Utils::vFlip2DBuffer(buffer, {2, 2});
	CHECK(buffer == bufferVFlipped);
}

TEST_CASE("Vertical flip buffer complex size use Y", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                                 {1, 2, 3, 4},
		                                                 {5, 6, 7, 8},
		                                                 {9, 10, 11, 12},
		                                                 {13, 14, 15, 16}
	                                                 }};

	std::array<std::array<uint32_t, 4>, 4> bufferVFlipped = {{
		                                                         {2, 1, 3, 4},
		                                                         {6, 5, 7, 8},
		                                                         {10, 9, 11, 12},
		                                                         {13, 14, 15, 16}
	                                                         }};

	ComSquare::PPU::Utils::vFlip2DBuffer(buffer, {2, 3});
	CHECK(buffer == bufferVFlipped);
}

TEST_CASE("Vertical flip buffer complex size use X", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                                 {1, 2, 3, 4},
		                                                 {5, 6, 7, 8},
		                                                 {9, 10, 11, 12},
		                                                 {13, 14, 15, 16}
	                                                 }};

	std::array<std::array<uint32_t, 4>, 4> bufferVFlipped = {{
		                                                         {3, 2, 1, 4},
		                                                         {7, 6, 5, 8},
		                                                         {9, 10, 11, 12},
		                                                         {13, 14, 15, 16}
	                                                         }};

	ComSquare::PPU::Utils::vFlip2DBuffer(buffer, {3, 2});
	CHECK(buffer == bufferVFlipped);
}

TEST_CASE("Vertical flip buffer simple offset use", "[PPU][Utils]")
{
	std::array<std::array<uint32_t, 4>, 4> buffer = {{
		                                                 {1, 2, 3, 4},
		                                                 {5, 6, 7, 8},
		                                                 {9, 10, 11, 12},
		                                                 {13, 14, 15, 16}
	                                                 }};

	std::array<std::array<uint32_t, 4>, 4> bufferVFlipped = {{
		                                                         {1, 2, 3, 4},
		                                                         {5, 6, 7, 8},
		                                                         {9, 10, 12, 11},
		                                                         {13, 14, 16, 15}
	                                                         }};

	ComSquare::PPU::Utils::vFlip2DBuffer(buffer, {2, 2}, {2, 2});
	CHECK(buffer == bufferVFlipped);
}