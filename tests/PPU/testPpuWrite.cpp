//
// Created by cbihan on 2/10/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"
using namespace ComSquare;

TEST_CASE("inidisp_data_full_ones PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2100, 0b11111111);
	REQUIRE(snes.ppu->_registers._inidisp.fblank == true);
	REQUIRE(snes.ppu->_registers._inidisp.brightness == 0xF);
}

TEST_CASE("inidisp_data_full_zeros PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2100, 0b00000000);
	REQUIRE(snes.ppu->_registers._inidisp.fblank == false);
	REQUIRE(snes.ppu->_registers._inidisp.brightness == 0x0);
}

TEST_CASE("inidisp_data_fBlank_on_brghtness_off PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2100, 0b10000000);
	REQUIRE(snes.ppu->_registers._inidisp.fblank == true);
	REQUIRE(snes.ppu->_registers._inidisp.brightness == 0x0);
}

TEST_CASE("inidisp_data_fBlank_off_brghtness_max PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2100, 0b00001111);
	REQUIRE(snes.ppu->_registers._inidisp.fblank == false);
	REQUIRE(snes.ppu->_registers._inidisp.brightness == 0xF);
}

TEST_CASE("inidisp_data_fBlank_off_brghtness_half PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2100, 0b00000101);
	REQUIRE(snes.ppu->_registers._inidisp.fblank == false);
	REQUIRE(snes.ppu->_registers._inidisp.brightness == 0x5);
}

TEST_CASE("obsel_111_object_size_and_all_null PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2101, 0b11100000);
	REQUIRE(snes.ppu->_registers._obsel.objectSize == 0b111);
	REQUIRE(snes.ppu->_registers._obsel.nameSelect == 0b00);
	REQUIRE(snes.ppu->_registers._obsel.nameBaseSelect == 0b000);
}

TEST_CASE("obsel_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2101, 0b11111111);
	REQUIRE(snes.ppu->_registers._obsel.objectSize == 0b111);
	REQUIRE(snes.ppu->_registers._obsel.nameSelect == 0b11);
	REQUIRE(snes.ppu->_registers._obsel.nameBaseSelect == 0b111);
}

TEST_CASE("obsel_data_full_nameselect PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2101, 0b00011000);
	REQUIRE(snes.ppu->_registers._obsel.objectSize == 0b000);
	REQUIRE(snes.ppu->_registers._obsel.nameSelect == 0b11);
	REQUIRE(snes.ppu->_registers._obsel.nameBaseSelect == 0b000);
}

TEST_CASE("obsel_data_full_baseselect_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2101, 0b00000111);
	REQUIRE(snes.ppu->_registers._obsel.objectSize == 0b000);
	REQUIRE(snes.ppu->_registers._obsel.nameSelect == 0b00);
	REQUIRE(snes.ppu->_registers._obsel.nameBaseSelect == 0b111);
}

TEST_CASE("oamaddl_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	REQUIRE(snes.ppu->_registers._oamadd.oamAddress == 0b011111111);
}

TEST_CASE("oamaddh_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2103, 0b11111111);
	REQUIRE(snes.ppu->_registers._oamadd.objPriorityActivationBit == true);
	REQUIRE(snes.ppu->_registers._oamadd.oamAddress == 0b100000000);
}

TEST_CASE("oamaddlh_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	snes.bus->write(0x2103, 0b11111111);
	REQUIRE(snes.ppu->_registers._oamadd.objPriorityActivationBit == true);
	REQUIRE(snes.ppu->_registers._oamadd.oamAddress == 0b111111111);
}

TEST_CASE("oamaddlh_data_full_priorityBit_off PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	snes.bus->write(0x2103, 0b01111111);
	REQUIRE(snes.ppu->_registers._oamadd.objPriorityActivationBit == false);
	REQUIRE(snes.ppu->_registers._oamadd.oamAddress == 0b111111111);
}

