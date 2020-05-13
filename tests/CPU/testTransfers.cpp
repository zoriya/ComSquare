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
	snes.cpu->TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TXS(0x0, ComSquare::CPU::AddressingMode::Implied);
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
	snes.cpu->TXS(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.s, 0x00CD, "The stack pointer should be 0x00CD but it was %x", snes.cpu->_registers.s);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TCD, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.d = 0x5656;
	snes.cpu->_registers.a = 0xABCD;
	snes.cpu->TCD(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.d, 0xABCD, "The direct page should be 0xABCD but it was %x", snes.cpu->_registers.d);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TCD, zero)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.d = 0x5656;
	snes.cpu->_registers.a = 0x0;
	snes.cpu->TCD(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.d, 0, "The direct page should be 0x0 but it was %x", snes.cpu->_registers.d);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TCS, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.s = 0x0156;
	snes.cpu->_registers.a = 0xABCD;
	snes.cpu->TCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.s, 0x01CD, "The stack pointer should be 0x01CD but it was %x", snes.cpu->_registers.s);
}

Test(TCS, native)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.s = 0x0156;
	snes.cpu->_registers.a = 0xABCD;
	snes.cpu->TCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.s, 0xABCD, "The stack pointer should be 0xABCD but it was %x", snes.cpu->_registers.s);
}

Test(TDC, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.d = 0xABCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TDC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xABCD, "The accumulator should be 0xABCD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TDC, zero)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.d = 0x0;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TDC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0, "The accumulator should be 0x0 but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TSC, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.s = 0xABCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TSC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xABCD, "The accumulator should be 0xABCD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TSC, zero)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.s = 0x0;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TSC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0, "The accumulator should be 0x0 but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.");
}

Test(TSX, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.s = 0xABCD;
	snes.cpu->_registers.x = 0x5656;
	snes.cpu->TSX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0x00CD, "The x index should be 0x00CD but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TSX, native)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.s = 0x8F00;
	snes.cpu->_registers.x = 0x5656;
	snes.cpu->TSX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0x8F00, "The x index should be 0x8F00 but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXA, double8bits)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0xABCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x56CD, "The accumulator should be 0x56CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXA, index8bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x0BCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x00CD, "The accumulator should be 0x00CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXA, accumulator8bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0x0BCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x56CD, "The accumulator should be 0x56CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXA, double16bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xAB0D;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xAB0D, "The accumulator should be 0xAB0D but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}


Test(TYA, double8bits)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0xABCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x56CD, "The accumulator should be 0x56CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TYA, index8bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x0BCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x00CD, "The accumulator should be 0x00CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TYA, accumulator8bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0x0BCD;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x56CD, "The accumulator should be 0x56CD but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TYA, double16bits)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0xAB0D;
	snes.cpu->_registers.a = 0x5656;
	snes.cpu->TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xAB0D, "The accumulator should be 0xAB0D but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXY, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x0BCD;
	snes.cpu->_registers.y = 0x5656;
	snes.cpu->TXY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0x56CD, "The y index should be 0x56CD but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TXY, nativeMode)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xAB0D;
	snes.cpu->_registers.y = 0x5656;
	snes.cpu->TXY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0xAB0D, "The y index should be 0xAB0D but it was %x", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TYX, emulationMode)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x0BCD;
	snes.cpu->_registers.x = 0x5656;
	snes.cpu->TYX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0x56CD, "The x index should be 0x56CD but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(TYX, nativeMode)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0xAB0D;
	snes.cpu->_registers.x = 0x5656;
	snes.cpu->TYX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0xAB0D, "The x index should be 0xAB0D but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should be not set.");
}

Test(MVN, hardCase)
{
	Init()
	snes.cpu->_registers.a = 0x10;
	snes.cpu->_registers.x = 0x0000;
	snes.cpu->_registers.y = 0x1000;
	for (int i = 0; i <= snes.cpu->_registers.a; i++)
		snes.wram->_data[i] = i;

	int cycles = snes.cpu->MVN(0x2010, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(cycles, 0x77, "The MVN should take 0x77 cycles but it took %x.", cycles);
	cr_assert_eq(snes.cpu->_registers.dbr, 0x20, "The data bank register should be 0x20 but it was %x", snes.cpu->_registers.dbr);
	cr_assert_eq(snes.cpu->_registers.a, 0xFFFF, "The c accumulator should be 0xFFFF but it was %x", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.x, 0x0011, "The x index should be 0x0011 but it was %x", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.y, 0x1011, "The y index should be 0x1011 but it was %x", snes.cpu->_registers.y);
	for (int i = 0; i < 0x11; i++)
		cr_assert_eq(snes.wram->_data[i + 0x1000], i, "The data in ram should be %x but it was %x", i, snes.wram->_data[i + 0x1000]);
}