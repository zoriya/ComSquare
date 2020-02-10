//
// Created by anonymus-raccoon on 2/10/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;


Test(AddrMode, Immediate)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x15;
	cr_assert_eq(pair.second.cpu->_GetImmediateAddr(), 0x15);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x16);
}