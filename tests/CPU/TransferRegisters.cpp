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
