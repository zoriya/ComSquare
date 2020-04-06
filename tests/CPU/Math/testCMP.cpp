//
// Created by anonymus-raccoon on 4/6/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

Test(CMP, underflow)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0;
	snes.wram->_data[0] = 0x1;
	snes.cpu->CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(CMP, zero)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x5;
	snes.wram->_data[0] = 0x5;
	snes.cpu->CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(CMP, nativeModeZero)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x5000;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x50;
	snes.cpu->CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(CMP, nativeModeNothing)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8000;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x50;
	snes.cpu->CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(CMP, negative)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0xB000;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x10;
	snes.cpu->CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

