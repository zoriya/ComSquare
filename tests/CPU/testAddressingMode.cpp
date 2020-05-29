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
	Init()
	cr_assert_eq(snes.bus.get(), snes.cpu->_bus.get(), "Warning, the CPU's bus is not the same the SNES's bus. Next tests of the CPU may fail.");
}

Test(AddrMode, Immediate)
{
	Init()
	snes.cpu->_registers.pac = 0x000015;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.m = false;
	cr_assert_eq(snes.cpu->_getImmediateAddrForA(), 0x000015, "Got %x, Expected 0x000015");
	cr_assert_eq(snes.cpu->_registers.pac, 0x000017);
}

Test(AddrMode, ImmediateMemoryFlag)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.pac = 0x000015;
	snes.cpu->_registers.p.m = false;
	cr_assert_eq(snes.cpu->_getImmediateAddrForA(), 0x000015, "Got %x, Expected 0x000015");
	cr_assert_eq(snes.cpu->_registers.pac, 0x000017);
}

Test(AddrMode, ImmediateBankChange)
{
	Init()
	snes.cpu->_registers.pac = 0x00FFFF;
	snes.cpu->_registers.p.m = true;
	cr_assert_eq(snes.cpu->_getImmediateAddrForA(), 0x00FFFF);
	cr_assert_eq(snes.cpu->_registers.pac, 0x000000);
}

