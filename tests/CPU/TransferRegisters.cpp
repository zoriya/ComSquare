//
// Created by anonymus-raccoon on 2/28/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

Test(TAX, 16bitsTo16Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.x = 0xABCD;
	snes.cpu->_registers.a = 0xFEDC;
	snes.cpu->TAX();
	cr_assert_eq(snes.cpu->_registers.x, 0xFEDC, "The flags should be 0xFEDC but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAX, 16bitsTo8Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.x = 0xFEDC;
	snes.cpu->_registers.a = 0xAB00;
	snes.cpu->TAX();
	cr_assert_eq(snes.cpu->_registers.x, 0xFE00, "The flags should be 0xFE00 but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TAX, 8bitsTo16Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.x = 0xFEDC;
	snes.cpu->_registers.a = 0xAB;
	snes.cpu->TAX();
	cr_assert_eq(snes.cpu->_registers.x, 0x00AB, "The flags should be 0x00AB but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAX, 8bitsTo8Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.x = 0xFE;
	snes.cpu->_registers.a = 0xAB;
	snes.cpu->TAX();
	cr_assert_eq(snes.cpu->_registers.x, 0xAB, "The flags should be 0xAB but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}


Test(TAY, 16bitsTo16Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.y = 0xABCD;
	snes.cpu->_registers.a = 0xFEDC;
	snes.cpu->TAY();
	cr_assert_eq(snes.cpu->_registers.y, 0xFEDC, "The y register should be 0xFEDC but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAY, 16bitsTo8Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.y = 0xFEDC;
	snes.cpu->_registers.a = 0xAB00;
	snes.cpu->TAY();
	cr_assert_eq(snes.cpu->_registers.y, 0xFE00, "The y register should be 0xFE00 but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TAY, 8bitsTo16Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.y = 0xFEDC;
	snes.cpu->_registers.a = 0xAB;
	snes.cpu->TAY();
	cr_assert_eq(snes.cpu->_registers.y, 0x00AB, "The y register should be 0x00AB but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAY, 8bitsTo8Bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.y = 0xFE;
	snes.cpu->_registers.a = 0xAB;
	snes.cpu->TAY();
	cr_assert_eq(snes.cpu->_registers.y, 0xAB, "The y register should be 0xAB but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXS, 16bitsIndex)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xABCD;
	snes.cpu->TXS();
	cr_assert_eq(snes.cpu->_registers.s, 0xABCD, "The stack pointer should be 0xABCD but it was %x", snes.cpu->_registers.s);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXS, 8bitsIndex)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0xABCD;
	snes.cpu->TXS();
	cr_assert_eq(snes.cpu->_registers.s, 0x00CD, "The stack pointer should be 0x00CD but it was %x", snes.cpu->_registers.s);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}