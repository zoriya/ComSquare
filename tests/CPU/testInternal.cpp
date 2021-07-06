//
// Created by anonymus-raccoon on 2/13/20.
//

#include <catch2/catch_test_macros.hpp>
#include <bitset>
#include "../tests.hpp"
using namespace ComSquare;

TEST_CASE("setall SEP", "[SEP]")
{
	Init()
	snes.wram._data[0] = 0xFF;
	snes.cpu.SEP(0x00, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.p.flags;
	REQUIRE(data == 0xFF);
}

TEST_CASE("setsome SEP", "[SEP]")
{
	Init()
	snes.cpu._registers.p.flags = 0b01000000;
	snes.wram._data[0] = 0b10110101;
	snes.cpu.SEP(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.p.flags;
	REQUIRE(data == 0b11110101);
}

TEST_CASE("resetall REP", "[REP]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.wram._data[0] = 0xFF;
	snes.cpu.REP(0x00, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.flags == 0x00);
}

TEST_CASE("resetsome REP", "[REP]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.flags = 0b01000000;
	snes.wram._data[0] = 0b01000000;
	snes.cpu.REP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.flags == 0x0);
}

TEST_CASE("resetallEmulation REP", "[REP]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.wram._data[0] = 0xFF;
	snes.cpu.REP(0x00, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.flags == 0b00110000);
}

TEST_CASE("resetsomeEmulation REP", "[REP]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0b01000101;
	snes.wram._data[0] = 0b01000001;
	snes.cpu.REP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.flags == 0b00110100);
}

TEST_CASE("jump JSR", "[JSR]")
{
	Init()
	snes.cpu._registers.pc = 0xABCD;
	snes.cpu._registers.s = 0x0123;
	snes.cpu.JSR(0xABFF, ComSquare::CPU::AddressingMode::Implied);
	auto pc = snes.cpu._registers.pc;
	REQUIRE(pc == 0xABFF);
	REQUIRE(snes.cpu._registers.s == 0x0121);
	auto pushed = snes.cpu._pop16();
	REQUIRE(pushed == 0xABCC);
}

TEST_CASE("jump JSL", "[JSL]")
{
	Init()
	snes.cpu._registers.pbr = 0xFF;
	snes.cpu._registers.pc = 0xABCD;
	snes.cpu._registers.s = 0x0123;
	snes.cpu.JSL(0xCDABFF, ComSquare::CPU::AddressingMode::Implied);
	auto pac = snes.cpu._registers.pac;
	REQUIRE(pac == 0xCDABFF);
	REQUIRE(snes.cpu._registers.s == 0x0120);
	auto pushed = snes.cpu._pop16() + (snes.cpu._pop() << 16u);
	REQUIRE(pushed == 0xFFABCC);
}

TEST_CASE("basic PHA", "[PHA]")
{
	Init()
	snes.cpu._registers.a = 0xABCD;
	snes.cpu._registers.s = 0x02;
	snes.cpu._registers.p.m = false;
	snes.cpu.PHA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[1] == 0xCD);
	REQUIRE(snes.wram._data[2] == 0xAB);
	REQUIRE(snes.cpu._registers.s == 0x0);
}

TEST_CASE("8bits PHA", "[PHA]")
{
	Init()
	snes.cpu._registers.a = 0xCD;
	snes.cpu._registers.s = 0x02;
	snes.cpu._registers.p.m = false;
	snes.cpu.PHA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[1] == 0xCD);
	REQUIRE(snes.cpu._registers.s == 0x0);
}