Test(AddrMode, Direct)
{
	Init()
	snes.cartridge->_data[0] = 0x15;
	snes.cpu->_registers.pac = 0x808000;
	snes.cpu->_registers.d = 0x1000;
	cr_assert_eq(snes.cpu->_getDirectAddr(), 0x1015, "Returned address was %x but was expecting 0x1015.", snes.cpu->_getDirectAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, Absolute)
{
	Init()
	snes.cartridge->_data[0] = 0x1C;
	snes.cartridge->_data[1] = 0x90;
	snes.cpu->_registers.pac = 0x808000;
	snes.cpu->_registers.dbr = 0x88;
	cr_assert_eq(snes.cpu->_getAbsoluteAddr(), 0x88901C, "Returned address was %x but was expecting 0x88901C.", snes.cpu->_getAbsoluteAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteLong)
{
	Init()
	snes.cartridge->_data[0] = 0x1C;
	snes.cartridge->_data[1] = 0x90;
	snes.cartridge->_data[2] = 0xFF;
	snes.cpu->_registers.pac = 0x808000;
	snes.cpu->_registers.dbr = 0x88;
	cr_assert_eq(snes.cpu->_getAbsoluteLongAddr(), 0xFF901C, "Returned address was %x but was expecting 0xFF901C.", snes.cpu->_getAbsoluteLongAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808003);
}

Test(AddrMode, DirectIndirectIndexed)
{
	Init()
	snes.cartridge->_data[0] = 0x10;
	snes.wram->_data[0x1010] = 0x30;
	snes.wram->_data[0x1011] = 0x40;
	snes.cpu->_registers.pac = 0x808000;
	snes.cpu->_registers.dbr = 0x80;
	snes.cpu->_registers.y = 0x0001;
	snes.cpu->_registers.d = 0x1000;
	cr_assert_eq(snes.cpu->_getDirectIndirectIndexedYAddr(), 0x804031, "Returned address was %x but was expecting 0x804031.",
				 snes.cpu->_getDirectIndirectIndexedYAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndirectIndexedLong)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cpu->_registers.d = 0x1000;
	snes.cartridge->_data[0] = 0x10;
	snes.wram->_data[0x1010] = 0x30;
	snes.wram->_data[0x1011] = 0x40;
	snes.wram->_data[0x1012] = 0x23;
	cr_assert_eq(snes.cpu->_getDirectIndirectIndexedYLongAddr(), 0x234030, "Returned address was %x but was expecting 0x234030.",
				 snes.cpu->_getDirectIndirectIndexedYLongAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedIndirect)
{
	Init()
	snes.cartridge->_data[0] = 0x10;
	snes.cpu->_registers.d = 0x1000;
	snes.cpu->_registers.x = 0x0002;
	snes.wram->_data[0x1012] = 0x30;
	snes.wram->_data[0x1013] = 0x40;
	snes.cpu->_registers.dbr = 0x80;
	snes.cpu->_registers.pac = 0x808000;
	cr_assert_eq(snes.cpu->_getDirectIndirectIndexedXAddr(), 0x804030, "Returned address was %x but was expecting 0x804030.",
				 snes.cpu->_getDirectIndirectIndexedXAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedByX)
{
	Init()
	snes.cartridge->_data[0] = 0x10;
	snes.cpu->_registers.d = 0x1000;
	snes.cpu->_registers.x = 0x0002;
	snes.cpu->_registers.pac = 0x808000;
	cr_assert_eq(snes.cpu->_getDirectIndexedByXAddr(), 0x1012, "Returned address was %x but was expecting 0x1012.", snes.cpu->_getDirectIndexedByXAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndexedByY)
{
	Init()
	snes.cartridge->_data[0] = 0x10;
	snes.cpu->_registers.d = 0x1000;
	snes.cpu->_registers.y = 0x0002;
	snes.cpu->_registers.pac = 0x808000;
	cr_assert_eq(snes.cpu->_getDirectIndexedByYAddr(), 0x1012, "Returned address was %x but was expecting 0x1012.", snes.cpu->_getDirectIndexedByYAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, AbsoluteIndexByX)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x10;
	snes.cartridge->_data[1] = 0xAC;
	snes.cpu->_registers.dbr = 0xEF;
	snes.cpu->_registers.x = 0x0005;
	cr_assert_eq(snes.cpu->_getAbsoluteIndexedByXAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.", snes.cpu->_getAbsoluteIndexedByXAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteIndexByY)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x10;
	snes.cartridge->_data[1] = 0xAC;
	snes.cpu->_registers.dbr = 0xEF;
	snes.cpu->_registers.y = 0x0005;
	cr_assert_eq(snes.cpu->_getAbsoluteIndexedByYAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.", snes.cpu->_getAbsoluteIndexedByYAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteLongIndexByX)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x10;
	snes.cartridge->_data[1] = 0xAC;
	snes.cartridge->_data[2] = 0xEF;
	snes.cpu->_registers.x = 0x0005;
	cr_assert_eq(snes.cpu->_getAbsoluteIndexedByXLongAddr(), 0xEFAC15, "Returned address was %x but was expecting 0xEFAC15.",
				 snes.cpu->_getAbsoluteIndexedByXLongAddr());
	cr_assert_eq(snes.cpu->_registers.pac, 0x808003);
}

//Test(AddrMode, ProgramCounterRelativePositive)
//{
//	Init()
//	snes.cpu->_registers.pac = 0x808010;
//	snes.cartridge->_data[0x10] = 0x15;
//	cr_assert_eq(snes.cpu->_getProgramCounterRelativeAddr(), 0x808025, "Returned address was %x but was expecting 0x808025.", snes.cpu->_getProgramCounterRelativeAddr());
//	cr_assert_eq(snes.cpu->_registers.pac, 0x808011);
//}
//
//Test(AddrMode, ProgramCounterRelativeNegative)
//{
//	Init()
//	snes.cpu->_registers.pac = 0x808010;
//	snes.cartridge->_data[0x10] = -0x15;
//	cr_assert_eq(snes.cpu->_getProgramCounterRelativeAddr(), 0x807FFB, "Returned address was %x but was expecting 0x807FFB.", snes.cpu->_getProgramCounterRelativeAddr());
//	cr_assert_eq(snes.cpu->_registers.pac, 0x808011);
//}
//
//Test(AddrMode, ProgramCounterRelativeLongPositive)
//{
//	Init()
//	snes.cpu->_registers.pac = 0x808010;
//	snes.cartridge->_data[0x10] = 0x15;
//	snes.cartridge->_data[0x11] = 0x10;
//	auto addr = snes.cpu->_getProgramCounterRelativeLongAddr();
//	cr_assert_eq(addr, 0x809025, "Returned address was %x but was expecting 0x809025.", addr);
//	cr_assert_eq(snes.cpu->_registers.pac, 0x808012);
//}
//
//Test(AddrMode, ProgramCounterRelativeLongNegative)
//{
//	Init()
//	snes.cpu->_registers.pac = 0x808010;
//	snes.cartridge->_data[0x10] = 0x10;
//	snes.cartridge->_data[0x11] = -0x15;
//	auto addr = snes.cpu->_getProgramCounterRelativeLongAddr();
//	cr_assert_eq(addr, 0x806B00, "Returned address was %x but was expecting 0x806B00.", addr);
//	cr_assert_eq(snes.cpu->_registers.pac, 0x808012);
//}

Test(AddrMode, AbsoluteIndirect)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0xAB;
	snes.cartridge->_data[1] = 0x01;
	snes.wram->_data[0x01AB] = 0xEF;
	snes.wram->_data[0x01AC] = 0x01;
	auto addr = snes.cpu->_getAbsoluteIndirectAddr();
	cr_assert_eq(addr, 0x01EF, "Returned address was %x but was expecting 0x01EF.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, AbsoluteIndexedIndirect)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0xAB;
	snes.cartridge->_data[1] = 0x01;
	snes.cpu->_registers.x = 2;
	snes.wram->_data[0x01AD] = 0xEF;
	snes.wram->_data[0x01AE] = 0x01;
	auto addr = snes.cpu->_getAbsoluteIndirectIndexedByXAddr();
	cr_assert_eq(addr, 0x01EF, "Returned address was %x but was expecting 0x01EF.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808002);
}

Test(AddrMode, DirectIndirect)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x01;
	snes.cpu->_registers.d = 0x1010;
	snes.wram->_data[0x1011] = 0xEF;
	snes.wram->_data[0x1012] = 0x01;
	snes.cpu->_registers.dbr = 0x88;
	auto addr = snes.cpu->_getDirectIndirectAddr();
	cr_assert_eq(addr, 0x8801EF, "Returned address was %x but was expecting 0x8801EF.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, DirectIndirectLong)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x06;
	snes.cpu->_registers.d = 0x1010;
	snes.wram->_data[0x1016] = 0xEF;
	snes.wram->_data[0x1017] = 0x01;
	snes.wram->_data[0x1018] = 0x88;
	auto addr = snes.cpu->_getDirectIndirectLongAddr();
	cr_assert_eq(addr, 0x8801EF, "Returned address was %x but was expecting 0x8801EF.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, StackRelative)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x06;
	snes.cpu->_registers.s = 0x1010;
	auto addr = snes.cpu->_getStackRelativeAddr();
	cr_assert_eq(addr, 0x1016, "Returned address was %x but was expecting 0x1016.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}

Test(AddrMode, StackRelativeIndirectIndexed)
{
	Init()
	snes.cpu->_registers.pac = 0x808000;
	snes.cartridge->_data[0] = 0x06;
	snes.cpu->_registers.s = 0x1010;
	snes.cpu->_registers.y = 0x5;
	snes.cpu->_registers.dbr = 0x88;
	auto addr = snes.cpu->_getStackRelativeIndirectIndexedYAddr();
	cr_assert_eq(addr, 0x88101B, "Returned address was %x but was expecting 0x88101B.", addr);
	cr_assert_eq(snes.cpu->_registers.pac, 0x808001);
}