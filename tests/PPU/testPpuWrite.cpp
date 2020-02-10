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
	auto pair = Init();
	pair.first.write(0x2100, 0b11111111);
	cr_assert_eq(pair.second.ppu->_inidisp.fblank, true);
	cr_assert_eq(pair.second.ppu->_inidisp.brightness, 0xF);
}

Test(PPU_write, inidisp_data_full_zeros)
{
	auto pair = Init();
	pair.first.write(0x2100, 0b00000000);
	cr_assert_eq(pair.second.ppu->_inidisp.fblank, false);
	cr_assert_eq(pair.second.ppu->_inidisp.brightness, 0x0);
}

Test(PPU_write, inidisp_data_fBlank_on_brghtness_off)
{
	auto pair = Init();
	pair.first.write(0x2100, 0b10000000);
	cr_assert_eq(pair.second.ppu->_inidisp.fblank, true);
	cr_assert_eq(pair.second.ppu->_inidisp.brightness, 0x0);
}

Test(PPU_write, inidisp_data_fBlank_off_brghtness_max)
{
	auto pair = Init();
	pair.first.write(0x2100, 0b00001111);
	cr_assert_eq(pair.second.ppu->_inidisp.fblank, false);
	cr_assert_eq(pair.second.ppu->_inidisp.brightness, 0xF);
}

Test(PPU_write, inidisp_data_fBlank_off_brghtness_half)
{
	auto pair = Init();
	pair.first.write(0x2100, 0b00000101);
	cr_assert_eq(pair.second.ppu->_inidisp.fblank, false);
	cr_assert_eq(pair.second.ppu->_inidisp.brightness, 0x5);
}

Test(PPU_write, obsel_111_object_size_and_all_null)
{
	auto pair = Init();
	pair.first.write(0x2101, 0b11100000);
	cr_assert_eq(pair.second.ppu->_obsel.objectSize, 0b111);
	cr_assert_eq(pair.second.ppu->_obsel.nameSelect, 0b00);
	cr_assert_eq(pair.second.ppu->_obsel.baseSelect, 0b000);
}

Test(PPU_write, obsel_data_full)
{
	auto pair = Init();
	pair.first.write(0x2101, 0b11111111);
	cr_assert_eq(pair.second.ppu->_obsel.objectSize, 0b111);
	cr_assert_eq(pair.second.ppu->_obsel.nameSelect, 0b11);
	cr_assert_eq(pair.second.ppu->_obsel.baseSelect, 0b111);
}

Test(PPU_write, obsel_data_full_nameselect)
{
	auto pair = Init();
	pair.first.write(0x2101, 0b00011000);
	cr_assert_eq(pair.second.ppu->_obsel.objectSize, 0b000);
	cr_assert_eq(pair.second.ppu->_obsel.nameSelect, 0b11);
	cr_assert_eq(pair.second.ppu->_obsel.baseSelect, 0b000);
}

Test(PPU_write, obsel_data_full_baseselect)
{
	auto pair = Init();
	pair.first.write(0x2101, 0b00000111);
	cr_assert_eq(pair.second.ppu->_obsel.objectSize, 0b000);
	cr_assert_eq(pair.second.ppu->_obsel.nameSelect, 0b00);
	cr_assert_eq(pair.second.ppu->_obsel.baseSelect, 0b111);
}

Test(PPU_write, oamaddl_data_full)
{
	auto pair = Init();
	pair.first.write(0x2102, 0b11111111);
	cr_assert_eq(pair.second.ppu->_oamadd.oamAddress, 0b011111111);
}

Test(PPU_write, oamaddh_data_full)
{
	auto pair = Init();
	pair.first.write(0x2103, 0b11111111);
	cr_assert_eq(pair.second.ppu->_oamadd.objPriorityActivationBit, true);
	cr_assert_eq(pair.second.ppu->_oamadd.oamAddress, 0b100000000);
}

Test(PPU_write, oamaddlh_data_full)
{
	auto pair = Init();
	pair.first.write(0x2102, 0b11111111);
	pair.first.write(0x2103, 0b11111111);
	cr_assert_eq(pair.second.ppu->_oamadd.objPriorityActivationBit, true);
	cr_assert_eq(pair.second.ppu->_oamadd.oamAddress, 0b111111111);
}

Test(PPU_write, oamaddlh_data_full_priorityBit_off)
{
	auto pair = Init();
	pair.first.write(0x2102, 0b11111111);
	pair.first.write(0x2103, 0b01111111);
	cr_assert_eq(pair.second.ppu->_oamadd.objPriorityActivationBit, false);
	cr_assert_eq(pair.second.ppu->_oamadd.oamAddress, 0b111111111);
}

Test(PPU_write, oamaddlh_oamAdress_11_priorityBit_on)
{
	auto pair = Init();
	pair.first.write(0x2102, 0b00001011);
	pair.first.write(0x2103, 0b10011100);
	cr_assert_eq(pair.second.ppu->_oamadd.objPriorityActivationBit, true);
	cr_assert_eq(pair.second.ppu->_oamadd.oamAddress, 11);
}