//
// Created by anonymus-raccoon on 4/6/20.
//

#include <catch2/catch_test_macros.hpp>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("underflow CMP", "[CMP]")
{
	Init()
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.a = 0;
	snes.wram._data[0] = 0x1;
	snes.cpu.CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("zero CMP", "[CMP]")
{
	Init()
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.a = 0x5;
	snes.wram._data[0] = 0x5;
	snes.cpu.CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("nativeModeZero CMP", "[CMP]")
{
	Init()
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.a = 0x5000;
	snes.wram._data[0] = 0x00;
	snes.wram._data[1] = 0x50;
	snes.cpu.CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("nativeModeNothing CMP", "[CMP]")
{
	Init()
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.a = 0x8000;
	snes.wram._data[0] = 0x00;
	snes.wram._data[1] = 0x50;
	snes.cpu.CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("negative CMP", "[CMP]")
{
	Init()
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.a = 0xB000;
	snes.wram._data[0] = 0x00;
	snes.wram._data[1] = 0x10;
	snes.cpu.CMP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == true);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

