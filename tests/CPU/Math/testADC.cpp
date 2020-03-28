//
// Created by anonymus-raccoon on 2/11/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

Test(ADC, addingOne)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 1, "The accumulator's value should be 0x1 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, addingOneEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 1, "The accumulator's value should be 0x1 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, overflow)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0xFFFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(ADC, overflowEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0xFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0, "The accumulator's value should be 0x0 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(ADC, signedOverflow)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x7FFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x8000, "The accumulator's value should be 0x8000 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, true, "The overflow flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, signedOverflowEmulated)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0x007F;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x0080, "The accumulator's value should be 0x0080 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, true, "The overflow flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, negative)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x8FFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x9000, "The accumulator's value should be 0x9000 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ADC, memoryTwoBytes)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x000F;
	snes.wram->_data[0] = 0x01;
	snes.wram->_data[1] = 0x04;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x0410, "The accumulator's value should be 0x0410 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}