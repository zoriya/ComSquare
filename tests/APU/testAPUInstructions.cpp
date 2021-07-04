//
// Created by Melefo on 11/02/2020.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/APU/APU.hpp"
#include "../../sources/Utility/Utility.hpp"

using namespace ComSquare;

////////////////////Init()\n\tauto apu = snes.apu
//				  //
// Standbys tests //
//				  //
////////////////////

TEST_CASE("NOP Standbys", "[Standbys]")
{
	Init()
	int result = 0;

	result = snes.apu.NOP();
	REQUIRE(result == 2);
}

TEST_CASE("SLEEP Standbys", "[Standbys]")
{
	Init()
	int result = 0;

	result = snes.apu.SLEEP();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._state == APU::Sleeping);
}

TEST_CASE("STOP Standbys", "[Standbys]")
{
	Init()
	int result = 0;

	result = snes.apu.STOP();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._state == APU::Stopped);
}

///////////////
//			 //
// PSW tests //
//			 //
///////////////

TEST_CASE("CLRC PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.CLRC();
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.c == false);
}

TEST_CASE("SETC PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.SETC();
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("NOTC PSW", "[PSW]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.c = false;
	result = snes.apu.NOTC();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("CLRV PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.CLRV();
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.v == false);
	REQUIRE(snes.apu._internalRegisters.h == false);
}

TEST_CASE("CLRP PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.CLRP();
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.p == false);
}

TEST_CASE("SETP PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.SETP();
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.p == true);
}

TEST_CASE("EI PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.EI();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.i == true);
}

TEST_CASE("DI PSW", "[PSW]")
{
	Init()
	int result = 0;

	result = snes.apu.DI();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.i == false);
}

///////////////
//			 //
// Bit tests //
//			 //
///////////////

TEST_CASE("SET1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(snes.apu._getDirectAddr(), 0b00000000);
	snes.apu._internalRegisters.pc--;
	result = snes.apu.SET1(snes.apu._getDirectAddr(), 0);
	snes.apu._internalRegisters.pc--;
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(snes.apu._getDirectAddr()) == 1);
}

TEST_CASE("CLR1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(snes.apu._getDirectAddr(), 0b11111111);
	snes.apu._internalRegisters.pc--;
	result = snes.apu.CLR1(snes.apu._getDirectAddr(), 0);
	snes.apu._internalRegisters.pc--;
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(snes.apu._getDirectAddr()) == 0b11111110);
}

TEST_CASE("TSET1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr(), 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.TSET1(snes.apu._getAbsoluteAddr());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRead(snes.apu._getAbsoluteAddr()) == 0x7B);
	REQUIRE(result == 6);
}

TEST_CASE("TCLR1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x80;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr(), 0x80);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.TCLR1(snes.apu._getAbsoluteAddr());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRead(snes.apu._getAbsoluteAddr()) == 0x00);
	REQUIRE(result == 6);
}

TEST_CASE("AND1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.AND1(snes.apu._getAbsoluteBit());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 4);
}

TEST_CASE("AND1_invert Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.AND1(snes.apu._getAbsoluteBit(), true);
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 4);
}

TEST_CASE("OR1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.OR1(snes.apu._getAbsoluteBit());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("OR1_invert Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.OR1(snes.apu._getAbsoluteBit(), true);
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == true);
	REQUIRE(result == 5);
}

TEST_CASE("EOR1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.EOR1(snes.apu._getAbsoluteBit());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("NOT1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.NOT1(snes.apu._getAbsoluteBit());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("MOV1 Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.MOV1(snes.apu._getAbsoluteBit());
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRead(snes.apu._getAbsoluteAddr() & 0x1FFFu) == 123);
	REQUIRE(result == 6);
}

TEST_CASE("MOV1_carry Bit", "[Bit]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 42;
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr() & 0x1FFFu, 123);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.MOV1(snes.apu._getAbsoluteBit(), true);
	snes.apu._internalRegisters.pc -= 2;
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(result == 4);
}

