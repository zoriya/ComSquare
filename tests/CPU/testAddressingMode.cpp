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
	cr_assert_eq(pair.second.cpu->_getDirectIndirectIndexedYAddr(), 0x804031, "Returned address was %x but was expecting 0x804031.",
				 pair.second.cpu->_getDirectIndirectIndexedYAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndirectIndexedLong)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cpu->_registers.d = 0x1000;
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.wram->_data[0x1010] = 0x30;
	pair.second.wram->_data[0x1011] = 0x40;
	pair.second.wram->_data[0x1012] = 0x23;
	cr_assert_eq(pair.second.cpu->_getDirectIndirectIndexedYLongAddr(), 0x234030, "Returned address was %x but was expecting 0x234030.",
				 pair.second.cpu->_getDirectIndirectIndexedYLongAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedIndirect)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cpu->_registers.d = 0x1000;
	pair.second.cpu->_registers.x = 0x0002;
	pair.second.wram->_data[0x1012] = 0x30;
	pair.second.wram->_data[0x1013] = 0x40;
	pair.second.cpu->_registers.dbr = 0x80;
	pair.second.cpu->_registers.pac = 0x808000;
	cr_assert_eq(pair.second.cpu->_getDirectIndirectIndexedXAddr(), 0x804030, "Returned address was %x but was expecting 0x804030.",
				 pair.second.cpu->_getDirectIndirectIndexedXAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedByX)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cpu->_registers.d = 0x1000;
	pair.second.cpu->_registers.x = 0x0002;
	pair.second.cpu->_registers.pac = 0x808000;
	cr_assert_eq(pair.second.cpu->_getDirectIndexedByXAddr(), 0x1012, "Returned address was %x but was expecting 0x1012.", pair.second.cpu->_getDirectIndexedByXAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedByY)
{
	auto pair = Init();
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cpu->_registers.d = 0x1000;
	pair.second.cpu->_registers.y = 0x0002;
	pair.second.cpu->_registers.pac = 0x808000;
	cr_assert_eq(pair.second.cpu->_getDirectIndexedByYAddr(), 0x1012, "Returned address was %x but was expecting 0x1012.", pair.second.cpu->_getDirectIndexedByYAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, AbsoluteIndexByX)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cartridge->_data[1] = 0xAC;
	pair.second.cpu->_registers.dbr = 0xEF;
	pair.second.cpu->_registers.x = 0x0005;
	cr_assert_eq(pair.second.cpu->_getAbsoluteIndexedByXAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.", pair.second.cpu->_getAbsoluteIndexedByXAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteIndexByY)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cartridge->_data[1] = 0xAC;
	pair.second.cpu->_registers.dbr = 0xEF;
	pair.second.cpu->_registers.y = 0x0005;
	cr_assert_eq(pair.second.cpu->_getAbsoluteIndexedByYAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.", pair.second.cpu->_getAbsoluteIndexedByYAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteLongIndexByX)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x10;
	pair.second.cartridge->_data[1] = 0xAC;
	pair.second.cartridge->_data[2] = 0xEF;
	pair.second.cpu->_registers.x = 0x0005;
	cr_assert_eq(pair.second.cpu->_getAbsoluteIndexedByXLongAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.",
				 pair.second.cpu->_getAbsoluteIndexedByXLongAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808003);
}

Test(AddrMode, ProgramCounterRelativePositive)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808010;
	pair.second.cartridge->_data[0x10] = 0x15;
	cr_assert_eq(pair.second.cpu->_getProgramCounterRelativeAddr(), 0x808025, "Returned address was %x but was expecting 0x808025.", pair.second.cpu->_getProgramCounterRelativeAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808011);
}

Test(AddrMode, ProgramCounterRelativeNegative)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808010;
	pair.second.cartridge->_data[0x10] = -0x15;
	cr_assert_eq(pair.second.cpu->_getProgramCounterRelativeAddr(), 0x807FFB, "Returned address was %x but was expecting 0x807FFB.", pair.second.cpu->_getProgramCounterRelativeAddr());
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808011);
}

Test(AddrMode, ProgramCounterRelativeLongPositive)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808010;
	pair.second.cartridge->_data[0x10] = 0x15;
	pair.second.cartridge->_data[0x11] = 0x10;
	auto addr = pair.second.cpu->_getProgramCounterRelativeLongAddr();
	cr_assert_eq(addr, 0x809025, "Returned address was %x but was expecting 0x809025.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808012);
}

Test(AddrMode, ProgramCounterRelativeLongNegative)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808010;
	pair.second.cartridge->_data[0x10] = 0x10;
	pair.second.cartridge->_data[0x11] = -0x15;
	auto addr = pair.second.cpu->_getProgramCounterRelativeLongAddr();
	cr_assert_eq(addr, 0x806B00, "Returned address was %x but was expecting 0x806B00.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808012);
}

Test(AddrMode, AbsoluteIndirect)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0xAB;
	pair.second.cartridge->_data[1] = 0x01;
	pair.second.wram->_data[0x01AB] = 0xEF;
	pair.second.wram->_data[0x01AC] = 0x01;
	auto addr = pair.second.cpu->_getAbsoluteIndirectAddr();
	cr_assert_eq(addr, 0x01EF, "Returned address was %x but was expecting 0x01EF.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteIndexedIndirect)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0xAB;
	pair.second.cartridge->_data[1] = 0x01;
	pair.second.cpu->_registers.x = 2;
	pair.second.wram->_data[0x01AD] = 0xEF;
	pair.second.wram->_data[0x01AE] = 0x01;
	auto addr = pair.second.cpu->_getAbsoluteIndexedIndirectAddr();
	cr_assert_eq(addr, 0x01EF, "Returned address was %x but was expecting 0x01EF.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, DirectIndirect)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x01;
	pair.second.cpu->_registers.d = 0x1010;
	pair.second.wram->_data[0x1011] = 0xEF;
	pair.second.wram->_data[0x1012] = 0x01;
	pair.second.cpu->_registers.dbr = 0x88;
	auto addr = pair.second.cpu->_getDirectIndirectAddr();
	cr_assert_eq(addr, 0x8801EF, "Returned address was %x but was expecting 0x8801EF.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndirectLong)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x06;
	pair.second.cpu->_registers.d = 0x1010;
	pair.second.wram->_data[0x1016] = 0xEF;
	pair.second.wram->_data[0x1017] = 0x01;
	pair.second.wram->_data[0x1018] = 0x88;
	auto addr = pair.second.cpu->_getDirectIndirectLongAddr();
	cr_assert_eq(addr, 0x8801EF, "Returned address was %x but was expecting 0x8801EF.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, StackRelative)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x06;
	pair.second.cpu->_registers.s = 0x1010;
	auto addr = pair.second.cpu->_getStackRelativeAddr();
	cr_assert_eq(addr, 0x1016, "Returned address was %x but was expecting 0x1016.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, StackRelativeIndirectIndexed)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x808000;
	pair.second.cartridge->_data[0] = 0x06;
	pair.second.cpu->_registers.s = 0x1010;
	pair.second.cpu->_registers.y = 0x5;
	pair.second.cpu->_registers.dbr = 0x88;
	auto addr = pair.second.cpu->_getStackRelativeIndirectIndexedYAddr();
	cr_assert_eq(addr, 0x88101B, "Returned address was %x but was expecting 0x88101B.", addr);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
}