//
// Created by anonymus-raccoon on 2/10/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include <iostream>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;


Test(AddrModeInit, LegitBus)
{
	auto pair = Init();
	cr_assert_eq(pair.first.get(), pair.second.cpu->_bus.get(), "Warning, the CPU's bus is not the same the SNES's bus. Next tests of the CPU may fail.");
}

Test(AddrMode, Immediate)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x000015;
	cr_assert_eq(pair.second.cpu->_getImmediateAddr(), 0x000015, "Got %x, Expected 0x000015");
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x000016);
}

Test(AddrMode, ImmediateBankChange)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x00FFFF;
	cr_assert_eq(pair.second.cpu->_getImmediateAddr(), 0x00FFFF);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x010000);
}

Test(AddrMode, Direct)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x15;
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cpu->_registers.d = 0x1000;
	cr_assert_eq(pair.second.cpu->_getDirectAddr(), 0x1015, "Returned address was %x but was expecting 0x1015.", pair.second.cpu->_getDirectAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, Absolute)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x1C;
	pair.second.cartridge->_data[1] = 0x90;
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cpu->_registers.dbr = 0x88;
	cr_assert_eq(pair.second.cpu->_getAbsoluteAddr(), 0x88901C, "Returned address was %x but was expecting 0x88901C.", pair.second.cpu->_getAbsoluteAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteLong)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x1C;
	pair.second.cartridge->_data[1] = 0x90;
	pair.second.cartridge->_data[2] = 0xFF;
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cpu->_registers.dbr = 0x88;
	cr_assert_eq(pair.second.cpu->_getAbsoluteLongAddr(), 0xFF901C, "Returned address was %x but was expecting 0xFF901C.", pair.second.cpu->_getAbsoluteLongAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808003);
}

Test(AddrMode, DirectIndirectIndexed)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.wram->_data[0x1010] = 0x30;
	pair.second.wram->_data[0x1011] = 0x40;
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cpu->_registers.dbr = 0x80;
	pair.second.cpu->_registers.y = 0x0001;
	pair.second.cpu->_registers.d = 0x1000;
	cr_assert_eq(pair.second.cpu->_getDirectIndirectIndexedAddr(), 0x804031, "Returned address was %x but was expecting 0x804031.", pair.second.cpu->_getDirectIndirectIndexedAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}