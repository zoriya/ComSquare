//
// Created by Melefo on 12/02/2020.
//

#include <catch2/catch_test_macros.hpp>
#include "tests.hpp"
#include "SNES.hpp"
#include "APU/APU.hpp"
#include "Exceptions/InvalidAddress.hpp"

using namespace ComSquare;

//////////////////////////////
//							//
// APU::_internalRead tests	//
//							//
//////////////////////////////

TEST_CASE("register internalRead", "[internalRead]")
{
	Init()

	snes.apu._registers.counter0 = 123;
	auto result = snes.apu._internalRead(0x00FD);
	REQUIRE(result == 123);
}

TEST_CASE("Page0 read Read", "[Read]")
{
	Init()
	uint8_t result;

	snes.apu._map.Page0._data[0x0010] = 123;
	result = snes.apu._internalRead(0x0010);
	REQUIRE(result == 123);
}

TEST_CASE("Page1 read Read", "[Read]")
{
	Init()
	uint8_t result = 0;

	snes.apu._map.Page1._data[0x0042] = 123;
	result = snes.apu._internalRead(0x0142);
	REQUIRE(result == 123);
}

TEST_CASE("Memory internalRead", "[internalRead]")
{
	Init()
	uint8_t result = 0;

	snes.apu._map.Memory._data[0xFCDC] = 123;
	result = snes.apu._internalRead(0xFEDC);
	REQUIRE(result == 123);
}

TEST_CASE("IPL internalRead", "[internalRead]")
{
	Init()
	uint8_t result = 0;

	snes.apu._map.IPL._data[0x001F] = 123;
	result = snes.apu._internalRead(0xFFDF);
	REQUIRE(result == 123);
}

TEST_CASE("Invalid internalRead", "[internalRead]")
{
	Init()


	REQUIRE_THROWS_AS(snes.apu._internalRead(0x10000), InvalidAddress);
}

///////////////////////////////
//							 //
// APU::_internalWrite tests //
//							 //
///////////////////////////////

TEST_CASE("Page0 write Write", "[Write]")
{
	Init()


	snes.apu._internalWrite(0x0001, 123);
	REQUIRE(snes.apu._map.Page0._data[0x0001] == 123);
}

TEST_CASE("register write Write", "[Write]")
{
	Init()


	snes.apu._internalWrite(0x00F4, 123);
	REQUIRE(snes.apu._registers.port0 == 123);
}

TEST_CASE("Page1 internalWrite", "[internalWrite]")
{
	Init()


	snes.apu._internalWrite(0x01FF, 123);
	REQUIRE(snes.apu._map.Page1._data[0x00FF] == 123);
}

TEST_CASE("Memory write internalWrite", "[internalWrite]")
{
	Init()


	snes.apu._internalWrite(0x0789, 123);
	REQUIRE(snes.apu._map.Memory._data[0x0589] == 123);
}

TEST_CASE("IPL internalWrite", "[internalWrite]")
{
	Init()


	snes.apu._internalWrite(0xFFF0, 123);
	REQUIRE(snes.apu._map.IPL._data[0x0030] == 123);
}

TEST_CASE("Invalid internalWrite", "[internalWrite]")
{
	Init()


	REQUIRE_THROWS_AS(snes.apu._internalWrite(0x10000, 123), InvalidAddress);
}

/////////////////////
//				   //
// APU::read tests //
//				   //
/////////////////////

TEST_CASE("Valid read", "[read]")
{
	Init()
	uint8_t result = 0;

	snes.apu._registers.port2 = 123;
	result = snes.apu.read(0x02);
	REQUIRE(result == 123);
}

TEST_CASE("Invalid read", "[read]")
{
	Init()


	REQUIRE_THROWS_AS(snes.apu.read(0x10000), InvalidAddress);
}

//////////////////////
//					//
// APU::write tests	//
//					//
//////////////////////

TEST_CASE("Valid write", "[write]")
{
	Init()


	snes.apu.write(0x03, 123);
	REQUIRE(snes.apu._registers.port3 == 123);
}

TEST_CASE("Invalid write", "[write]")
{
	Init()


	REQUIRE_THROWS_AS(snes.apu.write(0x04, 123), InvalidAddress);
}

///////////////////////////////////
//								 //
// APU::executeInstruction tests //
//								 //
///////////////////////////////////

TEST_CASE("Valid executeInstruction", "[executeInstruction]")
{
	Init()
	uint8_t result = 0;

	snes.apu._internalRegisters.pc = 0x00;
	result = snes.apu._executeInstruction();
	REQUIRE(result == 2);
}

///////////////////////
//					 //
// APU::update tests //
//					 //
///////////////////////

TEST_CASE("running update", "[update]")
{
	Init()


	snes.apu._internalRegisters.pc = 0x00;
	snes.apu.update(1);
	REQUIRE(snes.apu._paddingCycles == 1);
}

TEST_CASE("stopped update", "[update]")
{
	Init()


	snes.apu._state = APU::Stopped;
	snes.apu.update(1);
	REQUIRE(snes.apu._paddingCycles == 0);
}

//////////////////////////
//						//
// APU::_get*Addr tests	//
//						//
//////////////////////////

TEST_CASE("direct get", "[get]")
{
	Init()


	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 123);
	REQUIRE(snes.apu._getDirectAddr() == 123);
}

TEST_CASE("absolute get", "[get]")
{
	Init()


	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	REQUIRE(snes.apu._getAbsoluteAddr() == 61455);
}