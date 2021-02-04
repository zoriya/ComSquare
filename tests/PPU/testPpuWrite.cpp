//
// Created by cbihan on 2/10/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"
using namespace ComSquare;

Test(PPU_write, inidisp_data_full_ones)
{
	Init()
	snes.bus->write(0x2100, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, true);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0xF);
}

Test(PPU_write, inidisp_data_full_zeros)
{
	Init()
	snes.bus->write(0x2100, 0b00000000);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, false);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0x0);
}

Test(PPU_write, inidisp_data_fBlank_on_brghtness_off)
{
	Init()
	snes.bus->write(0x2100, 0b10000000);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, true);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0x0);
}

Test(PPU_write, inidisp_data_fBlank_off_brghtness_max)
{
	Init()
	snes.bus->write(0x2100, 0b00001111);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, false);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0xF);
}

Test(PPU_write, inidisp_data_fBlank_off_brghtness_half)
{
	Init()
	snes.bus->write(0x2100, 0b00000101);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, false);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0x5);
}

Test(PPU_write, obsel_111_object_size_and_all_null)
{
	Init()
	snes.bus->write(0x2101, 0b11100000);
	cr_assert_eq(snes.ppu->_registers._obsel.objectSize, 0b111);
	cr_assert_eq(snes.ppu->_registers._obsel.nameSelect, 0b00);
	cr_assert_eq(snes.ppu->_registers._obsel.nameBaseSelect, 0b000);
}

Test(PPU_write, obsel_data_full)
{
	Init()
	snes.bus->write(0x2101, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._obsel.objectSize, 0b111);
	cr_assert_eq(snes.ppu->_registers._obsel.nameSelect, 0b11);
	cr_assert_eq(snes.ppu->_registers._obsel.nameBaseSelect, 0b111);
}

Test(PPU_write, obsel_data_full_nameselect)
{
	Init()
	snes.bus->write(0x2101, 0b00011000);
	cr_assert_eq(snes.ppu->_registers._obsel.objectSize, 0b000);
	cr_assert_eq(snes.ppu->_registers._obsel.nameSelect, 0b11);
	cr_assert_eq(snes.ppu->_registers._obsel.nameBaseSelect, 0b000);
}

Test(PPU_write, obsel_data_full_baseselect)
{
	Init()
	snes.bus->write(0x2101, 0b00000111);
	cr_assert_eq(snes.ppu->_registers._obsel.objectSize, 0b000);
	cr_assert_eq(snes.ppu->_registers._obsel.nameSelect, 0b00);
	cr_assert_eq(snes.ppu->_registers._obsel.nameBaseSelect, 0b111);
}

Test(PPU_write, oamaddl_data_full)
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._oamadd.oamAddress, 0b011111111);
}

Test(PPU_write, oamaddh_data_full)
{
	Init()
	snes.bus->write(0x2103, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._oamadd.objPriorityActivationBit, true);
	cr_assert_eq(snes.ppu->_registers._oamadd.oamAddress, 0b100000000);
}

Test(PPU_write, oamaddlh_data_full)
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	snes.bus->write(0x2103, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._oamadd.objPriorityActivationBit, true);
	cr_assert_eq(snes.ppu->_registers._oamadd.oamAddress, 0b111111111);
}

Test(PPU_write, oamaddlh_data_full_priorityBit_off)
{
	Init()
	snes.bus->write(0x2102, 0b11111111);
	snes.bus->write(0x2103, 0b01111111);
	cr_assert_eq(snes.ppu->_registers._oamadd.objPriorityActivationBit, false);
	cr_assert_eq(snes.ppu->_registers._oamadd.oamAddress, 0b111111111);
}

Test(PPU_write, oamaddlh_oamAdress_11_priorityBit_on)
{
	Init()
	snes.bus->write(0x2102, 0b00001011);
	snes.bus->write(0x2103, 0b10011100);
	cr_assert_eq(snes.ppu->_registers._oamadd.objPriorityActivationBit, true);
	cr_assert_eq(snes.ppu->_registers._oamadd.oamAddress, 11);
}

