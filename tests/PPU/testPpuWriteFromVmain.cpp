//
// Created by cbihan on 2/11/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"
using namespace ComSquare;

Test(PPU_write_2, vmain_data_full)
{
	auto pair = Init();
	pair.first->write(0x2115, 0b11111111);
	cr_assert_eq(pair.second.ppu->_vmain.incrementMode, true);
	cr_assert_eq(pair.second.ppu->_vmain.addressRemapping, 0b11);
	cr_assert_eq(pair.second.ppu->_vmain.incrementAmount, 0b11);
}

Test(PPU_write_2, vmain_incrementmode_off_false_else_full)
{
	auto pair = Init();
	pair.first->write(0x2115, 0b01111111);
	cr_assert_eq(pair.second.ppu->_vmain.incrementMode, false);
	cr_assert_eq(pair.second.ppu->_vmain.addressRemapping, 0b11);
	cr_assert_eq(pair.second.ppu->_vmain.incrementAmount, 0b11);
}

Test(PPU_write_2, vmain_addressremaping_null_else_full)
{
	auto pair = Init();
	pair.first->write(0x2115, 0b11110011);
	cr_assert_eq(pair.second.ppu->_vmain.incrementMode, true);
	cr_assert_eq(pair.second.ppu->_vmain.addressRemapping, 0b00);
	cr_assert_eq(pair.second.ppu->_vmain.incrementAmount, 0b11);
}

Test(PPU_write_2, vmain_incrementamount_null_else_full)
{
	auto pair = Init();
	pair.first->write(0x2115, 0b11111100);
	cr_assert_eq(pair.second.ppu->_vmain.incrementMode, true);
	cr_assert_eq(pair.second.ppu->_vmain.addressRemapping, 0b11);
	cr_assert_eq(pair.second.ppu->_vmain.incrementAmount, 0b00);
}

Test(PPU_write_2, vmadd_full_data)
{
	auto pair = Init();
	pair.first->write(0x2116, 0b11111111);
	pair.first->write(0x2117, 0b11111111);
	cr_assert_eq(pair.second.ppu->_vmadd.vmadd, 0b1111111111111111);
}

Test(PPU_write_2, vmadd_full_high_byte_null)
{
	auto pair = Init();
	pair.first->write(0x2116, 0b11111111);
	pair.first->write(0x2117, 0b00000000);
	cr_assert_eq(pair.second.ppu->_vmadd.vmadd, 0b0000000011111111);
}

Test(PPU_write_2, vmdata_full_data)
{
	auto pair = Init();
	pair.first->write(0x2118, 0b11111111);
	pair.first->write(0x2119, 0b11111111);
	cr_assert_eq(pair.second.ppu->_vmdata.vmdata, 0b1111111111111111);
}

Test(PPU_write_2, vmdata_full_high_byte_null)
{
	auto pair = Init();
	pair.first->write(0x2118, 0b11111111);
	pair.first->write(0x2119, 0b00000000);
	cr_assert_eq(pair.second.ppu->_vmdata.vmdata, 0b0000000011111111);
}

Test(PPU_write_2, cgadd_full_high_byte_null)
{
	auto pair = Init();
	pair.first->write(0x2121, 0b11111111);
	cr_assert_eq(pair.second.ppu->_cgadd, 0b11111111);
	cr_assert_eq(pair.second.ppu->_isLowByte, true);
}

Test(PPU_write_2, cgdata_data_full)
{
	auto pair = Init();
	pair.first->write(0x2121, 0x0);
	pair.first->write(0x2122, 0b11111111);
	cr_assert_eq(pair.second.ppu->_cgdata.cgdatal, 0b11111111);
	cr_assert_eq(pair.second.ppu->_isLowByte, false);
	int address = pair.second.ppu->_cgadd;
	pair.first->write(0x2122, 0b11111000);
	cr_assert_eq(pair.second.ppu->_cgdata.cgdatah, 0b11111000);
	cr_assert_eq(pair.second.ppu->_isLowByte, true);
	cr_assert_eq(pair.second.ppu->_cgadd, address + 1);
}

Test(PPU_write_2, m7sel_data_full)
{
	auto pair = Init();
	pair.first->write(0x211A, 0b11111111);
	cr_assert_eq(pair.second.ppu->_m7sel.playingFieldSize, true);
	cr_assert_eq(pair.second.ppu->_m7sel.emptySpaceFill, true);
	cr_assert_eq(pair.second.ppu->_m7sel.horizontalMirroring, true);
	cr_assert_eq(pair.second.ppu->_m7sel.verticalMirroring, true);
}