/////////////////
//			   //
// Stack tests //
//			   //
/////////////////

TEST_CASE("PUSH Stack", "[Stack]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 56;
	result = snes.apu.PUSH(snes.apu._internalRegisters.a);
	snes.apu._internalRegisters.sp++;
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(snes.apu._internalRegisters.sp | 0x100u) == 56);
}

TEST_CASE("POP Stack", "[Stack]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 82);
	snes.apu._internalRegisters.sp--;
	result = snes.apu.POP(snes.apu._internalRegisters.y);
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.y == 82);
}

//////////////////////
//					//
// Subroutine tests //
//					//
//////////////////////

TEST_CASE("CALL Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._getAbsoluteAddr(), 23);
	snes.apu._internalRegisters.pc -= 2;
	result = snes.apu.CALL(snes.apu._getAbsoluteAddr());
	REQUIRE(result == 8);
	REQUIRE(snes.apu._internalRegisters.pc == 23);
	REQUIRE(snes.apu._internalRead(++snes.apu._internalRegisters.sp + 0x0100u) == 2);
	REQUIRE(snes.apu._internalRead(++snes.apu._internalRegisters.sp + 0x0100u) == 0);
}

TEST_CASE("PCALL Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 123);
	result = snes.apu.PCALL();
	REQUIRE(result == 6);
	REQUIRE(snes.apu._internalRegisters.pc == 65403);
}

TEST_CASE("TCALL Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(0xFFD0, 45);
	result = snes.apu.TCALL(7);
	REQUIRE(result == 8);
	REQUIRE(snes.apu._internalRegisters.pc == 45);
}

TEST_CASE("BRK Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pch = 0xFF;
	snes.apu._internalRegisters.pcl = 0xEE;
	snes.apu._internalRegisters.psw = 0xDD;
	snes.apu._internalWrite(0xFFDF, 0xAA);
	snes.apu._internalWrite(0xFFDE, 0xBB);
	result = snes.apu.BRK();
	snes.apu._internalRegisters.sp += 3;
	REQUIRE(result == 8);
	REQUIRE(snes.apu._internalRegisters.i == false);
	REQUIRE(snes.apu._internalRegisters.b == true);
	REQUIRE(snes.apu._internalRegisters.pc == 0xAABB);
	REQUIRE(snes.apu._internalRead(snes.apu._internalRegisters.sp-- | 0x100u) == 0xFF);
	REQUIRE(snes.apu._internalRead(snes.apu._internalRegisters.sp-- | 0x100u) == 0xEE);
	REQUIRE(snes.apu._internalRead(snes.apu._internalRegisters.sp | 0x100u) == 0xDD);
}

TEST_CASE("RET Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 0x12);
	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 0x34);
	snes.apu._internalRegisters.sp -= 2;
	result = snes.apu.RET();
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRegisters.pch == 0x12);
	REQUIRE(snes.apu._internalRegisters.pcl == 0x34);
}

TEST_CASE("RETI Subroutine", "[Subroutine]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 0x12);
	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 0x34);
	snes.apu._internalWrite(++snes.apu._internalRegisters.sp | 0x100u, 0x56);
	snes.apu._internalRegisters.sp -= 3;
	result = snes.apu.RETI();
	REQUIRE(result == 6);
	REQUIRE(snes.apu._internalRegisters.psw == 0x12);
	REQUIRE(snes.apu._internalRegisters.pch == 0x34);
	REQUIRE(snes.apu._internalRegisters.pcl == 0x56);
}

////////////////////////
//					  //
// Program Flow tests //
//					  //
////////////////////////

TEST_CASE("BRA ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 23);
	result = snes.apu.BRA(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 24);
}

