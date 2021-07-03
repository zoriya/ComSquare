//
// Created by anonymus-raccoon on 2/20/20.
//

#include <catch2/catch.hpp>
#include <bitset>
#include <iostream>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("emulation AND", "[AND]")
{
	Init()
	snes.wram->_data[0] = 0x00;
	snes.cpu->_registers.a = 0xFF;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x00);
	REQUIRE(snes.cpu->_registers.p.z == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("nativeNegative AND", "[AND]")
{
	Init()
	snes.wram->_data[0] = 0xF0;
	snes.wram->_data[1] = 0xF0;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0xFF00;
	snes.cpu->_isEmulationMode = false;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0xF000);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
}

TEST_CASE("emulationTest AND", "[AND]")
{
	Init()
	snes.wram->_data[0] = 0b00110011;
	snes.cpu->_registers.a = 0b00110111;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->AND(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0b00110011);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTest TSB", "[TSB]")
{
	Init()
	snes.wram->_data[0] = 0b00110011;
	snes.cpu->_registers.a = 0b00110111;
	snes.cpu->_registers.p.m = true;
	snes.cpu->TSB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram->_data[0] == 0b00110111);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nativeTest TSB", "[TSB]")
{
	Init()
	snes.wram->_data[0] = 0xF0;
	snes.wram->_data[1] = 0x0F;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.m = false;
	snes.cpu->TSB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram->_data[0] == 0xF8);
	REQUIRE(snes.wram->_data[1] == 0x8F);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("immediate BIT", "[BIT]")
{
	Init()
	snes.wram->_data[0] = 0xFF;
	snes.wram->_data[1] = 0x00;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.p.n = false;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::ImmediateForA);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("immediateZero BIT", "[BIT]")
{
	Init()
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0xFF;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x0008;
	snes.cpu->_registers.p.v = true;
	snes.cpu->_registers.p.n = true;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::ImmediateForA);
	REQUIRE(snes.cpu->_registers.p.z == true);
	REQUIRE(snes.cpu->_registers.p.v == true);
	REQUIRE(snes.cpu->_registers.p.n == true);
}

TEST_CASE("other BIT", "[BIT]")
{
	Init()
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0xFF;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8008;
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.p.n = false;
	snes.cpu->BIT(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.v == true);
	REQUIRE(snes.cpu->_registers.p.n == true);
}

TEST_CASE("emulationTest ASL", "[ASL]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b01100110);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("nativeTest ASL", "[ASL]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b01100110);
	REQUIRE(snes.wram->_data[1] == 0b00000111);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("accumulator ASL", "[ASL]")
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->ASL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.al == 0b01100110);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTest LSR", "[LSR]")
{
	Init()
	snes.wram->_data[0] = 0b01100110;
	snes.cpu->_registers.p.m = true;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b00110011);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("nativeTest LSR", "[LSR]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b11011001);
	REQUIRE(snes.wram->_data[1] == 0b01000001);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("accumulator LSR", "[LSR]")
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->LSR(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.al == 0b01011001);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTest ROL", "[ROL]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->ROL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b01100110);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTestWithCarry ROL", "[ROL]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b01100111);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("nativeTest ROL", "[ROL]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROL(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b01100111);
	REQUIRE(snes.wram->_data[1] == 0b00000111);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("accumulator ROL", "[ROL]")
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.al == 0b01100111);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTestWithtoutCarry ROR", "[ROR]")
{
	Init()
	snes.wram->_data[0] = 0b01100110;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = false;
	snes.cpu->ROR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b00110011);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTest ROR", "[ROR]")
{
	Init()
	snes.wram->_data[0] = 0b01100110;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b10110011);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("nativeTest ROR", "[ROR]")
{
	Init()
	snes.wram->_data[0] = 0b10110011;
	snes.wram->_data[1] = 0b10000011;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROR(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0b11011001);
	REQUIRE(snes.wram->_data[1] == 0b11000001);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("accumulator ROR", "[ROR]")
{
	Init()
	snes.cpu->_registers.a = 0b10110011;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = true;
	snes.cpu->ROR(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.al == 0b11011001);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("emulationTest TRB", "[TRB]")
{
	Init()
	snes.wram->_data[0] = 0xFF;
	snes.cpu->_registers.a = 0b00110111;
	snes.cpu->_registers.p.m = true;
	snes.cpu->TRB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram->_data[0] == 0b11001000);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nativeTest TRB", "[TRB]")
{
	Init()
	snes.wram->_data[0] = 0xF0;
	snes.wram->_data[1] = 0x0F;
	snes.cpu->_registers.a = 0x0F0F;
	snes.cpu->_registers.p.m = false;
	snes.cpu->TRB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram->_data[0] == 0xF0);
	REQUIRE(snes.wram->_data[1] == 0x00);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("zero TRB", "[TRB]")
{
	Init()
	snes.wram->_data[0] = 0xFF;
	snes.cpu->_registers.a = 0b0;
	snes.cpu->_registers.p.m = true;
	snes.cpu->TRB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram->_data[0] == 0xFF);
	REQUIRE(snes.cpu->_registers.p.z == true);
}