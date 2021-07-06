//
// Created by anonymus-raccoon on 2/28/20.
//

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("16bitsTo16Bits TAX", "[TAX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.x = 0xABCD;
	snes.cpu._registers.a = 0xFEDC;
	snes.cpu.TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0xFEDC);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("16bitsTo8Bits TAX", "[TAX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.x = 0xFEDC;
	snes.cpu._registers.a = 0xAB00;
	snes.cpu.TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0xFE00);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("8bitsTo16Bits TAX", "[TAX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.x = 0xFEDC;
	snes.cpu._registers.a = 0xAB;
	snes.cpu.TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x00AB);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("8bitsTo8Bits TAX", "[TAX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.x = 0xFE;
	snes.cpu._registers.a = 0xAB;
	snes.cpu.TAX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0xAB);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}


TEST_CASE("16bitsTo16Bits TAY", "[TAY]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.y = 0xABCD;
	snes.cpu._registers.a = 0xFEDC;
	snes.cpu.TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0xFEDC);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("16bitsTo8Bits TAY", "[TAY]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.y = 0xFEDC;
	snes.cpu._registers.a = 0xAB00;
	snes.cpu.TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0xFE00);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("8bitsTo16Bits TAY", "[TAY]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.y = 0xFEDC;
	snes.cpu._registers.a = 0xAB;
	snes.cpu.TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0x00AB);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("8bitsTo8Bits TAY", "[TAY]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.y = 0xFE;
	snes.cpu._registers.a = 0xAB;
	snes.cpu.TAY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0xAB);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("16bitsIndex TXS", "[TXS]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0xABCD;
	snes.cpu.TXS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0xABCD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("8bitsIndex TXS", "[TXS]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.x = 0xABCD;
	snes.cpu.TXS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0x00CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("emulationMode TCD", "[TCD]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.d = 0x5656;
	snes.cpu._registers.a = 0xABCD;
	snes.cpu.TCD(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.d == 0xABCD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("zero TCD", "[TCD]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.d = 0x5656;
	snes.cpu._registers.a = 0x0;
	snes.cpu.TCD(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.d == 0);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("emulationMode TCS", "[TCS]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.s = 0x0156;
	snes.cpu._registers.a = 0xABCD;
	snes.cpu.TCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0x01CD);
}

TEST_CASE("native TCS", "[TCS]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.s = 0x0156;
	snes.cpu._registers.a = 0xABCD;
	snes.cpu.TCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0xABCD);
}

TEST_CASE("emulationMode TDC", "[TDC]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.d = 0xABCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TDC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0xABCD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("zero TDC", "[TDC]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.d = 0x0;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TDC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("emulationMode TSC", "[TSC]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.s = 0xABCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TSC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0xABCD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("zero TSC", "[TSC]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.s = 0x0;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TSC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == true);
}

TEST_CASE("emulationMode TSX", "[TSX]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.s = 0xABCD;
	snes.cpu._registers.x = 0x5656;
	snes.cpu.TSX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x00CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("native TSX", "[TSX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.s = 0x8F00;
	snes.cpu._registers.x = 0x5656;
	snes.cpu.TSX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x8F00);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("double8bits TXA", "[TXA]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.x = 0xABCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("index8bits TXA", "[TXA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.x = 0x0BCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x00CD);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("accumulator8bits TXA", "[TXA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0x0BCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("double16bits TXA", "[TXA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0xAB0D;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TXA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0xAB0D);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}


TEST_CASE("double8bits TYA", "[TYA]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.y = 0xABCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("index8bits TYA", "[TYA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.y = 0x0BCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x00CD);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("accumulator8bits TYA", "[TYA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = true;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.y = 0x0BCD;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("double16bits TYA", "[TYA]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.m = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.y = 0xAB0D;
	snes.cpu._registers.a = 0x5656;
	snes.cpu.TYA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.a == 0xAB0D);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("emulationMode TXY", "[TXY]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.x = 0x0BCD;
	snes.cpu._registers.y = 0x5656;
	snes.cpu.TXY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("nativeMode TXY", "[TXY]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0xAB0D;
	snes.cpu._registers.y = 0x5656;
	snes.cpu.TXY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0xAB0D);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("emulationMode TYX", "[TYX]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.y = 0x0BCD;
	snes.cpu._registers.x = 0x5656;
	snes.cpu.TYX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x56CD);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("nativeMode TYX", "[TYX]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.y = 0xAB0D;
	snes.cpu._registers.x = 0x5656;
	snes.cpu.TYX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0xAB0D);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.z == false);
}

TEST_CASE("hardCase MVN", "[MVN]")
{
	Init()
	snes.cpu._registers.a = 0x10;
	snes.cpu._registers.x = 0x0000;
	snes.cpu._registers.y = 0x1000;
	for (int i = 0; i <= snes.cpu._registers.a; i++)
		snes.wram._data[i] = i;

	int cycles = snes.cpu.MVN(0x2010, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(cycles == 0x77);
	REQUIRE(snes.cpu._registers.dbr == 0x20);
	REQUIRE(snes.cpu._registers.a == 0xFFFF);
	REQUIRE(snes.cpu._registers.x == 0x0011);
	REQUIRE(snes.cpu._registers.y == 0x1011);
	for (int i = 0; i < 0x11; i++)
		REQUIRE(snes.wram._data[i + 0x1000] == i);
}

TEST_CASE("hardCase MVP", "[MVP]")
{
	Init()
	snes.cpu._registers.a = 0x10;
	snes.cpu._registers.x = 0x0010;
	snes.cpu._registers.y = 0x1010;
	for (int i = 0; i <= snes.cpu._registers.a; i++)
		snes.wram._data[i] = i;

	int cycles = snes.cpu.MVP(0x2010, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(cycles == 0x77);
	REQUIRE(snes.cpu._registers.dbr == 0x20);
	REQUIRE(snes.cpu._registers.a == 0xFFFF);
	REQUIRE(snes.cpu._registers.x == 0xFFFF);
	REQUIRE(snes.cpu._registers.y == 0x0FFF);
	for (int i = 0; i < 0x11; i++)
		REQUIRE(snes.wram._data[i + 0x1000] == i);
}