Test(PPU_write_2, m7sel_data_actual)
{
	auto pair = Init();
	pair.first->write(0x211A, 0b01111101);
	cr_assert_eq(pair.second.ppu->_m7sel.playingFieldSize, false);
	cr_assert_eq(pair.second.ppu->_m7sel.emptySpaceFill, true);
	cr_assert_eq(pair.second.ppu->_m7sel.horizontalMirroring, true);
	cr_assert_eq(pair.second.ppu->_m7sel.verticalMirroring, false);
}

Test(PPU_write_2, w12sel_data_full)
{
	auto pair = Init();
	pair.first->write(0x2123, 0b11111111);
	cr_assert_eq(pair.second.ppu->_wsel[0].window1InversionForBg1Bg2Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].enableWindow1ForBg1Bg2Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].window2InversionForBg1Bg3Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].enableWindow2ForBg1Bg3Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].window1InversionForBg2Bg4Color, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].enableWindow1ForBg2Bg4Color, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].window2InversionForBg2Bg4Color, true);
	cr_assert_eq(pair.second.ppu->_wsel[0].enableWindow2ForBg2Bg4Color, true);
}

Test(PPU_write_2, w34sel_data_full)
{
	auto pair = Init();
	pair.first->write(0x2124, 0b10101010);
	cr_assert_eq(pair.second.ppu->_wsel[1].window1InversionForBg1Bg2Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[1].enableWindow1ForBg1Bg2Obj, false);
	cr_assert_eq(pair.second.ppu->_wsel[1].window2InversionForBg1Bg3Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[1].enableWindow2ForBg1Bg3Obj, false);
	cr_assert_eq(pair.second.ppu->_wsel[1].window1InversionForBg2Bg4Color, true);
	cr_assert_eq(pair.second.ppu->_wsel[1].enableWindow1ForBg2Bg4Color, false);
	cr_assert_eq(pair.second.ppu->_wsel[1].window2InversionForBg2Bg4Color, true);
	cr_assert_eq(pair.second.ppu->_wsel[1].enableWindow2ForBg2Bg4Color, false);
}

Test(PPU_write_2, wobjsel_data_full)
{
	auto pair = Init();
	pair.first->write(0x2125, 0b10110001);
	cr_assert_eq(pair.second.ppu->_wsel[2].window1InversionForBg1Bg2Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[2].enableWindow1ForBg1Bg2Obj, false);
	cr_assert_eq(pair.second.ppu->_wsel[2].window2InversionForBg1Bg3Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[2].enableWindow2ForBg1Bg3Obj, true);
	cr_assert_eq(pair.second.ppu->_wsel[2].window1InversionForBg2Bg4Color, false);
	cr_assert_eq(pair.second.ppu->_wsel[2].enableWindow1ForBg2Bg4Color, false);
	cr_assert_eq(pair.second.ppu->_wsel[2].window2InversionForBg2Bg4Color, false);
	cr_assert_eq(pair.second.ppu->_wsel[2].enableWindow2ForBg2Bg4Color, true);
}

Test(PPU_write_2, wbglog_data_full)
{
	auto pair = Init();
	pair.first->write(0x212A, 0b10110001);
	cr_assert_eq(pair.second.ppu->_wbglog.maskLogicBg1, 0b10);
	cr_assert_eq(pair.second.ppu->_wbglog.maskLogicBg2, 0b11);
	cr_assert_eq(pair.second.ppu->_wbglog.maskLogicBg3, 0b00);
	cr_assert_eq(pair.second.ppu->_wbglog.maskLogicBg4, 0b01);
}

Test(PPU_write_2, wobjlog_data_full)
{
	auto pair = Init();
	pair.first->write(0x212B, 0b10110001);
	cr_assert_eq(pair.second.ppu->_wobjlog.maskLogicObj, 0b01);
	cr_assert_eq(pair.second.ppu->_wobjlog.maskLogicColor, 0b00);
}

Test(PPU_write_2, tm_data_full)
{
	auto pair = Init();
	pair.first->write(0x212C, 0b10110001);
	cr_assert_eq(pair.second.ppu->_t[0].enableWindowDisplayBg1, true);
	cr_assert_eq(pair.second.ppu->_t[0].enableWindowDisplayBg2, false);
	cr_assert_eq(pair.second.ppu->_t[0].enableWindowDisplayBg3, false);
	cr_assert_eq(pair.second.ppu->_t[0].enableWindowDisplayBg4, false);
	cr_assert_eq(pair.second.ppu->_t[0].enableWindowDisplayObj, true);
}

