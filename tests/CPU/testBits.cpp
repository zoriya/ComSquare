//
// Created by anonymus-raccoon on 2/20/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include <iostream>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

Test(AND, emulation)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0x00;
	pair.second.cpu->_registers.a = 0xFF;
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->AND(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0x00, "The flags should be 0x00 but it was %x", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(AND, nativeNegative)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0xF0;
	pair.second.wram->_data[1] = 0xF0;
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.a = 0xFF00;
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->AND(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0xF000, "The flags should be 0xF000 but it was %x", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
}


Test(AND, emulationTest)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0b00110011;
	pair.second.cpu->_registers.a = 0b00110111;
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->AND(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0b00110011, "The flags should be 0b00110011 but it was %x", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

