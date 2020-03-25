//
// Created by anonymus-raccoon on 2/12/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

Test(STA, 8bits)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x11;
	snes.cpu->STA(0x0);
	auto data = snes.wram->_data[0];
	cr_assert_eq(data, 0x11, "The stored value should be 0x11 but it was 0x%x.", data);
}

Test(STA, 16bits)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x11AB;
	snes.cpu->STA(0x0);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	cr_assert_eq(data, 0x11AB, "The stored value should be 0x11AB but it was 0x%x.", data);
}

Test(STX, 8bits)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x11;
	snes.cpu->STX(0x0);
	auto data = snes.wram->_data[0];
	cr_assert_eq(data, 0x11, "The stored value should be 0x11 but it was 0x%x.", data);
}

Test(STX, 16bits)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0x11AB;
	snes.cpu->STX(0x0);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	cr_assert_eq(data, 0x11AB, "The stored value should be 0x11AB but it was 0x%x.", data);
}

Test(STY, 8bits)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x11;
	snes.cpu->STY(0x0);
	auto data = snes.wram->_data[0];
	cr_assert_eq(data, 0x11, "The stored value should be 0x11 but it was 0x%x.", data);
}

Test(STY, 16bits)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0x11AB;
	snes.cpu->STY(0x0);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	cr_assert_eq(data, 0x11AB, "The stored value should be 0x11AB but it was 0x%x.", data);
}

Test(STZ, 8bits)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x11;
	snes.cpu->STZ(0x0);
	auto data = snes.wram->_data[0];
	cr_assert_eq(data, 0x00, "The stored value should be 0x00 but it was 0x%x.", data);
}

Test(STZ, 16bits)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.wram->_data[0] = 0x11;
	snes.wram->_data[1] = 0x11;
	snes.cpu->STZ(0x0);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	cr_assert_eq(data, 0x00, "The stored value should be 0x00 but it was 0x%x.", data);
}

Test(LDX, 8bits)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x01;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x01, "The stored value should be 0x01 but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag register should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag register should not be set.");
}

Test(LDX, 8bitsNegative)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x11;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x11, "The stored value should be 0x11 but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag register should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag register should be set.");
}

Test(LDX, 8bitsZero)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x11;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x00, "The stored value should be 0x00 but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag register should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag register should not be set.");
}

Test(LDX, 16bits)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0xAB;
	snes.wram->_data[1] = 001;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x01AB, "The stored value should be 0x01AB but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag register should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag register should not be set.");
}

Test(LDX, 16bitsNegative)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0xAB;
	snes.wram->_data[1] = 0x11;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x11AB, "The stored value should be 0x11AB but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag register should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag register should be set.");
}

Test(LDX, 16bitsZero)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x00;
	snes.cpu->LDX(0x0);
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x0000, "The stored value should be 0x0000 but it was 0x%x.", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag register should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag register should not be set.");
}