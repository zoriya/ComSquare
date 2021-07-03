//
// Created by anonymus-raccoon on 4/3/20.
//

#include <catch2/catch.hpp>
#include <bitset>
#include "../../tests.hpp"
#include "../../../sources/SNES.hpp"
using namespace ComSquare;

TEST_CASE("simple DEX", "[DEX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x57;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.x == 0x56);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("overflowEmul DEX", "[DEX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.x == 0xFF);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("fakeOverflowNonEmul DEX", "[DEX]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xFF00;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.x == 0xFEFF);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nonNegative DEX", "[DEX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0x80;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.x == 0x7F);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("zero DEX", "[DEX]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 1;
	snes.cpu->DEX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.x == 0);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("simple DEY", "[DEY]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x57;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.y == 0x56);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("overflowEmul DEY", "[DEY]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.y == 0xFF);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("fakeOverflowNonEmul DEY", "[DEY]")
{
	Init()
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0xFF00;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.y == 0xFEFF);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nonNegative DEY", "[DEY]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0x80;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.y == 0x7F);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("zero DEY", "[DEY]")
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 1;
	snes.cpu->DEY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.y == 0);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("simple ORA", "[ORA]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0x0F;
	snes.cpu->ORA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x8F);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("simple2 ORA", "[ORA]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->ORA(0x00, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0xF0);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("zero ORA", "[ORA]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x00;
	snes.wram->_data[0] = 0x00;
	snes.cpu->ORA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x00);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("simple INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x56;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0x57);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negative INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x7F;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0x80);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("accumulator INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x56;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x57);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negativeAccumulator INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x7F;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x80);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nativeAccumulator INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x5600;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x5601);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negativeNativeAccumulator INC", "[INC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x8FFF;
	snes.cpu->INC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x9000);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("simple DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x58;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0x57);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negative DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.wram->_data[0] = 0x81;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Absolute);
	REQUIRE(snes.wram->_data[0] == 0x80);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("accumulator DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x58;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x57);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negativeAccumulator DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x81;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x80);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("nativeAccumulator DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x5602;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x5601);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("negativeNativeAccumulator DEC", "[DEC]")
{
	Init()
	snes.cpu->_registers.p.m = false;
	snes.cpu->_registers.a = 0x9001;
	snes.cpu->DEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x9000);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("simple EOR", "[EOR]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0x0F;
	snes.cpu->EOR(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x8F);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("simple2 EOR", "[EOR]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->EOR(0x00, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x70);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == false);
}

TEST_CASE("zero EOR", "[EOR]")
{
	Init()
	snes.cpu->_registers.p.m = true;
	snes.cpu->_registers.a = 0x00;
	snes.wram->_data[0] = 0x00;
	snes.cpu->EOR(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x00);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("zero XBA", "[XBA]")
{
	Init()
	snes.cpu->_registers.a = 0x0001;
	snes.cpu->XBA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x0100);
	REQUIRE(snes.cpu->_registers.p.n == false);
	REQUIRE(snes.cpu->_registers.p.z == true);
}

TEST_CASE("negative XBA", "[XBA]")
{
	Init()
	snes.cpu->_registers.a = 0x8001;
	snes.cpu->XBA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu->_registers.a == 0x0180);
	REQUIRE(snes.cpu->_registers.p.n == true);
	REQUIRE(snes.cpu->_registers.p.z == false);
}
