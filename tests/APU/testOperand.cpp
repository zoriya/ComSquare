//
// Created by Melefo on 26/02/2020.
//

#include <catch2/catch.hpp>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/APU/APU.hpp"
#include "../../sources/Exceptions/InvalidAddress.hpp"
#include "../../sources/Exceptions/InvalidOpcode.hpp"

using namespace ComSquare;

TEST_CASE("immediate apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0x40);
	REQUIRE(snes.apu._getImmediateData() == 0x40);
}

TEST_CASE("direct apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.p = true;
	snes.apu._internalWrite(0x32, 0x40);
	REQUIRE(snes.apu._getDirectAddr() == 0x140);
}

TEST_CASE("X apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.x = 0x32;
	snes.apu._internalRegisters.p = true;
	REQUIRE(snes.apu._getIndexXAddr() == 0x132);
}

TEST_CASE("Y apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.y = 0x32;
	snes.apu._internalRegisters.p = true;
	REQUIRE(snes.apu._getIndexYAddr() == 0x132);
}

TEST_CASE("directbyX apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.x = 0x03;
	snes.apu._internalWrite(0x32, 0x40);
	REQUIRE(snes.apu._getDirectAddrByX() == 0x43);
}

TEST_CASE("directbyY apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.y = 0x05;
	snes.apu._internalWrite(0x32, 0x40);
	REQUIRE(snes.apu._getDirectAddrByY() == 0x45);
}

TEST_CASE("absolute apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	REQUIRE(snes.apu._getAbsoluteAddr() == 61455);
}

TEST_CASE("absolutebyx apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.x = 10;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(0b1111000000001111 + 10, 255);
	REQUIRE(snes.apu._getAbsoluteByXAddr() == 255);
}

TEST_CASE("absoluteaddrbyx apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.x = 10;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	REQUIRE(snes.apu._getAbsoluteAddrByX() == 61465);
}

TEST_CASE("absoluteaddrbyy apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.y = 10;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	REQUIRE(snes.apu._getAbsoluteAddrByY() == 61465);
}

TEST_CASE("absolutebit apu_get", "[apu_get]")
{
	Init()
	std::pair<uint24_t, uint24_t> result;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	result = snes.apu._getAbsoluteBit();
	REQUIRE(result.first == 4111);
	REQUIRE(result.second == 7);
}

TEST_CASE("absolutebyxdirect apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.p = true;
	snes.apu._internalRegisters.x = 0x10;
	snes.apu._internalWrite(0x32, 0x42);
	snes.apu._internalWrite(0x152, 0b00001101);
	snes.apu._internalWrite(0x253, 0b01101011);
	REQUIRE(snes.apu._getAbsoluteDirectByXAddr() == 0b0110101100001101);
}

TEST_CASE("absolutedirectbyy apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.p = true;
	snes.apu._internalRegisters.y = 0x10;
	snes.apu._internalWrite(0x32, 0x42);
	snes.apu._internalWrite(0x142, 0b00001101);
	snes.apu._internalWrite(0x243, 0b01101011);
	REQUIRE(snes.apu._getAbsoluteDirectAddrByY() == 0b0110101100011101);
}