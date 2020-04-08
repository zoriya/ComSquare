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

Test(ORA, simple)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0x0F;
	snes.cpu->ORA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x8F, "The accumulator's value should be 0x8F but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ORA, simple2)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->ORA(0x00, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0xF0, "The accumulator's value should be 0xF0 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(ORA, zero)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x00;
	snes.wram->_data[0] = 0x00;
	snes.cpu->ORA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x00, "The accumulator's value should be 0x00 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(INC, simple)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x56;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0x57, "The incremented value should be 0x57 but it was 0x%x.", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(INC, negative)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x7F;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0x80, "The incremented value should be 0x80 but it was 0x%x.", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(INC, accumulator)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x56;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x57, "The incremented value should be 0x57 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(INC, negativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x7F;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x80, "The incremented value should be 0x80 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(INC, nativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x5600;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x5601, "The incremented value should be 0x5601 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(INC, negativeNativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8FFF;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x9000, "The incremented value should be 0x9000 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, simple)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x58;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0x57, "The incremented value should be 0x57 but it was 0x%x.", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, negative)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x81;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	cr_assert_eq(snes.wram->_data[0], 0x80, "The incremented value should be 0x80 but it was 0x%x.", snes.wram->_data[0]);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, accumulator)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x58;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x57, "The incremented value should be 0x57 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, negativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x81;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x80, "The incremented value should be 0x80 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, nativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x5602;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x5601, "The incremented value should be 0x5601 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(DEC, negativeNativeAccumulator)
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x9001;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x9000, "The incremented value should be 0x9000 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(EOR, simple)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0x0F;
	snes.cpu->EOR(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x8F, "The accumulator's value should be 0x8F but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(EOR, simple2)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->EOR(0x00, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x70, "The accumulator's value should be 0x70 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should not be set.");
}

Test(EOR, zero)
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x00;
	snes.wram->_data[0] = 0x00;
	snes.cpu->EOR(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x00, "The accumulator's value should be 0x00 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(XBA, zero)
{
	Init()
	snes.cpu->_registers.a = 0x0001;
	snes.cpu->XBA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x0100, "The accumulator's value should be 0x0100 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flags should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flags should be set.");
}

Test(XBA, negative)
{
	Init()
	snes.cpu->_registers.a = 0x8001;
	snes.cpu->XBA(0x0, ComSquare::CPU::AddressingMode::Implied);
	cr_assert_eq(snes.cpu->_registers.a, 0x0180, "The accumulator's value should be 0x0180 but it was 0x%x.", snes.cpu->_registers.a);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flags should be set.");
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flags should be not set.");
}