TEST_CASE("oamaddlh_oamAdress_11_priorityBit_on PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2102, 0b00001011);
	snes.bus->write(0x2103, 0b10011100);
	REQUIRE(snes.ppu->_registers._oamadd.objPriorityActivationBit == true);
	REQUIRE(snes.ppu->_registers._oamadd.oamAddress == 11);
}

TEST_CASE("bgmode_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2105, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgmode.bgMode == 7);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg1 == true);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg2 == true);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg3 == true);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg4 == true);
	REQUIRE(snes.ppu->_registers._bgmode.mode1Bg3PriorityBit == true);
}

TEST_CASE("bgmode_bgmode_5_and_bg24_on PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2105, 0b10100101);
	REQUIRE(snes.ppu->_registers._bgmode.bgMode == 5);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg1 == false);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg2 == true);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg3 == false);
	REQUIRE(snes.ppu->_registers._bgmode.characterSizeBg4 == true);
	REQUIRE(snes.ppu->_registers._bgmode.mode1Bg3PriorityBit == false);
}

TEST_CASE("mosaic_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2106, 0b11111111);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg1 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg2 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg3 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg4 == true);
	REQUIRE(snes.ppu->_registers._mosaic.pixelSize == 0xF);
}

TEST_CASE("mosaic_affectbg23_w_1x1_size PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2106, 0b00000110);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg1 == false);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg2 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg3 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg4 == false);
	REQUIRE(snes.ppu->_registers._mosaic.pixelSize == 0x0);
}

TEST_CASE("mosaic_affectbg14_w_2x2_size PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2106, 0b00101001);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg1 == true);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg2 == false);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg3 == false);
	REQUIRE(snes.ppu->_registers._mosaic.affectBg4 == true);
	REQUIRE(snes.ppu->_registers._mosaic.pixelSize == 0x2);
}

TEST_CASE("bg1sc_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2107, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgsc[0].tilemapAddress == 0b111111);
	REQUIRE(snes.ppu->_registers._bgsc[0].tilemapHorizontalMirroring == true);
	REQUIRE(snes.ppu->_registers._bgsc[0].tilemapVerticalMirroring == true);
}

TEST_CASE("bg2sc_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2108, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgsc[1].tilemapAddress == 0b111111);
	REQUIRE(snes.ppu->_registers._bgsc[1].tilemapHorizontalMirroring == true);
	REQUIRE(snes.ppu->_registers._bgsc[1].tilemapVerticalMirroring == true);
}

TEST_CASE("bg3sc_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x2109, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgsc[2].tilemapAddress == 0b111111);
	REQUIRE(snes.ppu->_registers._bgsc[2].tilemapHorizontalMirroring == true);
	REQUIRE(snes.ppu->_registers._bgsc[2].tilemapVerticalMirroring == true);
}

TEST_CASE("bg4sc_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210A, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapAddress == 0b111111);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring == true);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring == true);
}

TEST_CASE("bg4sc_data_null PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210A, 0b00000000);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapAddress == 0);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring == false);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring == false);
}

TEST_CASE("bg4sc_horizontal_off_vertical_on_random_tilemapAdress PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210A, 0b11000110);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapAddress == 0b110001);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring == false);
	REQUIRE(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring == true);
}

TEST_CASE("bg12nba_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210B, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgnba[0].baseAddressBg1a3 == 0b1111);
	REQUIRE(snes.ppu->_registers._bgnba[0].baseAddressBg2a4 == 0b1111);
}

TEST_CASE("bg34nba_data_full PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210C, 0b11111111);
	REQUIRE(snes.ppu->_registers._bgnba[1].baseAddressBg1a3 == 0b1111);
	REQUIRE(snes.ppu->_registers._bgnba[1].baseAddressBg2a4 == 0b1111);
}

TEST_CASE("bg12nba_data_random_data PPU_write", "[PPU_write]")
{
	Init()
	snes.bus->write(0x210B, 0b10101010);
	REQUIRE(snes.ppu->_registers._bgnba[0].baseAddressBg1a3 == 0b1010);
	REQUIRE(snes.ppu->_registers._bgnba[0].baseAddressBg2a4 == 0b1010);
}