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
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.a = 0x1;
	snes.wram->_data[0] = 0x1;
	snes.cpu->SBC(0x0);
	cr_assert_eq(snes.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(SBC, legitOverflowWithCarry)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.wram->_data[0] = 0x03;
	snes.wram->_data[1] = 0x20;
	snes.cpu->SBC(0x0);
	cr_assert_eq(snes.cpu->_registers.a, 0xDFFE, "The accumulator's value should be 0xDFFE but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(SBC, overflowWithCarry)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.wram->_data[0] = 0x03;
	snes.wram->_data[1] = 0x20;
	snes.cpu->SBC(0x0);
	cr_assert_eq(snes.cpu->_registers.a, 0xDFFE, "The accumulator's value should be 0xDFFE but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should be not set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(SBC, overflowEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = false;
	snes.wram->_data[0] = 0x02;
	snes.cpu->SBC(0x0);
	cr_assert_eq(snes.cpu->_registers.a, 0xFE, "The accumulator's value should be 0xFE but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should be not set.");
}


//Test(SBC, decimal)
//{
//	Init()
//	snes.cpu->_isEmulationMode = true;
//	snes.cpu->_registers.a = 0x1;
//	snes.cpu->_registers.p.d = true;
//	snes.cpu->_registers.p.m = false;
//	snes.wram->_data[0] = 0x03;
//	snes.wram->_data[1] = 0x20;
//	snes.cpu->SBC(0x0);
//	cr_assert_eq(snes.cpu->_registers.a, 0x7998, "The accumulator's value should be 0x7998 but it was 0x%x.", snes.cpu->_registers.a);
//	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
//	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
//	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
//	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should be not set.");
//}
