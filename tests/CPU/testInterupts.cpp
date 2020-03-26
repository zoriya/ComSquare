//
// Created by anonymus-raccoon on 1/24/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;

Test(CPU_emulated, BRK)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cartridge->header.emulationInterrupts.brk = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->BRK(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x123u, "The program counter should be 0x123u but it was 0x%X", snes.cpu->_registers.pc);
	cr_assert_eq(snes.cpu->_registers.pbr, 0x15, "The PBR should be 0x15 but it was 0x%X", snes.cpu->_registers.pbr);
	cr_assert_eq(snes.cpu->_registers.p.d, false, "The decimal flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.i, true, "The Interrupt disable flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.x_b, true, "The break flag should be set.");
	int data = snes.cpu->_pop();
	cr_assert_eq(data, 0xF1, "The Status Registers should be pushed into the stack with the value 0xF1 but it was 0x%X  (expected 0xF1).", data);
	data = snes.cpu->_pop16();
	cr_assert_eq(data, 0x158u, "The program counter should be incremented by two and pushed on the stack but it was 0x%X (expected 0x158).", data);
}

Test(CPU_native, BRK)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cartridge->header.nativeInterrupts.brk = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->BRK(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x123u, "The program counter should be 0x123u but it was 0x%X", snes.cpu->_registers.pc);
	cr_assert_eq(snes.cpu->_registers.pbr, 0x0, "The PBR should be 0x0 but it was 0x%X", snes.cpu->_registers.pbr);
	cr_assert_eq(snes.cpu->_registers.p.d, false, "The decimal flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.i, true, "The Interrupt disable flag should be set.");
	int data = snes.cpu->_pop();
	cr_assert_eq(data, 0xF1, "The Status Registers should be pushed into the stack with the value 0xF1 but it was 0x%X  (expected 0xF1).", data);
	data = snes.cpu->_pop16();
	cr_assert_eq(data, 0x158u, "The program counter should be incremented by two and pushed on the stack but it was 0x%X (expected 0x158).", data);
	data = snes.cpu->_pop();
	cr_assert_eq(data, 0x15, "The program bank register should be pushed on the stack but it was 0x%X  (expected 0x15).", data);
}

Test(CPU_emulated, COP)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cartridge->header.emulationInterrupts.cop = 0x123u;
	snes.cpu->_registers.p.flags = 0x0F;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->COP(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x123u, "The program counter should be 0x123u but it was 0x%X", snes.cpu->_registers.pc);
	cr_assert_eq(snes.cpu->_registers.pbr, 0x15, "The PBR should be 0x15 but it was 0x%X", snes.cpu->_registers.pbr);
	cr_assert_eq(snes.cpu->_registers.p.d, false, "The decimal flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.i, true, "The Interrupt disable flag should be set.");
	cr_assert_eq(snes.cpu->_registers.p.x_b, false, "The break flag should not be set.");
	int data = snes.cpu->_pop();
	cr_assert_eq(data, 0x0F, "The Status Registers should be pushed into the stack with the value 0x0F but it was 0x%X  (expected 0xF1).", data);
	data = snes.cpu->_pop16();
	cr_assert_eq(data, 0x158u, "The program counter should be incremented by two and pushed on the stack but it was 0x%X (expected 0x158).", data);
}

Test(CPU_native, COP)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cartridge->header.nativeInterrupts.cop = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->COP(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x123u, "The program counter should be 0x123u but it was 0x%X", snes.cpu->_registers.pc);
	cr_assert_eq(snes.cpu->_registers.pbr, 0x0, "The PBR should be 0x0 but it was 0x%X", snes.cpu->_registers.pbr);
	cr_assert_eq(snes.cpu->_registers.p.d, false, "The decimal flag should not be set.");
	cr_assert_eq(snes.cpu->_registers.p.i, true, "The Interrupt disable flag should be set.");
	int data = snes.cpu->_pop();
	cr_assert_eq(data, 0xF1, "The Status Registers should be pushed into the stack with the value 0xF1 but it was 0x%X  (expected 0xF1).", data);
	data = snes.cpu->_pop16();
	cr_assert_eq(data, 0x158u, "The program counter should be incremented by two and pushed on the stack but it was 0x%X (expected 0x158).", data);
	data = snes.cpu->_pop();
	cr_assert_eq(data, 0x15, "The program bank register should be pushed on the stack but it was 0x%X  (expected 0x15).", data);
}