TEST_CASE("basic PHB", "[PHB]")
{
	Init()
	snes.cpu._registers.dbr = 0xFF;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHB(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[2] == 0xFF);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("basic PHD", "[PHD]")
{
	Init()
	snes.cpu._registers.d = 0xABCD;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHD(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[1] == 0xCD);
	REQUIRE(snes.wram._data[2] == 0xAB);
	REQUIRE(snes.cpu._registers.s == 0x0);
}

TEST_CASE("basic PHK", "[PHK]")
{
	Init()
	snes.cpu._registers.pbr = 0xFF;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHK(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[2] == 0xFF);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("basic PHP", "[PHP]")
{
	Init()
	snes.cpu._registers.p.flags = 0xFF;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHP(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[2] == 0xFF);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("basic PHX", "[PHX]")
{
	Init()
	snes.cpu._registers.x = 0xABCD;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[1] == 0xCD);
	REQUIRE(snes.wram._data[2] == 0xAB);
	REQUIRE(snes.cpu._registers.s == 0x0);
}

TEST_CASE("basic PHY", "[PHY]")
{
	Init()
	snes.cpu._registers.y = 0xABCD;
	snes.cpu._registers.s = 0x02;
	snes.cpu.PHY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.wram._data[1] == 0xCD);
	REQUIRE(snes.wram._data[2] == 0xAB);
	REQUIRE(snes.cpu._registers.s == 0x0);
}

TEST_CASE("basic PLA", "[PLA]")
{
	Init()
	snes.wram._data[1] = 0xCD;
	snes.wram._data[2] = 0x7B;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.m = false;
	snes.cpu.PLA(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.a;
	REQUIRE(data == 0x7BCD);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("zero PLA", "[PLA]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.m = false;
	snes.cpu.PLA(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.a;
	REQUIRE(data == 0x0000);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("negative PLA", "[PLA]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0xA0;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.m = false;
	snes.cpu.PLA(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.a;
	REQUIRE(data == 0xA000);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("basic PLX", "[PLX]")
{
	Init()
	snes.wram._data[1] = 0xCD;
	snes.wram._data[2] = 0x7B;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.x;
	REQUIRE(data == 0x7BCD);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("zero PLX", "[PLX]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.x;
	REQUIRE(data == 0x0000);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("negative PLX", "[PLX]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0xA0;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLX(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.x;
	REQUIRE(data == 0xA000);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("basic PLY", "[PLY]")
{
	Init()
	snes.wram._data[1] = 0xCD;
	snes.wram._data[2] = 0x7B;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLY(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.y;
	REQUIRE(data == 0x7BCD);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("zero PLY", "[PLY]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLY(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.y;
	REQUIRE(data == 0x0000);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("negative PLY", "[PLY]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0xA0;
	snes.cpu._registers.s = 0x00;
	snes.cpu._registers.p.x_b = false;
	snes.cpu.PLY(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.y;
	REQUIRE(data == 0xA000);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("basic PLD", "[PLD]")
{
	Init()
	snes.wram._data[1] = 0xCD;
	snes.wram._data[2] = 0x7B;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLD(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.d;
	REQUIRE(data == 0x7BCD);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("zero PLD", "[PLD]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLD(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.d;
	REQUIRE(data == 0x0000);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("negative PLD", "[PLD]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.wram._data[2] = 0xA0;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLD(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.d;
	REQUIRE(data == 0xA000);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.s == 0x2);
}

TEST_CASE("basic PLB", "[PLB]")
{
	Init()
	snes.wram._data[1] = 0x7D;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLB(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.dbr;
	REQUIRE(data == 0x7D);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("zero PLB", "[PLB]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLB(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.dbr;
	REQUIRE(data == 0x00);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("negative PLB", "[PLB]")
{
	Init()
	snes.wram._data[1] = 0xA0;
	snes.cpu._registers.s = 0x00;
	snes.cpu.PLB(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.dbr;
	REQUIRE(data == 0xA0);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("basic PLP", "[PLP]")
{
	Init()
	snes.wram._data[1] = 0x7D;
	snes.cpu._registers.s = 0x00;
	snes.cpu._isEmulationMode = false;
	snes.cpu.PLP(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.p.flags;
	REQUIRE(data == 0x7D);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("emulation PLP", "[PLP]")
{
	Init()
	snes.wram._data[1] = 0x00;
	snes.cpu._registers.s = 0x00;
	snes.cpu._isEmulationMode = true;
	snes.cpu.PLP(0x0, ComSquare::CPU::AddressingMode::Implied);
	auto data = snes.cpu._registers.p.flags;
	REQUIRE(data == 0b00110000);
	REQUIRE(snes.cpu._registers.s == 0x1);
}

TEST_CASE("clear CLC", "[CLC]")
{
	Init()
	snes.cpu._registers.p.flags = 0xFF;
	snes.cpu.CLC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == false);
}

TEST_CASE("clear CLI", "[CLI]")
{
	Init()
	snes.cpu._registers.p.flags = 0xFF;
	snes.cpu.CLI(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.i == false);
}

TEST_CASE("clear CLD", "[CLD]")
{
	Init()
	snes.cpu._registers.p.flags = 0xFF;
	snes.cpu.CLD(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.d == false);
}

TEST_CASE("clear CLV", "[CLV]")
{
	Init()
	snes.cpu._registers.p.flags = 0xFF;
	snes.cpu.CLV(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.v == false);
}

TEST_CASE("set SEC", "[SEC]")
{
	Init()
	snes.cpu._registers.p.flags = 0x00;
	snes.cpu.SEC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.c == true);
}

TEST_CASE("set SEI", "[SEI]")
{
	Init()
	snes.cpu._registers.p.flags = 0x00;
	snes.cpu.SEI(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.i == true);
}

TEST_CASE("set SED", "[SED]")
{
	Init()
	snes.cpu._registers.p.flags = 0x00;
	snes.cpu.SED(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.d == true);
}

TEST_CASE("enableEmulation XCE", "[XCE]")
{
	Init()
	snes.cpu._isEmulationMode = false;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.c = true;
	snes.cpu._registers.xh = 0xFF;
	snes.cpu._registers.yh = 0xFF;
	snes.cpu.XCE(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._isEmulationMode == true);
	REQUIRE(snes.cpu._registers.p.c == false);
	REQUIRE(snes.cpu._registers.p.m == false);
	REQUIRE(snes.cpu._registers.p.x_b == false);
	REQUIRE(snes.cpu._registers.xh == 0xFF);
	REQUIRE(snes.cpu._registers.yh == 0xFF);
}

TEST_CASE("enableNative XCE", "[XCE]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.xh = 0xFF;
	snes.cpu._registers.yh = 0xFF;
	snes.cpu.XCE(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._isEmulationMode == false);
	REQUIRE(snes.cpu._registers.p.c == true);
	REQUIRE(snes.cpu._registers.p.m == true);
	REQUIRE(snes.cpu._registers.p.x_b == true);
	REQUIRE(snes.cpu._registers.xh == 0);
	REQUIRE(snes.cpu._registers.yh == 0);
}

TEST_CASE("basic INX", "[INX]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0xFF;
	snes.cpu.INX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x0100);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
}

TEST_CASE("8bits INX", "[INX]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.x = 0xFF;
	snes.cpu.INX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.x == 0x00);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
}

TEST_CASE("basic INY", "[INY]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.y = 0xFF;
	snes.cpu.INY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0x0100);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == false);
}

TEST_CASE("8bits INY", "[INY]")
{
	Init()
	snes.cpu._isEmulationMode = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.y = 0xFF;
	snes.cpu.INY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.y == 0x00);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
}

TEST_CASE("basic CPX", "[CPX]")
{
	Init()
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.x = 0xFF;
	snes.wram._data[0] = 0xFF;
	snes.cpu.CPX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.c == true);
}

TEST_CASE("negative CPX", "[CPX]")
{
	Init()
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.x = 0x80;
	snes.wram._data[0] = 0xFF;
	snes.cpu.CPX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.c == false);
}

TEST_CASE("16bits CPX", "[CPX]")
{
	Init()
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.p.x_b = false;
	snes.cpu._registers.x = 0x8888;
	snes.wram._data[0] = 0x88;
	snes.wram._data[1] = 0x98;
	snes.cpu.CPX(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.c == false);
}

TEST_CASE("basic CPY", "[CPY]")
{
	Init()
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.y = 0xFF;
	snes.wram._data[0] = 0xFF;
	snes.cpu.CPY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.z == true);
	REQUIRE(snes.cpu._registers.p.n == false);
	REQUIRE(snes.cpu._registers.p.c == true);
}

TEST_CASE("negative CPY", "[CPY]")
{
	Init()
	snes.cpu._registers.p.x_b = true;
	snes.cpu._registers.p.flags = 0;
	snes.cpu._registers.y = 0x80;
	snes.wram._data[0] = 0xFF;
	snes.cpu.CPY(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.p.z == false);
	REQUIRE(snes.cpu._registers.p.n == true);
	REQUIRE(snes.cpu._registers.p.c == false);
}

TEST_CASE("basic BCC", "[BCC]")
{
	Init()
	snes.cpu._registers.p.c = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BCC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BCC", "[BCC]")
{
	Init()
	snes.cpu._registers.p.c = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BCC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BCC", "[BCC]")
{
	Init()
	snes.cpu._registers.p.c = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BCC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BCS", "[BCS]")
{
	Init()
	snes.cpu._registers.p.c = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BCS", "[BCS]")
{
	Init()
	snes.cpu._registers.p.c = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BCS", "[BCS]")
{
	Init()
	snes.cpu._registers.p.c = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BCS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BEQ", "[BEQ]")
{
	Init()
	snes.cpu._registers.p.z = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BEQ(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BEQ", "[BEQ]")
{
	Init()
	snes.cpu._registers.p.z = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BEQ(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BEQ", "[BEQ]")
{
	Init()
	snes.cpu._registers.p.z = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BEQ(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BNE", "[BNE]")
{
	Init()
	snes.cpu._registers.p.z = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BNE(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BNE", "[BNE]")
{
	Init()
	snes.cpu._registers.p.z = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BNE(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BNE", "[BNE]")
{
	Init()
	snes.cpu._registers.p.z = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BNE(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BMI", "[BMI]")
{
	Init()
	snes.cpu._registers.p.n = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BMI(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BMI", "[BMI]")
{
	Init()
	snes.cpu._registers.p.n = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BMI(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BMI", "[BMI]")
{
	Init()
	snes.cpu._registers.p.n = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BMI(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BPL", "[BPL]")
{
	Init()
	snes.cpu._registers.p.n = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BPL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BPL", "[BPL]")
{
	Init()
	snes.cpu._registers.p.n = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BPL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BPL", "[BPL]")
{
	Init()
	snes.cpu._registers.p.n = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BPL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("basic BRA", "[BRA]")
{
	Init()
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BRA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BRA", "[BRA]")
{
	Init()
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BRA(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("basic BRL", "[BRL]")
{
	Init()
	snes.cpu._registers.pc = 0x8080;
	snes.wram._data[0] = 0x00;
	snes.wram._data[1] = 0x10;
	snes.cpu.BRL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x9080);
}

TEST_CASE("negativeJump BRL", "[BRL]")
{
	Init()
	snes.cpu._registers.pc = 0x8080;
	snes.wram._data[0] = 0x00;
	snes.wram._data[1] = 0xF0;
	snes.cpu.BRL(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x7080);
}

TEST_CASE("basic BVC", "[BVC]")
{
	Init()
	snes.cpu._registers.p.v = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BVC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BVC", "[BVC]")
{
	Init()
	snes.cpu._registers.p.v = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BVC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BVC", "[BVC]")
{
	Init()
	snes.cpu._registers.p.v = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BVC(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}


TEST_CASE("basic BVS", "[BVS]")
{
	Init()
	snes.cpu._registers.p.v = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x50;
	snes.cpu.BVS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0xD0);
}

TEST_CASE("negativeJump BVS", "[BVS]")
{
	Init()
	snes.cpu._registers.p.v = true;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0xF0;
	snes.cpu.BVS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x70);
}

TEST_CASE("noJump BVS", "[BVS]")
{
	Init()
	snes.cpu._registers.p.v = false;
	snes.cpu._registers.pc = 0x80;
	snes.wram._data[0] = 0x90;
	snes.cpu.BVS(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x80);
}

TEST_CASE("simpleJump JMP", "[JMP]")
{
	Init()
	snes.cpu._registers.pc = 0x8000;
	snes.cpu.JMP(0x1000, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pc == 0x1000);
}

TEST_CASE("simpleJump JML", "[JML]")
{
	Init()
	snes.cpu._registers.pc = 0x8000;
	snes.cpu.JML(0x10AB00, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.pac == 0x10AB00);
}

TEST_CASE("simple PER", "[PER]")
{
	Init()
	snes.cpu._registers.pac = 0x008005;
	snes.cpu._registers.s = 0x1FFF;
	snes.wram._data[0x0] = 0xFF;
	snes.wram._data[0x1] = 0xFF;
	snes.cpu.PER(0x0, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0x1FFD);
	uint16_t value = snes.cpu._pop16();
	REQUIRE(value == 0x8004);
}

TEST_CASE("simple PEI", "[PEI]")
{
	Init()
	snes.cpu._registers.s = 0x1FFF;
	snes.cpu.PEI(0xFFFF, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0x1FFD);
	uint16_t value = snes.cpu._pop16();
	REQUIRE(value == 0xFFFF);
}

TEST_CASE("simple PEA", "[PEA]")
{
	Init()
	snes.cpu._registers.s = 0x1FFF;
	snes.wram._data[0x0] = 0xFF;
	snes.wram._data[0x1] = 0xFF;
	snes.cpu.PEA(0xFFFF, ComSquare::CPU::AddressingMode::Implied);
	REQUIRE(snes.cpu._registers.s == 0x1FFD);
	uint16_t value = snes.cpu._pop16();
	REQUIRE(value == 0xFFFF);
}