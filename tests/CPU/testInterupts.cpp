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
	auto pair = Init();
	pair.second.cartridge->header.emulationInterrupts.brk = 0x123u;
	pair.second.cpu->_registers.p.d = true;
	pair.second.cpu->_registers.pc = 0x156u;
	cr_assert_eq(pair.second.cpu->BRK(), 0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x123u);
	cr_assert_eq(pair.second.cpu->_registers.p.i, 1, "pair.second.cpu->_registers.p.i mmust be equal to 1 but it was %d", pair.second.cpu->_registers.p.i);
	cr_assert_eq(pair.second.cpu->_registers.p.d, false);
}

Test(CPU_native, BRK)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cartridge->header.nativeInterrupts.brk = 0x123u;
	pair.second.cpu->_registers.pc = 0x156u;
	cr_assert_eq(pair.second.cpu->BRK(), 1);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x123u);
	cr_assert_eq(pair.second.cpu->_registers.p.i, true);
	cr_assert_eq(pair.second.cpu->_registers.p.d, false);
}