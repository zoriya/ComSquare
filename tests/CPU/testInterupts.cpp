//
// Created by anonymus-raccoon on 1/24/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;

TEST_CASE("BRK_emulated CPU][emulated", "[CPU][emulated]")
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cartridge->header.emulationInterrupts.brk = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->BRK(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.pc == 0x123u);
	REQUIRE(snes.cpu->_registers.pbr == 0x0);
	REQUIRE(snes.cpu->_registers.p.d == false);
	REQUIRE(snes.cpu->_registers.p.i == true);
	REQUIRE(snes.cpu->_registers.p.x_b == true);
	int data = snes.cpu->_pop();
	REQUIRE(data == 0xF1);
	data = snes.cpu->_pop16();
	REQUIRE(data == 0x156u);
}

TEST_CASE("BRK_native CPU][native", "[CPU][native]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cartridge->header.nativeInterrupts.brk = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->BRK(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.pc == 0x123u);
	REQUIRE(snes.cpu->_registers.pbr == 0x0);
	REQUIRE(snes.cpu->_registers.p.d == false);
	REQUIRE(snes.cpu->_registers.p.i == true);
	int data = snes.cpu->_pop();
	REQUIRE(data == 0xF1);
	data = snes.cpu->_pop16();
	REQUIRE(data == 0x156u);
	data = snes.cpu->_pop();
	REQUIRE(data == 0x15);
}

TEST_CASE("COP emulated CPU][emulated", "[CPU][emulated]")
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cartridge->header.emulationInterrupts.cop = 0x123u;
	snes.cpu->_registers.p.flags = 0x0F;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->COP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.pc == 0x123u);
	REQUIRE(snes.cpu->_registers.pbr == 0x0);
	REQUIRE(snes.cpu->_registers.p.d == false);
	REQUIRE(snes.cpu->_registers.p.i == true);
	REQUIRE(snes.cpu->_registers.p.x_b == false);
	int data = snes.cpu->_pop();
	REQUIRE(data == 0x0F);
	data = snes.cpu->_pop16();
	REQUIRE(data == 0x156u);
}

TEST_CASE("COP native CPU][native", "[CPU][native]")
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cartridge->header.nativeInterrupts.cop = 0x123u;
	snes.cpu->_registers.p.flags = 0xF1;
	snes.cpu->_registers.pc = 0x156u;
	snes.cpu->_registers.pbr = 0x15;
	snes.cpu->COP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.pc == 0x123u);
	REQUIRE(snes.cpu->_registers.pbr == 0x0);
	REQUIRE(snes.cpu->_registers.p.d == false);
	REQUIRE(snes.cpu->_registers.p.i == true);
	int data = snes.cpu->_pop();
	REQUIRE(data == 0xF1);
	data = snes.cpu->_pop16();
	REQUIRE(data == 0x156u);
	data = snes.cpu->_pop();
	REQUIRE(data == 0x15);
}