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
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.x = 0xABCD;
	pair.second.cpu->_registers.a = 0xFEDC;
	pair.second.cpu->TAX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0xFEDC, "The flags should be 0xFEDC but it was %x", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAX, 16bitsTo8Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.x = 0xFEDC;
	pair.second.cpu->_registers.a = 0xAB00;
	pair.second.cpu->TAX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0xFE00, "The flags should be 0xFE00 but it was %x", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TAX, 8bitsTo16Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.x = 0xFEDC;
	pair.second.cpu->_registers.a = 0xAB;
	pair.second.cpu->TAX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0x00AB, "The flags should be 0x00AB but it was %x", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAX, 8bitsTo8Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.x = 0xFE;
	pair.second.cpu->_registers.a = 0xAB;
	pair.second.cpu->TAX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0xAB, "The flags should be 0xAB but it was %x", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should be not set.");
}


Test(TAY, 16bitsTo16Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.y = 0xABCD;
	pair.second.cpu->_registers.a = 0xFEDC;
	pair.second.cpu->TAY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0xFEDC, "The y register should be 0xFEDC but it was %x", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAY, 16bitsTo8Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.y = 0xFEDC;
	pair.second.cpu->_registers.a = 0xAB00;
	pair.second.cpu->TAY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0xFE00, "The y register should be 0xFE00 but it was %x", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TAY, 8bitsTo16Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.y = 0xFEDC;
	pair.second.cpu->_registers.a = 0xAB;
	pair.second.cpu->TAY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0x00AB, "The y register should be 0x00AB but it was %x", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TAY, 8bitsTo8Bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.y = 0xFE;
	pair.second.cpu->_registers.a = 0xAB;
	pair.second.cpu->TAY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0xAB, "The y register should be 0xAB but it was %x", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXS, 16bitsIndex)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.x = 0xABCD;
	pair.second.cpu->TXS();
	cr_assert_eq(pair.second.cpu->_registers.s, 0xABCD, "The stack pointer should be 0xABCD but it was %x", pair.second.cpu->_registers.s);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXS, 8bitsIndex)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.x = 0xABCD;
	pair.second.cpu->TXS();
	cr_assert_eq(pair.second.cpu->_registers.s, 0x00CD, "The stack pointer should be 0x00CD but it was %x", pair.second.cpu->_registers.s);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should be not set.");
}