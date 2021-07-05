//
// Created by cbihan on 2/11/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"
using namespace ComSquare;

TEST_CASE("vmain_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2115, 0b11111111);
	REQUIRE(snes.ppu._registers._vmain.incrementMode == true);
	REQUIRE(snes.ppu._registers._vmain.addressRemapping == 0b11);
	REQUIRE(snes.ppu._registers._vmain.incrementAmount == 0b11);
}

TEST_CASE("vmain_incrementmode_off_false_else_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2115, 0b01111111);
	REQUIRE(snes.ppu._registers._vmain.incrementMode == false);
	REQUIRE(snes.ppu._registers._vmain.addressRemapping == 0b11);
	REQUIRE(snes.ppu._registers._vmain.incrementAmount == 0b11);
}

TEST_CASE("vmain_addressremaping_null_else_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2115, 0b11110011);
	REQUIRE(snes.ppu._registers._vmain.incrementMode == true);
	REQUIRE(snes.ppu._registers._vmain.addressRemapping == 0b00);
	REQUIRE(snes.ppu._registers._vmain.incrementAmount == 0b11);
}

TEST_CASE("vmain_incrementamount_null_else_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2115, 0b11111100);
	REQUIRE(snes.ppu._registers._vmain.incrementMode == true);
	REQUIRE(snes.ppu._registers._vmain.addressRemapping == 0b11);
	REQUIRE(snes.ppu._registers._vmain.incrementAmount == 0b00);
}

TEST_CASE("vmadd_full_data PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2116, 0b11111111);
	snes.bus.write(0x2117, 0b11111111);
	REQUIRE(snes.ppu._registers._vmadd.vmadd == 0b1111111111111111);
}

TEST_CASE("vmadd_full_data_check_ram PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2115, 0b10000000);
	snes.bus.write(0x2116, 2);
	snes.bus.write(0x2117, 0);
	snes.bus.write(0x2118, 0xFF);
	snes.bus.write(0x2119, 0xFF);
	REQUIRE(snes.ppu._registers._vmadd.vmadd == 3);
	REQUIRE(snes.ppu.vram.read(4) == 0xFF);
	REQUIRE(snes.ppu.vram.read(5) == 0xF);
}

TEST_CASE("vmadd_full_high_byte_null PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2116, 0b11111111);
	snes.bus.write(0x2117, 0b00000000);
	REQUIRE(snes.ppu._registers._vmadd.vmadd == 0b0000000011111111);
}

TEST_CASE("vmdata_full_data PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2118, 0b11111111);
	snes.bus.write(0x2119, 0b11111111);
	REQUIRE(snes.ppu._registers._vmdata.vmdata == 0b1111111111111111);
}

TEST_CASE("vmdata_full_high_byte_null PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2118, 0b11111111);
	snes.bus.write(0x2119, 0b00000000);
	REQUIRE(snes.ppu._registers._vmdata.vmdata == 0b0000000011111111);
}

TEST_CASE("cgadd_full_high_byte_null PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2121, 0b11111111);
	REQUIRE(snes.ppu._registers._cgadd == 0b11111111);
	REQUIRE(snes.ppu._registers._isLowByte == true);
}

TEST_CASE("cgdata_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2121, 0x0);
	snes.bus.write(0x2122, 0b11111111);
	REQUIRE(snes.ppu._registers._cgdata.cgdatal == 0b11111111);
	REQUIRE(snes.ppu._registers._isLowByte == false);
	int address = snes.ppu._registers._cgadd;
	snes.bus.write(0x2122, 0b11111000);
	REQUIRE(snes.ppu._registers._cgdata.cgdatah == 0b11111000);
	REQUIRE(snes.ppu._registers._isLowByte == true);
	REQUIRE(snes.ppu._registers._cgadd == address + 2);
}

TEST_CASE("m7sel_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x211A, 0b11111111);
	REQUIRE(snes.ppu._registers._m7sel.playingFieldSize == true);
	REQUIRE(snes.ppu._registers._m7sel.emptySpaceFill == true);
	REQUIRE(snes.ppu._registers._m7sel.horizontalMirroring == true);
	REQUIRE(snes.ppu._registers._m7sel.verticalMirroring == true);
}

