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