Test(PPU_write_2, ts_data_full)
{
	auto pair = Init();
	pair.first->write(0x212D, 0b10101110);
	cr_assert_eq(pair.second.ppu->_t[1].enableWindowDisplayBg1, false);
	cr_assert_eq(pair.second.ppu->_t[1].enableWindowDisplayBg2, true);
	cr_assert_eq(pair.second.ppu->_t[1].enableWindowDisplayBg3, true);
	cr_assert_eq(pair.second.ppu->_t[1].enableWindowDisplayBg4, true);
	cr_assert_eq(pair.second.ppu->_t[1].enableWindowDisplayObj, false);
}

Test(PPU_write_2, tmw_data_full)
{
	auto pair = Init();
	pair.first->write(0x212E, 0b10101110);
	cr_assert_eq(pair.second.ppu->_tw[0].enableWindowMaskingBg1, false);
	cr_assert_eq(pair.second.ppu->_tw[0].enableWindowMaskingBg2, true);
	cr_assert_eq(pair.second.ppu->_tw[0].enableWindowMaskingBg3, true);
	cr_assert_eq(pair.second.ppu->_tw[0].enableWindowMaskingBg4, true);
	cr_assert_eq(pair.second.ppu->_tw[0].enableWindowMaskingObj, false);
}

Test(PPU_write_2, tsw_data_full)
{
	auto pair = Init();
	pair.first->write(0x212F, 0b10100011);
	cr_assert_eq(pair.second.ppu->_tw[1].enableWindowMaskingBg1, true);
	cr_assert_eq(pair.second.ppu->_tw[1].enableWindowMaskingBg2, true);
	cr_assert_eq(pair.second.ppu->_tw[1].enableWindowMaskingBg3, false);
	cr_assert_eq(pair.second.ppu->_tw[1].enableWindowMaskingBg4, false);
	cr_assert_eq(pair.second.ppu->_tw[1].enableWindowMaskingObj, false);
}

Test(PPU_write_2, cgwsel_data_full)
{
	auto pair = Init();
	pair.first->write(0x2130, 0b10111001);
	cr_assert_eq(pair.second.ppu->_cgwsel.clipColorToBlackBeforeMath, 0b10);
	cr_assert_eq(pair.second.ppu->_cgwsel.preventColorMath, 0b11);
	cr_assert_eq(pair.second.ppu->_cgwsel.addSubscreen, false);
	cr_assert_eq(pair.second.ppu->_cgwsel.directColorMode, true);
}

Test(PPU_write_2, cgadsub_data_full)
{
	auto pair = Init();
	pair.first->write(0x2131, 0b10111001);
	cr_assert_eq(pair.second.ppu->_cgadsub.addSubtractSelect, true);
	cr_assert_eq(pair.second.ppu->_cgadsub.halfColorMath, false);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathBackdrop, true);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathObj, true);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathBg4, true);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathBg3, false);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathBg2, false);
	cr_assert_eq(pair.second.ppu->_cgadsub.enableColorMathBg1, true);
}

Test(PPU_write_2, coldata_data_full)
{
	auto pair = Init();
	pair.first->write(0x2132, 0b10111001);
	cr_assert_eq(pair.second.ppu->_coldata.blue, true);
	cr_assert_eq(pair.second.ppu->_coldata.green, false);
	cr_assert_eq(pair.second.ppu->_coldata.red, true);
	cr_assert_eq(pair.second.ppu->_coldata.colorIntensity, 0b11001);
}

Test(PPU_write_2, setini_data_full)
{
	auto pair = Init();
	pair.first->write(0x2133, 0b10111001);
	cr_assert_eq(pair.second.ppu->_setini.externalSync, true);
	cr_assert_eq(pair.second.ppu->_setini.mode7ExtBg, false);
	cr_assert_eq(pair.second.ppu->_setini.enablePseudoHiresMode, true);
	cr_assert_eq(pair.second.ppu->_setini.overscanMode, false);
	cr_assert_eq(pair.second.ppu->_setini.objInterlace, false);
	cr_assert_eq(pair.second.ppu->_setini.screenInterlace, true);
}