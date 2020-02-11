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