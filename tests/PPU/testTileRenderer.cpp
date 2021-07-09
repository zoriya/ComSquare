//
// Created by cbihan on 08/07/2021.
//

#include <sstream>
#include <string>
#include <span>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include "PPU/TileRenderer.hpp"
#include "Ram/Ram.hpp"
#include "PPU/Tile.hpp"

TEST_CASE("read2BPPValue", "[PPU][TileRenderer]")
{
	ComSquare::Ram::Ram vram(100, static_cast<ComSquare::Component>(0), "vramTest");
	ComSquare::Ram::Ram cgram(512, static_cast<ComSquare::Component>(0), "cgramTest");
    ComSquare::PPU::TileRenderer tileRenderer(vram, cgram);

    vram.write(0, 0xBA);
    vram.write(1, 0x7C);

	CHECK(tileRenderer.read2BPPValue(0, 0) == 1);
	CHECK(tileRenderer.read2BPPValue(0, 1) == 2);
	CHECK(tileRenderer.read2BPPValue(0, 2) == 3);
	CHECK(tileRenderer.read2BPPValue(0, 3) == 3);
	CHECK(tileRenderer.read2BPPValue(0, 4) == 3);
	CHECK(tileRenderer.read2BPPValue(0, 5) == 2);
	CHECK(tileRenderer.read2BPPValue(0, 6) == 1);
	CHECK(tileRenderer.read2BPPValue(0, 7) == 0);
}

TEST_CASE("read2BPPValue with circular ram", "[PPU][TileRenderer]")
{
	ComSquare::Ram::Ram vram(10, static_cast<ComSquare::Component>(0), "vramTest");
	ComSquare::Ram::Ram cgram(512, static_cast<ComSquare::Component>(0), "cgramTest");
	ComSquare::PPU::TileRenderer tileRenderer(vram, cgram);

	vram.write(9, 0xBA);
	vram.write(0, 0x7C);

	CHECK(tileRenderer.read2BPPValue(9, 0) == 1);
	CHECK(tileRenderer.read2BPPValue(9, 1) == 2);
	CHECK(tileRenderer.read2BPPValue(9, 2) == 3);
	CHECK(tileRenderer.read2BPPValue(9, 3) == 3);
	CHECK(tileRenderer.read2BPPValue(9, 4) == 3);
	CHECK(tileRenderer.read2BPPValue(9, 5) == 2);
	CHECK(tileRenderer.read2BPPValue(9, 6) == 1);
	CHECK(tileRenderer.read2BPPValue(9, 7) == 0);
}

TEST_CASE("getPixelReferenceFromTileRow 2bpp", "[PPU][TileRenderer]")
{
	ComSquare::Ram::Ram vram(10, static_cast<ComSquare::Component>(0), "vramTest");
	ComSquare::Ram::Ram cgram(512, static_cast<ComSquare::Component>(0), "cgramTest");
	ComSquare::PPU::TileRenderer tileRenderer(vram, cgram);

	tileRenderer.setBpp(2);

	vram.write(2, 0xD6);
	vram.write(3, 0x00);

	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 0) == 1);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 1) == 1);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 2) == 0);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 3) == 1);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 4) == 0);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 5) == 1);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 6) == 1);
	CHECK(tileRenderer.getPixelReferenceFromTileRow(2, 7) == 0);
}

TEST_CASE("getPixelReferenceFromTileRow 4bpp", "[PPU][TileRenderer]")
{
	ComSquare::Ram::Ram vram(40, static_cast<ComSquare::Component>(0), "vramTest");
	ComSquare::Ram::Ram cgram(512, static_cast<ComSquare::Component>(0), "cgramTest");
	ComSquare::PPU::TileRenderer tileRenderer(vram, cgram);

	tileRenderer.setBpp(4);

	std::vector<std::string> vramValues {
		"7C7C82EE82FE7C7C",
		"0000D68254443838",
		"7C00AA1082007C00",
		"1000540038000000"
	};

	uint8_t value;
	int i = 0;

	for (auto row : vramValues) {
		while (!row.empty()) {
			value = std::stoul(row.substr(0, 2), nullptr, 16);
			row.erase(0, 2);
			vram.write(i++, value);
		}
	}

	char correctValues[8][8] = {
        {0, 7, 7, 7, 7, 7, 0, 0},
        {7, 2, 6, 8, 6, 2, 7, 0},
        {7, 2, 2, 2, 2, 2, 7, 0},
        {0, 7, 7, 7, 7, 7, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0},
        {3, 5, 0, 5, 0, 5, 3, 0},
        {0, 3, 4, 5, 4, 3, 0, 0},
        {0, 0, 3, 3, 3, 0, 0, 0}
    };

	i = 0;
	int j = 0;
	for (const auto &row : correctValues) {
		for (const auto &refValue : row) {
			CHECK(tileRenderer.getPixelReferenceFromTileRow(i, j++) == refValue);
		}
		j = 0;
		i += 2;
	}
}

TEST_CASE("getPixelReferenceFromTileRow 8bpp", "[PPU][TileRenderer]")
{
	ComSquare::Ram::Ram vram(100, static_cast<ComSquare::Component>(0), "vramTest");
	ComSquare::Ram::Ram cgram(512, static_cast<ComSquare::Component>(0), "cgramTest");
	ComSquare::PPU::TileRenderer tileRenderer(vram, cgram);

	tileRenderer.setBpp(8);

	std::vector<std::string> vramValues {
		"0C7C5CA0C0BC3C001010964038282018",
		"0000020002007C000000820044003800",
		"007C44927C82007C1010D6D67C7C3838",
		"00002800000000000000000000000000"
	};

	uint8_t value;
	int i = 0;

	for (auto row : vramValues) {
		while (!row.empty()) {
			value = std::stoul(row.substr(0, 2), nullptr, 16);
			row.erase(0, 2);
			vram.write(i++, value);
		}
	}

	uint8_t correctValues[8][8] = {
		{0x00, 0x22, 0x22, 0x22, 0x23, 0x23, 0x00, 0x00},
		{0x22, 0x11, 0x42, 0x21, 0x41, 0x11, 0x24, 0x00},
		{0x23, 0x11, 0x12, 0x12, 0x12, 0x12, 0x24, 0x00},
		{0x00, 0x24, 0x25, 0x25, 0x25, 0x25, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00},
		{0x35, 0x32, 0x00, 0x31, 0x00, 0x31, 0x35, 0x00},
		{0x00, 0x34, 0x33, 0x31, 0x33, 0x34, 0x00, 0x00},
		{0x00, 0x00, 0x35, 0x36, 0x36, 0x00, 0x00, 0x00},
	};


	i = 0;
	int j = 0;
	for (const auto &row : correctValues) {
		for (const auto &refValue : row) {
			std::cout << i << ' ' << j << std::endl;
			CHECK(tileRenderer.getPixelReferenceFromTileRow(i, j++) == refValue);
		}
		j = 0;
		i += 2;
	}
}
