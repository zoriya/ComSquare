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
	Init()
	snes.wram->_data[0] = 0x00;
	snes.cpu->_registers.a = 0xFF;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x00, "The flags should be 0x00 but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(AND, nativeNegative)
{
	Init()
	snes.wram->_data[0] = 0xF0;
	snes.wram->_data[1] = 0xF0;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0xFF00;
	snes.cpu->_isEmulationMode = false;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xF000, "The flags should be 0xF000 but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
}

Test(AND, emulationTest)
{
	Init()
	snes.wram->_data[0] = 0b00110011;
	snes.cpu->_registers.a = 0b00110111;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0b00110011, "The flags should be 0b00110011 but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(TSB, emulationTest)
{
	Init()
	snes.wram->_data[0] = 0b00110011;
	snes.cpu->_registers.a = 0b00110111;
	snes.cpu->_registers.p.m = true;
	snes.cpu->TSB(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.wram->_data[0], 0b00110111, "The data in ram should be 0b00110111 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(TSB, nativeTest)
{
	Init()
	snes.wram->_data[0] = 0xF0;
	snes.wram->_data[1] = 0x0F;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.m = false;
	snes.cpu->TSB(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.wram->_data[0], 0xF8, "The first data in ram should be 0xF8 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.wram->_data[1], 0x8F, "The second data in ram should be 0x8F but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
}

Test(BIT, immediate)
{
	Init()
	snes.wram->_data[0] = 0xFF;
	snes.wram->_data[1] = 0x00;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.p.n = false;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::ImmediateForA);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(BIT, immediateZero)
{
	Init()
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0xFF;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x0008;
	snes.cpu->_registers.p.v = true;
	snes.cpu->_registers.p.n = true;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::ImmediateForA);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, true, "The overflow flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
}

Test(BIT, other)
{
	Init()
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0xFF;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.p.n = false;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, true, "The overflow flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
}

Test(ASL, emulationTest)
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0b01100110, "The data in ram should be 0b01100110 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(ASL, nativeTest)
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0b01100110, "The data in ram should be 0b01100110 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.wram->_data[1], 0b00000111, "The data in ram should be 0b00000111 but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(ASL, accumulator)
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.al, 0b01100110, "The accumulator should be 0b01100110 but it was %x", snes.cpu->_registers.al);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(LSR, emulationTest)
{
	Init()
	snes.wram->_data[0] = 0b01100110;
	snes.cpu->_registers.p.m = true;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0b00110011, "The data in ram should be 0b00110011 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(LSR, nativeTest)
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0b11011001, "The data in ram should be 0b11011001 but it was %x", snes.wram->_data[0]);
	cr_assert_eq(snes.wram->_data[1], 0b01000001, "The data in ram should be 0b01000001 but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}

Test(LSR, accumulator)
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.al, 0b01011001, "The accumulator should be 0b01011001 but it was %x", snes.cpu->_registers.al);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
}