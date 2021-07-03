//
// Created by anonymus-raccoon on 2/12/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("8bits STA", "[STA]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x11;
	snes.cpu->STA(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0];
	REQUIRE(data == 0x11);
}

TEST_CASE("16bits STA", "[STA]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x11AB;
	snes.cpu->STA(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	REQUIRE(data == 0x11AB);
}

TEST_CASE("8bits STX", "[STX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x11;
	snes.cpu->STX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0];
	REQUIRE(data == 0x11);
}

TEST_CASE("16bits STX", "[STX]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0x11AB;
	snes.cpu->STX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	REQUIRE(data == 0x11AB);
}

TEST_CASE("8bits STY", "[STY]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x11;
	snes.cpu->STY(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0];
	REQUIRE(data == 0x11);
}

TEST_CASE("16bits STY", "[STY]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0x11AB;
	snes.cpu->STY(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	REQUIRE(data == 0x11AB);
}

TEST_CASE("8bits STZ", "[STZ]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x11;
	snes.cpu->STZ(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0];
	REQUIRE(data == 0x00);
}

TEST_CASE("16bits STZ", "[STZ]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.wram->_data[0] = 0x11;
	snes.wram->_data[1] = 0x11;
	snes.cpu->STZ(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.wram->_data[0] + (snes.wram->_data[1] << 8u);
	REQUIRE(data == 0x00);
}

TEST_CASE("8bits LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x01;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x01);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("8bitsNegative LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x11;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x11);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
}

TEST_CASE("8bitsZero LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x11;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x00);
	REQUIRE(snes.cpu->_registers.p.z == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("16bits LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0xAB;
	snes.wram->_data[1] = 001;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x01AB);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
}

TEST_CASE("16bitsNegative LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0xAB;
	snes.wram->_data[1] = 0x11;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x11AB);
	REQUIRE(snes.cpu->_registers.p.z == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
}

TEST_CASE("16bitsZero LDX", "[LDX]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x00;
	snes.cpu->LDX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu->_registers.x;
	REQUIRE(data == 0x0000);
	REQUIRE(snes.cpu->_registers.p.z == true);
	REQUIRE(snes.cpu->_registers.p.n == false);
}