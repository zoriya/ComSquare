//
// Created by anonymus-raccoon on 2/11/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;

Test(ADC, addingOne)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.a = 0;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 1, "The accumulator's value should be 0x1 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, addingOneEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.a = 0;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 1, "The accumulator's value should be 0x1 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, overflow)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.a = 0xFFFF;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(ADC, overflowEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.a = 0xFF;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flags should be set.");
}


Test(ADC, signedOverflow)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.a = 0x0FFF;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0x1000, "The accumulator's value should be 0xF000 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, true, "The overflow flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, signedOverflowEmulated)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.a = 0x000F;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->ADC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0x0010, "The accumulator's value should be 0xF000 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, true, "The overflow flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}