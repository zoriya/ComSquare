//
// Created by anonymus-raccoon on 27/02/20.
//

#include <catch2/catch.hpp>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("removingOne SBC", "[SBC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.a = 0x1;
	snes.wram->_data[0] = 0x1;
	snes.cpu->SBC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0);
	REQUIRE(snes.cpu->_registers.p.c == true);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("legitOverflowWithCarry SBC", "[SBC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.wram->_data[0] = 0x03;
	snes.wram->_data[1] = 0x20;
	snes.cpu->SBC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0xDFFE);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("overflowWithCarry SBC", "[SBC]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.p.c = true;
	snes.wram->_data[0] = 0x03;
	snes.wram->_data[1] = 0x20;
	snes.cpu->SBC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0xDFFE);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("overflowEmulation SBC", "[SBC]")
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.a = 0x1;
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.p.c = false;
	snes.wram->_data[0] = 0x02;
	snes.cpu->SBC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0xFE);
	REQUIRE(snes.cpu->_registers.p.c == false);
	REQUIRE(snes.cpu->_registers.p.v == false);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}


//TEST_CASE("decimal SBC", "[SBC]")
//{
//	Init()
//	snes.cpu->_isEmulationMode = true;
//	snes.cpu->_registers.a = 0x1;
//	snes.cpu->_registers.p.d = true;
//	snes.cpu->_registers.p.m = false;
//	snes.wram->_data[0] = 0x03;
//	snes.wram->_data[1] = 0x20;
//	snes.cpu->SBC(0x0, ComSquare::CPU::AddressingMode::Implied);
//	REQUIRE(snes.cpu->_registers.a == 0x7998);
//	REQUIRE(snes.cpu->_registers.p.c == false);
//	REQUIRE(snes.cpu->_registers.p.v == false);
//	REQUIRE(snes.cpu->_registers.p.n == false);
//	REQUIRE(snes.cpu->_registers.p.z == false);
//}