TEST_CASE("BEQ ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	result = snes.apu.BEQ(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.z = true;
	result = snes.apu.BEQ(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BNE ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	snes.apu._internalRegisters.z = true;
	result = snes.apu.BNE(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.z = false;
	result = snes.apu.BNE(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BCS ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	result = snes.apu.BCS(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.c = true;
	result = snes.apu.BCS(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BCC ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	snes.apu._internalRegisters.c = true;
	result = snes.apu.BCC(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.c = false;
	result = snes.apu.BCC(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BVS ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	result = snes.apu.BVS(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.v = true;
	result = snes.apu.BVS(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BVC ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	snes.apu._internalRegisters.v = true;
	result = snes.apu.BVC(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.v = false;
	result = snes.apu.BVC(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BMI ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	result = snes.apu.BMI(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.n = true;
	result = snes.apu.BMI(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BPL ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	snes.apu._internalRegisters.n = true;
	result = snes.apu.BPL(snes.apu._getImmediateData());
	REQUIRE(result == 2);
	snes.apu._internalRegisters.n = false;
	result = snes.apu.BPL(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.pc == 25);
}

TEST_CASE("BBS ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 23);
	snes.apu._internalWrite(23, 0);
	result = snes.apu.BBS(snes.apu._getDirectAddr(), snes.apu._getImmediateData(), 2);
	REQUIRE(result == 5);
	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 10);
	snes.apu._internalWrite(23, 100);
	result = snes.apu.BBS(snes.apu._getDirectAddr(), snes.apu._getImmediateData(), 2);
	REQUIRE(result == 7);
	REQUIRE(snes.apu._internalRegisters.pc == 12);
}

TEST_CASE("BBC ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 10);
	snes.apu._internalWrite(10, 255);
	result = snes.apu.BBC(snes.apu._getDirectAddr(), snes.apu._getImmediateData(), 2);
	REQUIRE(result == 5);
	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 10);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 0);
	result = snes.apu.BBC(snes.apu._getDirectAddr(), snes.apu._getImmediateData(), 2);
	REQUIRE(result == 7);
	REQUIRE(snes.apu._internalRegisters.pc == 12);
}

TEST_CASE("CBNE ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalRegisters.a = 4;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 10);
	snes.apu._internalWrite(10, 4);
	result = snes.apu.CBNE(snes.apu._getDirectAddr(), snes.apu._getImmediateData());
	REQUIRE(result == 5);
	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalRegisters.a = 0;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 10);
	result = snes.apu.CBNE(snes.apu._getDirectAddrByX(), snes.apu._getImmediateData(), true);
	REQUIRE(result == 8);
	REQUIRE(snes.apu._internalRegisters.pc == 12);
}

TEST_CASE("DBNZ ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalRegisters.y = 1;
	result = snes.apu.DBNZ(snes.apu._getImmediateData());
	REQUIRE(result == 4);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 10);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc + 1, 5);
	snes.apu._internalWrite(5, 55);
	result = snes.apu.DBNZ(snes.apu._getImmediateData(), true);
	REQUIRE(result == 7);
	REQUIRE(	snes.apu._internalRead(5) == 54);
	REQUIRE(snes.apu._internalRegisters.pc == 13);
}

TEST_CASE("JMP ProgramFlow", "[ProgramFlow]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalWrite(0x32, 0b00001111);
	snes.apu._internalWrite(0x33, 0b11110000);
	result = snes.apu.JMP(snes.apu._getAbsoluteAddr());
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.pc == 61455);
	snes.apu._internalRegisters.pc = 0x32;
	snes.apu._internalRegisters.x = 0b000000001;
	snes.apu._internalWrite(0b1111000000001111 + 1, 0b00010000);
	snes.apu._internalWrite(0b1111000000001111 + 2, 0b11110001);
	result = snes.apu.JMP(snes.apu._getAbsoluteByXAddr(), true);
	REQUIRE(result == 6);
	REQUIRE(snes.apu._internalRegisters.pc == 61712);
}

////////////////////////////////
//							  //
// Decimal Compensation tests //
//							  //
////////////////////////////////

TEST_CASE("DAA DecimalCompensation", "[DecimalCompensation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.c = true;
	snes.apu._internalRegisters.h = true;
	snes.apu._internalRegisters.a = 0x1A;
	result = snes.apu.DAA();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 0x80);
}

TEST_CASE("DAS DecimalCompensation", "[DecimalCompensation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.c = false;
	snes.apu._internalRegisters.h = false;
	snes.apu._internalRegisters.a = 0xFF;
	result = snes.apu.DAS();
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 0x99);
}

///////////////////////////////////
//								 //
// Multiplication Division tests //
//								 //
///////////////////////////////////

TEST_CASE("MUL MultiplicationDivision", "[MultiplicationDivision]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 10;
	snes.apu._internalRegisters.y = 23;
	result = snes.apu.MUL();
	REQUIRE(result == 9);
	REQUIRE(snes.apu._internalRegisters.ya == 230);
}

TEST_CASE("DIV MultiplicationDivision", "[MultiplicationDivision]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.ya = 235;
	snes.apu._internalRegisters.x = 10;
	result = snes.apu.DIV();
	REQUIRE(result == 12);
	REQUIRE(snes.apu._internalRegisters.y == 5);
	REQUIRE(snes.apu._internalRegisters.a == 23);
	snes.apu._internalRegisters.ya = 12345;
	snes.apu._internalRegisters.x = 2;
	result = snes.apu.DIV();
	REQUIRE(snes.apu._internalRegisters.y == 147);
	REQUIRE(snes.apu._internalRegisters.a == 211);
}

//////////////////////////////////
//								//
// (XVI)16-bit Arithmetic tests //
//								//
//////////////////////////////////

TEST_CASE("INCW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xFF);
	snes.apu._internalWrite(0x55 + 1, 0x22);
	result = snes.apu.INCW(snes.apu._getDirectAddr());
	REQUIRE(result == 6);
	REQUIRE(snes.apu._internalRead(0x55) == 0x00);
	REQUIRE(snes.apu._internalRead(0x55 + 1) == 0x23);
}

TEST_CASE("DECW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0x00);
	snes.apu._internalWrite(0x55 + 1, 0x23);
	result = snes.apu.DECW(snes.apu._getDirectAddr());
	REQUIRE(result == 6);
	REQUIRE(snes.apu._internalRead(0x55) == 0xFF);
	REQUIRE(snes.apu._internalRead(0x55 + 1) == 0x22);
}

TEST_CASE("ADDW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.ya = 0x4321;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0x11);
	snes.apu._internalWrite(0x55 + 1, 0x22);
	result = snes.apu.ADDW(snes.apu._getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRegisters.ya == 0x6532);
	REQUIRE(snes.apu._internalRegisters.v == false);
	REQUIRE(snes.apu._internalRegisters.h == false);
	REQUIRE(snes.apu._internalRegisters.c == false);
}

TEST_CASE("SUBW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.ya = 0x4321;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0x11);
	snes.apu._internalWrite(0x55 + 1, 0x22);
	result = snes.apu.SUBW(snes.apu._getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRegisters.ya == 0x2110);
	REQUIRE(snes.apu._internalRegisters.v == false);
	REQUIRE(snes.apu._internalRegisters.h == true);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("CMPW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.ya = 0x2211;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0x11);
	snes.apu._internalWrite(0x55 + 1, 0x22);
	result = snes.apu.CMPW(snes.apu._getDirectAddr());
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

/////////////////////////////////////////
//									   //
// (XVI)16-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

TEST_CASE("MOVW XVIbitDataTransmission", "[XVIbitDataTransmission]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalRegisters.ya = 0x2211;
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	result = snes.apu.MOVW(snes.apu._getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(0x55) == 0x11);
	REQUIRE(snes.apu._internalRead(0x56) == 0x22);
	snes.apu._internalRegisters.ya = 0x0000;
	snes.apu._internalRegisters.pc = 0;
	snes.apu._internalWrite(0x55, 0x33);
	snes.apu._internalWrite(0x55 + 1, 0x44);
	snes.apu.MOVW(snes.apu._getDirectAddr(), true);
	REQUIRE(snes.apu._internalRegisters.ya == 0x4433);
}

//////////////////////////////////////
//									//
// (VIII)8-bit Shift Rotation tests //
//									//
//////////////////////////////////////

TEST_CASE("ASL VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x66;
	result = snes.apu.ASL(snes.apu._internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0xCC);
	REQUIRE(snes.apu._internalRegisters.c == false);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.ASL(snes.apu._getDirectAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(0x55) == 0xBA);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("LSR VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x66;
	result = snes.apu.LSR(snes.apu._internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0x33);
	REQUIRE(snes.apu._internalRegisters.c == false);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.LSR(snes.apu._getDirectAddr(), 5);
	REQUIRE(result  == 5);
	REQUIRE(snes.apu._internalRead(0x55) == 0x6E);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("ROL VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x66;
	result = snes.apu.ROL(snes.apu._internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0xCC);
	REQUIRE(snes.apu._internalRegisters.c == false);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.ROL(snes.apu._getDirectAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(0x55) == 0xBA);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("ROR VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x66;
	result = snes.apu.ROR(snes.apu._internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0x33);
	REQUIRE(snes.apu._internalRegisters.c == false);
	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.ROR(snes.apu._getDirectAddr(), 5);
	REQUIRE(result  == 5);
	REQUIRE(snes.apu._internalRead(0x55) == 0x6E);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("XCN VIIIShiftRotation", "[VIIIShiftRotation]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0b10101010;
	result = snes.apu.XCN();
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRegisters.a == 0xAA);
}

///////////////////////////////////////////
//										 //
// (VIII)8-bit Increment Decrement tests //
//										 //
///////////////////////////////////////////

TEST_CASE("INC VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.INC(snes.apu._getDirectAddr(), 4);
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(0x55) == 0xDE);
}

TEST_CASE("INCreg VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x76;
	result = snes.apu.INCreg(snes.apu._internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0x77);
}

TEST_CASE("DEC VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	int result = 0;

	snes.apu._internalWrite(snes.apu._internalRegisters.pc, 0x55);
	snes.apu._internalWrite(0x55, 0xDD);
	result = snes.apu.DEC(snes.apu._getDirectAddr(), 4);
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(0x55) == 0xDC);
}

TEST_CASE("DECreg VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 0x76;
	result = snes.apu.DECreg(snes.apu._internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 0x75);
}

///////////////////////////////
//							 //
// (VIII)8-bit Logical tests //
//							 //
///////////////////////////////

TEST_CASE("ANDacc VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 24;
	snes.apu._internalWrite(4, 23);
	result = snes.apu.ANDacc(snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 16);
}

TEST_CASE("AND VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalWrite(4, 12);
	snes.apu._internalWrite(7, 44);
	result = snes.apu.AND(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(4) == 12);
}

TEST_CASE("ORacc VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 24;
	snes.apu._internalWrite(4, 23);
	result = snes.apu.ORacc(snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 31);
}

TEST_CASE("OR VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalWrite(4, 12);
	snes.apu._internalWrite(7, 44);
	result = snes.apu.OR(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(4) == 44);
}

TEST_CASE("EORacc VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 24;
	snes.apu._internalWrite(4, 23);
	result = snes.apu.EORacc(snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 15);
}

TEST_CASE("EOR VIIILogical", "[VIIILogical]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalWrite(4, 12);
	snes.apu._internalWrite(7, 44);
	result = snes.apu.EOR(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(4) == 32);
}

//////////////////////////////////
//								//
// (VIII)8-bit Arithmetic tests //
//								//
//////////////////////////////////

TEST_CASE("ADC VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalRegisters.c = true;
	snes.apu._internalWrite(4, 53);
	snes.apu._internalWrite(7, 76);
	result = snes.apu.ADC(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(4) == 130);
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(snes.apu._internalRegisters.h == true);
	REQUIRE(snes.apu._internalRegisters.v == true);
}

TEST_CASE("ADCacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 53;
	snes.apu._internalRegisters.c = true;
	snes.apu._internalWrite(4, 76);
	result = snes.apu.ADCacc(snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 130);
	REQUIRE(snes.apu._internalRegisters.c == false);
	REQUIRE(snes.apu._internalRegisters.h == true);
	REQUIRE(snes.apu._internalRegisters.v == true);
}

TEST_CASE("SBC VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalRegisters.c = true;
	snes.apu._internalWrite(4, 67);
	snes.apu._internalWrite(7, 45);
	result = snes.apu.SBC(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(4) == 22);
	REQUIRE(snes.apu._internalRegisters.c == true);
	REQUIRE(snes.apu._internalRegisters.h == false);
	REQUIRE(snes.apu._internalRegisters.v == false);
}

TEST_CASE("SBCacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 67;
	snes.apu._internalRegisters.c = true;
	snes.apu._internalWrite(4, 45);
	result = snes.apu.SBCacc(snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.a == 22);
	REQUIRE(snes.apu._internalRegisters.c == true);
	REQUIRE(snes.apu._internalRegisters.h == false);
	REQUIRE(snes.apu._internalRegisters.v == false);
}

TEST_CASE("CMP VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.y = 7;
	snes.apu._internalWrite(4, 67);
	snes.apu._internalWrite(7, 45);
	result = snes.apu.CMP(snes.apu._getIndexXAddr(), snes.apu._getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

TEST_CASE("CMPacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 4;
	snes.apu._internalRegisters.a = 67;
	snes.apu._internalWrite(4, 45);
	result = snes.apu.CMPreg(snes.apu._internalRegisters.a, snes.apu._getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(snes.apu._internalRegisters.c == true);
}

/////////////////////////////////////////
//									   //
// (VIII)8-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

TEST_CASE("MovRegToReg VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.a = 23;
	snes.apu._internalRegisters.x = 45;
	result = snes.apu.MOV(snes.apu._internalRegisters.x, snes.apu._internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(snes.apu._internalRegisters.a == 45);
}

TEST_CASE("MovMemToMem VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.pc = 0x23;
	snes.apu._internalWrite(0x23, 0x56);
	snes.apu._internalWrite(0x24, 0x33);
	snes.apu._internalWrite(0x56, 99);
	snes.apu._internalWrite(0x33, 66);
	result = snes.apu.MOV(snes.apu._getDirectAddr(), snes.apu._getImmediateData());
	REQUIRE(result == 5);
	REQUIRE(snes.apu._internalRead(0x33) == 0x56);
}

TEST_CASE("MovRegToMem VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 0x23;
	snes.apu._internalRegisters.a = 0x44;
	snes.apu._internalWrite(0x23, 0x56);
	result = snes.apu.MOV(snes.apu._internalRegisters.a, snes.apu._getIndexXAddr(), 4, true);
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRead(0x23) == 0x44);
	REQUIRE(snes.apu._internalRegisters.x == 0x24);
}

TEST_CASE("MovMemToReg VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	int result = 0;

	snes.apu._internalRegisters.x = 0x23;
	snes.apu._internalRegisters.a = 0x44;
	result = snes.apu.MOV(snes.apu._getIndexXAddr(), snes.apu._internalRegisters.a, 4, true);
	REQUIRE(result == 4);
	REQUIRE(snes.apu._internalRegisters.x == 0x24);
	REQUIRE(snes.apu._internalRegisters.a == 0x23);
}