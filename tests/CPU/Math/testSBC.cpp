//
// Created by anonymus-raccoon on 27/02/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

Test(SBC, removingOne)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.c = true;
	pair.second.cpu->_registers.a = 0x1;
	pair.second.wram->_data[0] = 0x1;
	pair.second.cpu->SBC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(SBC, legitOverflowWithCarry)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.a = 0x1;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.p.c = true;
	pair.second.wram->_data[0] = 0x03;
	pair.second.wram->_data[1] = 0x20;
	pair.second.cpu->SBC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0xDFFE, "The accumulator's value should be 0xDFFE but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(SBC, overflowWithCarry)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.a = 0x1;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.p.c = true;
	pair.second.wram->_data[0] = 0x03;
	pair.second.wram->_data[1] = 0x20;
	pair.second.cpu->SBC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0xDFFE, "The accumulator's value should be 0xDFFE but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should be not set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(SBC, overflowEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.a = 0x1;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.p.c = false;
	pair.second.wram->_data[0] = 0x02;
	pair.second.cpu->SBC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0xFE, "The accumulator's value should be 0xFE but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should be not set.");
}


Test(SBC, decimal)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.a = 0x1;
	pair.second.cpu->_registers.p.d = true;
	pair.second.cpu->_registers.p.m = false;
	pair.second.wram->_data[0] = 0x03;
	pair.second.wram->_data[1] = 0x20;
	pair.second.cpu->SBC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.a, 0x7998, "The accumulator's value should be 0x7998 but it was 0x%x.", pair.second.cpu->_registers.a);
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flags should be not set.");
}
