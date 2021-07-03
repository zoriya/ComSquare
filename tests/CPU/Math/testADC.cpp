//
// Created by anonymus-raccoon on 2/11/20.
//

#include <catch2/catch.hpp>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("addingOne ADC", "[ADC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 1);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("addingOneEmulation ADC", "[ADC]")
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 1);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("overflow ADC", "[ADC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0xFFFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("overflowEmulation ADC", "[ADC]")
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0xFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("signedOverflow ADC", "[ADC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x7FFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x8000);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == true);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("signedOverflowEmulated ADC", "[ADC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x007F;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x0080);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == true);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negative ADC", "[ADC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8FFF;
	snes.wram->_data[0] = 0x1;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x9000);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("memoryTwoBytes ADC", "[ADC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x000F;
	snes.wram->_data[0] = 0x01;
	snes.wram->_data[1] = 0x04;
	snes.cpu->ADC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x0410);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}