TEST_CASE("m7sel_data_actual PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x211A, 0b01111101);
	REQUIRE(snes.ppu._registers._m7sel.playingFieldSize == false);
	REQUIRE(snes.ppu._registers._m7sel.emptySpaceFill == true);
	REQUIRE(snes.ppu._registers._m7sel.horizontalMirroring == true);
	REQUIRE(snes.ppu._registers._m7sel.verticalMirroring == false);
}

TEST_CASE("w12sel_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2123, 0b11111111);
	REQUIRE(snes.ppu._registers._wsel[0].window1InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[0].enableWindow1ForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[0].window2InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[0].enableWindow2ForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[0].window1InversionForBg2Bg4Color == true);
	REQUIRE(snes.ppu._registers._wsel[0].enableWindow1ForBg2Bg4Color == true);
	REQUIRE(snes.ppu._registers._wsel[0].window2InversionForBg2Bg4Color == true);
	REQUIRE(snes.ppu._registers._wsel[0].enableWindow2ForBg2Bg4Color == true);
}

TEST_CASE("w34sel_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2124, 0b10101010);
	REQUIRE(snes.ppu._registers._wsel[1].window1InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[1].enableWindow1ForBg1Bg3Obj == false);
	REQUIRE(snes.ppu._registers._wsel[1].window2InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[1].enableWindow2ForBg1Bg3Obj == false);
	REQUIRE(snes.ppu._registers._wsel[1].window1InversionForBg2Bg4Color == true);
	REQUIRE(snes.ppu._registers._wsel[1].enableWindow1ForBg2Bg4Color == false);
	REQUIRE(snes.ppu._registers._wsel[1].window2InversionForBg2Bg4Color == true);
	REQUIRE(snes.ppu._registers._wsel[1].enableWindow2ForBg2Bg4Color == false);
}

TEST_CASE("wobjsel_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2125, 0b10110001);
	REQUIRE(snes.ppu._registers._wsel[2].window1InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[2].enableWindow1ForBg1Bg3Obj == false);
	REQUIRE(snes.ppu._registers._wsel[2].window2InversionForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[2].enableWindow2ForBg1Bg3Obj == true);
	REQUIRE(snes.ppu._registers._wsel[2].window1InversionForBg2Bg4Color == false);
	REQUIRE(snes.ppu._registers._wsel[2].enableWindow1ForBg2Bg4Color == false);
	REQUIRE(snes.ppu._registers._wsel[2].window2InversionForBg2Bg4Color == false);
	REQUIRE(snes.ppu._registers._wsel[2].enableWindow2ForBg2Bg4Color == true);
}

TEST_CASE("wbglog_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212A, 0b10110001);
	REQUIRE(snes.ppu._registers._wbglog.maskLogicBg1 == 0b10);
	REQUIRE(snes.ppu._registers._wbglog.maskLogicBg2 == 0b11);
	REQUIRE(snes.ppu._registers._wbglog.maskLogicBg3 == 0b00);
	REQUIRE(snes.ppu._registers._wbglog.maskLogicBg4 == 0b01);
}

TEST_CASE("wobjlog_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212B, 0b10110001);
	REQUIRE(snes.ppu._registers._wobjlog.maskLogicObj == 0b01);
	REQUIRE(snes.ppu._registers._wobjlog.maskLogicColor == 0b00);
}

TEST_CASE("tm_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212C, 0b10110001);
	REQUIRE(snes.ppu._registers._t[0].enableWindowDisplayBg1 == true);
	REQUIRE(snes.ppu._registers._t[0].enableWindowDisplayBg2 == false);
	REQUIRE(snes.ppu._registers._t[0].enableWindowDisplayBg3 == false);
	REQUIRE(snes.ppu._registers._t[0].enableWindowDisplayBg4 == false);
	REQUIRE(snes.ppu._registers._t[0].enableWindowDisplayObj == true);
}

