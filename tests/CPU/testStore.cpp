//
// Created by anonymus-raccoon on 2/12/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

Test(STA, 8bits)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.m = true;
	pair.second.cpu->_registers.a = 0x11;
	pair.second.cpu->STA(0x0);
	auto data = pair.second.wram->_data[0];
	cr_assert_eq(data, 0x11, "The stored value should be 0x11 but it was 0x%x.", data);
}

Test(STA, 16bits)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.m = false;
	pair.second.cpu->_registers.a = 0x11AB;
	pair.second.cpu->STA(0x0);
	auto data = pair.second.wram->_data[0] + (pair.second.wram->_data[1] << 8u);
	cr_assert_eq(data, 0x11AB, "The stored value should be 0x11AB but it was 0x%x.", data);
}