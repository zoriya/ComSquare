//
// Created by anonymus-raccoon on 4/3/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

Test(DEX, simple)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x57;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0x56, "The x index value should be 0x56 but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEX, overflowEmul)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0xFF, "The x index value should be 0xFF but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEX, fakeOverflowNonEmul)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xFF00;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0xFEFF, "The x index value should be 0xFEFF but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEX, nonNegative)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x80;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0x7F, "The x index value should be 0x7F but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEX, zero)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 1;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.x, 0, "The x index value should be 0 but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(DEY, simple)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x57;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0x56, "The x index value should be 0x56 but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEY, overflowEmul)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0xFF, "The x index value should be 0xFF but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEY, fakeOverflowNonEmul)
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0xFF00;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0xFEFF, "The x index value should be 0xFEFF but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEY, nonNegative)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x80;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0x7F, "The x index value should be 0x7F but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEY, zero)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 1;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.y, 0, "The x index value should be 0 but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}