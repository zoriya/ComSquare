//
// Created by anonymus-raccoon on 2/13/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;
//
//Test(SEP, setall)
//{
//	auto pair = Init();
//	pair.second.wram->_data[0] = 0xFF;
//	pair.second.cpu->SEP(0x0);
//	auto data = pair.second.cpu->_registers.p.flags;
//	cr_assert_eq(data, 0xFF, "The flag should be 0xFF but it was %b", data);
//}
//
//Test(SEP, setsome)
//{
//	auto pair = Init();
//	pair.second.wram->_data[0] = 0b10110101;
//	pair.second.cpu->SEP(0x0);
//	cr_assert_eq(pair.second.cpu->_registers.p.flags, 0xFF);
//}