Test(PPU_write, bgmode_data_full)
{
	Init()
	snes.bus->write(0x2105, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgmode.bgMode, 7);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg1, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg2, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg3, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg4, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.mode1Bg3PriorityBit, true);
}

Test(PPU_write, bgmode_bgmode_5_and_bg24_on)
{
	Init()
	snes.bus->write(0x2105, 0b10100101);
	cr_assert_eq(snes.ppu->_registers._bgmode.bgMode, 5);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg1, false);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg2, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg3, false);
	cr_assert_eq(snes.ppu->_registers._bgmode.characterSizeBg4, true);
	cr_assert_eq(snes.ppu->_registers._bgmode.mode1Bg3PriorityBit, false);
}

Test(PPU_write, mosaic_data_full)
{
	Init()
	snes.bus->write(0x2106, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg1, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg2, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg3, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg4, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.pixelSize, 0xF);
}

Test(PPU_write, mosaic_affectbg23_w_1x1_size)
{
	Init()
	snes.bus->write(0x2106, 0b00000110);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg1, false);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg2, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg3, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg4, false);
	cr_assert_eq(snes.ppu->_registers._mosaic.pixelSize, 0x0);
}

Test(PPU_write, mosaic_affectbg14_w_2x2_size)
{
	Init()
	snes.bus->write(0x2106, 0b00101001);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg1, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg2, false);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg3, false);
	cr_assert_eq(snes.ppu->_registers._mosaic.affectBg4, true);
	cr_assert_eq(snes.ppu->_registers._mosaic.pixelSize, 0x2);
}

Test(PPU_write, bg1sc_data_full)
{
	Init()
	snes.bus->write(0x2107, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[0].tilemapAddress, 0b111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[0].tilemapHorizontalMirroring, true);
	cr_assert_eq(snes.ppu->_registers._bgsc[0].tilemapVerticalMirroring, true);
}

Test(PPU_write, bg2sc_data_full)
{
	Init()
	snes.bus->write(0x2108, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[1].tilemapAddress, 0b111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[1].tilemapHorizontalMirroring, true);
	cr_assert_eq(snes.ppu->_registers._bgsc[1].tilemapVerticalMirroring, true);
}

Test(PPU_write, bg3sc_data_full)
{
	Init()
	snes.bus->write(0x2109, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[2].tilemapAddress, 0b111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[2].tilemapHorizontalMirroring, true);
	cr_assert_eq(snes.ppu->_registers._bgsc[2].tilemapVerticalMirroring, true);
}

Test(PPU_write, bg4sc_data_full)
{
	Init()
	snes.bus->write(0x210A, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapAddress, 0b111111);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring, true);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring, true);
}

Test(PPU_write, bg4sc_data_null)
{
	Init()
	snes.bus->write(0x210A, 0b00000000);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapAddress, 0);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring, false);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring, false);
}

Test(PPU_write, bg4sc_horizontal_off_vertical_on_random_tilemapAdress)
{
	Init()
	snes.bus->write(0x210A, 0b11000110);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapAddress, 0b110001);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapHorizontalMirroring, false);
	cr_assert_eq(snes.ppu->_registers._bgsc[3].tilemapVerticalMirroring, true);
}

Test(PPU_write, bg12nba_data_full)
{
	Init()
	snes.bus->write(0x210B, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgnba[0].baseAddressBg1a3, 0b1111);
	cr_assert_eq(snes.ppu->_registers._bgnba[0].baseAddressBg2a4, 0b1111);
}

Test(PPU_write, bg34nba_data_full)
{
	Init()
	snes.bus->write(0x210C, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._bgnba[1].baseAddressBg1a3, 0b1111);
	cr_assert_eq(snes.ppu->_registers._bgnba[1].baseAddressBg2a4, 0b1111);
}

Test(PPU_write, bg12nba_data_random_data)
{
	Init()
	snes.bus->write(0x210B, 0b10101010);
	cr_assert_eq(snes.ppu->_registers._bgnba[0].baseAddressBg1a3, 0b1010);
	cr_assert_eq(snes.ppu->_registers._bgnba[0].baseAddressBg2a4, 0b1010);
}