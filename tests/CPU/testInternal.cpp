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

Test(SEP, setall)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->SEP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0xFF, "The flag should be 0xFF but it was %x", data);
}

Test(SEP, setsome)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0b10110101;
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->SEP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b11110101, "The flag should be 245 but it was %i", data);
}

Test(REP, resetall)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x00, "The flag should be 0x00 but it was %x", data);
}

Test(REP, resetsome)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.wram->_data[0] = 0b01000000;
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x0, "The flag should be 0 but it was %x", data);
}

Test(REP, resetallEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flag should be 0b00110000 but it was %x", data);
}

Test(REP, resetsomeEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.wram->_data[0] = 0b01000001;
	pair.second.cpu->_registers.p.flags = 0b01000101;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110100, "The flag should be 0b00110100 but it was %x", data);
}