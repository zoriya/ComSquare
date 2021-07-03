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

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0x40);
	REQUIRE(apu->_getImmediateData() == 0x40);
}

TEST_CASE("direct apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalWrite(0x32, 0x40);
	REQUIRE(apu->_getDirectAddr() == 0x140);
}

TEST_CASE("X apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.x = 0x32;
	apu->_internalRegisters.p = true;
	REQUIRE(apu->_getIndexXAddr() == 0x132);
}

TEST_CASE("Y apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.y = 0x32;
	apu->_internalRegisters.p = true;
	REQUIRE(apu->_getIndexYAddr() == 0x132);
}

TEST_CASE("directbyX apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 0x03;
	apu->_internalWrite(0x32, 0x40);
	REQUIRE(apu->_getDirectAddrByX() == 0x43);
}

TEST_CASE("directbyY apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.y = 0x05;
	apu->_internalWrite(0x32, 0x40);
	REQUIRE(apu->_getDirectAddrByY() == 0x45);
}

TEST_CASE("absolute apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	REQUIRE(apu->_getAbsoluteAddr() == 61455);
}

TEST_CASE("absolutebyx apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(0b1111000000001111 + 10, 255);
	REQUIRE(apu->_getAbsoluteByXAddr() == 255);
}

TEST_CASE("absoluteaddrbyx apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	REQUIRE(apu->_getAbsoluteAddrByX() == 61465);
}

TEST_CASE("absoluteaddrbyy apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.y = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	REQUIRE(apu->_getAbsoluteAddrByY() == 61465);
}

TEST_CASE("absolutebit apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;
	std::pair<uint24_t, uint24_t> result;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	result = apu->_getAbsoluteBit();
	REQUIRE(result.first == 4111);
	REQUIRE(result.second == 7);
}

TEST_CASE("absolutebyxdirect apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalRegisters.x = 0x10;
	apu->_internalWrite(0x32, 0x42);
	apu->_internalWrite(0x152, 0b00001101);
	apu->_internalWrite(0x253, 0b01101011);
	REQUIRE(apu->_getAbsoluteDirectByXAddr() == 0b0110101100001101);
}

TEST_CASE("absolutedirectbyy apu_get", "[apu_get]")
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalRegisters.y = 0x10;
	apu->_internalWrite(0x32, 0x42);
	apu->_internalWrite(0x142, 0b00001101);
	apu->_internalWrite(0x243, 0b01101011);
	REQUIRE(apu->_getAbsoluteDirectAddrByY() == 0b0110101100011101);
}