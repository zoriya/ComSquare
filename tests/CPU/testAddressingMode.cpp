//
// Created by anonymus-raccoon on 2/10/20.
//

#include <catch2/catch.hpp>
#include "../tests.hpp"
#include "SNES.hpp"
using namespace ComSquare;


TEST_CASE("Immediate AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x000015;
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = false;
	REQUIRE(snes.cpu._getImmediateAddrForA() == 0x000015);
	REQUIRE(snes.cpu._registers.pac == 0x000017);
}

TEST_CASE("ImmediateMemoryFlag AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.pac = 0x000015;
	snes.cpu._registers.p.m = false;
	REQUIRE(snes.cpu._getImmediateAddrForA() == 0x000015);
	REQUIRE(snes.cpu._registers.pac == 0x000017);
}

TEST_CASE("ImmediateBankChange AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x00FFFF;
	snes.cpu._registers.p.m = true;
	REQUIRE(snes.cpu._getImmediateAddrForA() == 0x00FFFF);
	REQUIRE(snes.cpu._registers.pac == 0x000000);
}

TEST_CASE("Direct AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x15;
	snes.cpu._registers.pac = 0x808000;
	snes.cpu._registers.d = 0x1000;
	REQUIRE(snes.cpu._getDirectAddr() == 0x1015);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("Absolute AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x1C;
	snes.cartridge._data[1] = 0x90;
	snes.cpu._registers.pac = 0x808000;
	snes.cpu._registers.dbr = 0x88;
	REQUIRE(snes.cpu._getAbsoluteAddr() == 0x88901C);
	REQUIRE(snes.cpu._registers.pac == 0x808002);
}

TEST_CASE("AbsoluteLong AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x1C;
	snes.cartridge._data[1] = 0x90;
	snes.cartridge._data[2] = 0xFF;
	snes.cpu._registers.pac = 0x808000;
	snes.cpu._registers.dbr = 0x88;
	REQUIRE(snes.cpu._getAbsoluteLongAddr() == 0xFF901C);
	REQUIRE(snes.cpu._registers.pac == 0x808003);
}

TEST_CASE("DirectIndirectIndexed AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x10;
	snes.wram._data[0x1010] = 0x30;
	snes.wram._data[0x1011] = 0x40;
	snes.cpu._registers.pac = 0x808000;
	snes.cpu._registers.dbr = 0x80;
	snes.cpu._registers.y = 0x0001;
	snes.cpu._registers.d = 0x1000;
	REQUIRE(snes.cpu._getDirectIndirectIndexedYAddr() == 0x804031);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("DirectIndirectIndexedLong AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cpu._registers.d = 0x1000;
	snes.cartridge._data[0] = 0x10;
	snes.wram._data[0x1010] = 0x30;
	snes.wram._data[0x1011] = 0x40;
	snes.wram._data[0x1012] = 0x23;
	REQUIRE(snes.cpu._getDirectIndirectIndexedYLongAddr() == 0x234030);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("DirectIndexedIndirect AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x10;
	snes.cpu._registers.d = 0x1000;
	snes.cpu._registers.x = 0x0002;
	snes.wram._data[0x1012] = 0x30;
	snes.wram._data[0x1013] = 0x40;
	snes.cpu._registers.dbr = 0x80;
	snes.cpu._registers.pac = 0x808000;
	REQUIRE(snes.cpu._getDirectIndirectIndexedXAddr() == 0x804030);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("DirectIndexedByX AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x10;
	snes.cpu._registers.d = 0x1000;
	snes.cpu._registers.x = 0x0002;
	snes.cpu._registers.pac = 0x808000;
	REQUIRE(snes.cpu._getDirectIndexedByXAddr() == 0x1012);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("DirectIndexedByY AddrMode", "[AddrMode]")
{
	Init()
	snes.cartridge._data[0] = 0x10;
	snes.cpu._registers.d = 0x1000;
	snes.cpu._registers.y = 0x0002;
	snes.cpu._registers.pac = 0x808000;
	REQUIRE(snes.cpu._getDirectIndexedByYAddr() == 0x1012);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("AbsoluteIndexByX AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x10;
	snes.cartridge._data[1] = 0xAC;
	snes.cpu._registers.dbr = 0xEF;
	snes.cpu._registers.x = 0x0005;
	REQUIRE(snes.cpu._getAbsoluteIndexedByXAddr() == 0xEFAC15);
	REQUIRE(snes.cpu._registers.pac == 0x808002);
}

TEST_CASE("AbsoluteIndexByY AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x10;
	snes.cartridge._data[1] = 0xAC;
	snes.cpu._registers.dbr = 0xEF;
	snes.cpu._registers.y = 0x0005;
	REQUIRE(snes.cpu._getAbsoluteIndexedByYAddr() == 0xEFAC15);
	REQUIRE(snes.cpu._registers.pac == 0x808002);
}

TEST_CASE("AbsoluteLongIndexByX AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x10;
	snes.cartridge._data[1] = 0xAC;
	snes.cartridge._data[2] = 0xEF;
	snes.cpu._registers.x = 0x0005;
	REQUIRE(snes.cpu._getAbsoluteIndexedByXLongAddr() == 0xEFAC15);
	REQUIRE(snes.cpu._registers.pac == 0x808003);
}

//TEST_CASE("ProgramCounterRelativePositive AddrMode", "[AddrMode]")
//{
//	Init()
//	snes.cpu._registers.pac = 0x808010;
//	snes.cartridge._data[0x10] = 0x15;
//	REQUIRE(snes.cpu._getProgramCounterRelativeAddr() == 0x808025);
//	REQUIRE(snes.cpu._registers.pac == 0x808011);
//}
//
//TEST_CASE("ProgramCounterRelativeNegative AddrMode", "[AddrMode]")
//{
//	Init()
//	snes.cpu._registers.pac = 0x808010;
//	snes.cartridge._data[0x10] = -0x15;
//	REQUIRE(snes.cpu._getProgramCounterRelativeAddr() == 0x807FFB);
//	REQUIRE(snes.cpu._registers.pac == 0x808011);
//}
//
//TEST_CASE("ProgramCounterRelativeLongPositive AddrMode", "[AddrMode]")
//{
//	Init()
//	snes.cpu._registers.pac = 0x808010;
//	snes.cartridge._data[0x10] = 0x15;
//	snes.cartridge._data[0x11] = 0x10;
//	auto addr = snes.cpu._getProgramCounterRelativeLongAddr();
//	REQUIRE(addr == 0x809025);
//	REQUIRE(snes.cpu._registers.pac == 0x808012);
//}
//
//TEST_CASE("ProgramCounterRelativeLongNegative AddrMode", "[AddrMode]")
//{
//	Init()
//	snes.cpu._registers.pac = 0x808010;
//	snes.cartridge._data[0x10] = 0x10;
//	snes.cartridge._data[0x11] = -0x15;
//	auto addr = snes.cpu._getProgramCounterRelativeLongAddr();
//	REQUIRE(addr == 0x806B00);
//	REQUIRE(snes.cpu._registers.pac == 0x808012);
//}

TEST_CASE("AbsoluteIndirect AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0xAB;
	snes.cartridge._data[1] = 0x01;
	snes.wram._data[0x01AB] = 0xEF;
	snes.wram._data[0x01AC] = 0x01;
	auto addr = snes.cpu._getAbsoluteIndirectAddr();
	REQUIRE(addr == 0x01EF);
	REQUIRE(snes.cpu._registers.pac == 0x808002);
}

TEST_CASE("AbsoluteIndexedIndirect AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0xAB;
	snes.cartridge._data[1] = 0x01;
	snes.cpu._registers.x = 2;
	snes.wram._data[0x01AD] = 0xEF;
	snes.wram._data[0x01AE] = 0x01;
	auto addr = snes.cpu._getAbsoluteIndirectIndexedByXAddr();
	REQUIRE(addr == 0x01EF);
	REQUIRE(snes.cpu._registers.pac == 0x808002);
}

TEST_CASE("DirectIndirect AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x01;
	snes.cpu._registers.d = 0x1010;
	snes.wram._data[0x1011] = 0xEF;
	snes.wram._data[0x1012] = 0x01;
	snes.cpu._registers.dbr = 0x88;
	auto addr = snes.cpu._getDirectIndirectAddr();
	REQUIRE(addr == 0x8801EF);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("DirectIndirectLong AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x06;
	snes.cpu._registers.d = 0x1010;
	snes.wram._data[0x1016] = 0xEF;
	snes.wram._data[0x1017] = 0x01;
	snes.wram._data[0x1018] = 0x88;
	auto addr = snes.cpu._getDirectIndirectLongAddr();
	REQUIRE(addr == 0x8801EF);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("StackRelative AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x06;
	snes.cpu._registers.s = 0x1010;
	auto addr = snes.cpu._getStackRelativeAddr();
	REQUIRE(addr == 0x1016);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}

TEST_CASE("StackRelativeIndirectIndexed AddrMode", "[AddrMode]")
{
	Init()
	snes.cpu._registers.pac = 0x808000;
	snes.cartridge._data[0] = 0x06;
	snes.cpu._registers.s = 0x1010;
	snes.cpu._registers.y = 0x5;
	snes.cpu._registers.dbr = 0x88;
	auto addr = snes.cpu._getStackRelativeIndirectIndexedYAddr();
	REQUIRE(addr == 0x88101B);
	REQUIRE(snes.cpu._registers.pac == 0x808001);
}