TEST_CASE("ts_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212D, 0b10101110);
	REQUIRE(snes.ppu._registers._t[1].enableWindowDisplayBg1 == false);
	REQUIRE(snes.ppu._registers._t[1].enableWindowDisplayBg2 == true);
	REQUIRE(snes.ppu._registers._t[1].enableWindowDisplayBg3 == true);
	REQUIRE(snes.ppu._registers._t[1].enableWindowDisplayBg4 == true);
	REQUIRE(snes.ppu._registers._t[1].enableWindowDisplayObj == false);
}

TEST_CASE("tmw_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212E, 0b10101110);
	REQUIRE(snes.ppu._registers._tw[0].enableWindowMaskingBg1 == false);
	REQUIRE(snes.ppu._registers._tw[0].enableWindowMaskingBg2 == true);
	REQUIRE(snes.ppu._registers._tw[0].enableWindowMaskingBg3 == true);
	REQUIRE(snes.ppu._registers._tw[0].enableWindowMaskingBg4 == true);
	REQUIRE(snes.ppu._registers._tw[0].enableWindowMaskingObj == false);
}

TEST_CASE("tsw_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x212F, 0b10100011);
	REQUIRE(snes.ppu._registers._tw[1].enableWindowMaskingBg1 == true);
	REQUIRE(snes.ppu._registers._tw[1].enableWindowMaskingBg2 == true);
	REQUIRE(snes.ppu._registers._tw[1].enableWindowMaskingBg3 == false);
	REQUIRE(snes.ppu._registers._tw[1].enableWindowMaskingBg4 == false);
	REQUIRE(snes.ppu._registers._tw[1].enableWindowMaskingObj == false);
}

TEST_CASE("cgwsel_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2130, 0b10111001);
	REQUIRE(snes.ppu._registers._cgwsel.clipColorToBlackBeforeMath == 0b10);
	REQUIRE(snes.ppu._registers._cgwsel.preventColorMath == 0b11);
	REQUIRE(snes.ppu._registers._cgwsel.addSubscreen == false);
	REQUIRE(snes.ppu._registers._cgwsel.directColorMode == true);
}

TEST_CASE("cgadsub_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2131, 0b10111001);
	REQUIRE(snes.ppu._registers._cgadsub.addSubtractSelect == true);
	REQUIRE(snes.ppu._registers._cgadsub.halfColorMath == false);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathBackdrop == true);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathObj == true);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathBg4 == true);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathBg3 == false);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathBg2 == false);
	REQUIRE(snes.ppu._registers._cgadsub.enableColorMathBg1 == true);
}

TEST_CASE("coldata_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2132, 0b10111001);
	REQUIRE(snes.ppu._registers._coldata.blue == true);
	REQUIRE(snes.ppu._registers._coldata.green == false);
	REQUIRE(snes.ppu._registers._coldata.red == true);
	REQUIRE(snes.ppu._registers._coldata.colorIntensity == 0b11001);
}

TEST_CASE("setini_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x2133, 0b10111001);
	REQUIRE(snes.ppu._registers._setini.externalSync == true);
	REQUIRE(snes.ppu._registers._setini.mode7ExtBg == false);
	REQUIRE(snes.ppu._registers._setini.enablePseudoHiresMode == true);
	REQUIRE(snes.ppu._registers._setini.overscanMode == false);
	REQUIRE(snes.ppu._registers._setini.objInterlace == false);
	REQUIRE(snes.ppu._registers._setini.screenInterlace == true);
}

TEST_CASE("m7a_data_full PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x211B, 0b10111001);
	REQUIRE(snes.ppu._registers._m7[0].m7l == 0b10111001);
}

TEST_CASE("m7c_data_low_and_high_byte PPU_write_2", "[PPU_write_2]")
{
	Init()
	snes.bus.write(0x211D, 0b10111001);
	snes.bus.write(0x211D, 0b11111111);
	REQUIRE(snes.ppu._registers._m7[2].m7 == 0b1011100111111111);
}