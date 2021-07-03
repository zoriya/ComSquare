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
	auto apu = snes.apu;
	int result = 0;

	result = apu->NOP();
	REQUIRE(result == 2);
}

TEST_CASE("SLEEP Standbys", "[Standbys]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SLEEP();
	REQUIRE(result == 3);
	REQUIRE(apu->_state == APU::Sleeping);
}

TEST_CASE("STOP Standbys", "[Standbys]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->STOP();
	REQUIRE(result == 3);
	REQUIRE(apu->_state == APU::Stopped);
}

///////////////
//			 //
// PSW tests //
//			 //
///////////////

TEST_CASE("CLRC PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRC();
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.c == false);
}

TEST_CASE("SETC PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SETC();
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("NOTC PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	result = apu->NOTC();
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("CLRV PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRV();
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.v == false);
	REQUIRE(apu->_internalRegisters.h == false);
}

TEST_CASE("CLRP PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRP();
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.p == false);
}

TEST_CASE("SETP PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SETP();
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.p == true);
}

TEST_CASE("EI PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->EI();
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.i == true);
}

TEST_CASE("DI PSW", "[PSW]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->DI();
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.i == false);
}

///////////////
//			 //
// Bit tests //
//			 //
///////////////

TEST_CASE("SET1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(apu->_getDirectAddr(), 0b00000000);
	apu->_internalRegisters.pc--;
	result = apu->SET1(apu->_getDirectAddr(), 0);
	apu->_internalRegisters.pc--;
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(apu->_getDirectAddr()) == 1);
}

TEST_CASE("CLR1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(apu->_getDirectAddr(), 0b11111111);
	apu->_internalRegisters.pc--;
	result = apu->CLR1(apu->_getDirectAddr(), 0);
	apu->_internalRegisters.pc--;
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(apu->_getDirectAddr()) == 0b11111110);
}

TEST_CASE("TSET1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr(), 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->TSET1(apu->_getAbsoluteAddr());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRead(apu->_getAbsoluteAddr()) == 0x7B);
	REQUIRE(result == 6);
}

TEST_CASE("TCLR1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x80;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr(), 0x80);
	apu->_internalRegisters.pc -= 2;
	result = apu->TCLR1(apu->_getAbsoluteAddr());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRead(apu->_getAbsoluteAddr()) == 0x00);
	REQUIRE(result == 6);
}

TEST_CASE("AND1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->AND1(apu->_getAbsoluteBit());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(result == 4);
}

TEST_CASE("AND1_invert Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->AND1(apu->_getAbsoluteBit(), true);
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(result == 4);
}

TEST_CASE("OR1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->OR1(apu->_getAbsoluteBit());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("OR1_invert Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->OR1(apu->_getAbsoluteBit(), true);
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == true);
	REQUIRE(result == 5);
}

TEST_CASE("EOR1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->EOR1(apu->_getAbsoluteBit());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("NOT1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->NOT1(apu->_getAbsoluteBit());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(result == 5);
}

TEST_CASE("MOV1 Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->MOV1(apu->_getAbsoluteBit());
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRead(apu->_getAbsoluteAddr() & 0x1FFFu) == 123);
	REQUIRE(result == 6);
}

TEST_CASE("MOV1_carry Bit", "[Bit]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 42;
	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	apu->_internalWrite(apu->_getAbsoluteAddr() & 0x1FFFu, 123);
	apu->_internalRegisters.pc -= 2;
	result = apu->MOV1(apu->_getAbsoluteBit(), true);
	apu->_internalRegisters.pc -= 2;
	REQUIRE(apu->_internalRegisters.c == false);
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
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 56;
	result = apu->PUSH(apu->_internalRegisters.a);
	apu->_internalRegisters.sp++;
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(apu->_internalRegisters.sp | 0x100u) == 56);
}

TEST_CASE("POP Stack", "[Stack]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 82);
	apu->_internalRegisters.sp--;
	result = apu->POP(apu->_internalRegisters.y);
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.y == 82);
}

//////////////////////
//					//
// Subroutine tests //
//					//
//////////////////////

TEST_CASE("CALL Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_getAbsoluteAddr(), 23);
	apu->_internalRegisters.pc -= 2;
	result = apu->CALL(apu->_getAbsoluteAddr());
	REQUIRE(result == 8);
	REQUIRE(apu->_internalRegisters.pc == 23);
	REQUIRE(apu->_internalRead(++apu->_internalRegisters.sp + 0x0100u) == 2);
	REQUIRE(apu->_internalRead(++apu->_internalRegisters.sp + 0x0100u) == 0);
}

TEST_CASE("PCALL Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 123);
	result = apu->PCALL();
	REQUIRE(result == 6);
	REQUIRE(apu->_internalRegisters.pc == 65403);
}

TEST_CASE("TCALL Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(0xFFD0, 45);
	result = apu->TCALL(7);
	REQUIRE(result == 8);
	REQUIRE(apu->_internalRegisters.pc == 45);
}

TEST_CASE("BRK Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pch = 0xFF;
	apu->_internalRegisters.pcl = 0xEE;
	apu->_internalRegisters.psw = 0xDD;
	apu->_internalWrite(0xFFDF, 0xAA);
	apu->_internalWrite(0xFFDE, 0xBB);
	result = apu->BRK();
	apu->_internalRegisters.sp += 3;
	REQUIRE(result == 8);
	REQUIRE(apu->_internalRegisters.i == false);
	REQUIRE(apu->_internalRegisters.b == true);
	REQUIRE(apu->_internalRegisters.pc == 0xAABB);
	REQUIRE(apu->_internalRead(apu->_internalRegisters.sp-- | 0x100u) == 0xFF);
	REQUIRE(apu->_internalRead(apu->_internalRegisters.sp-- | 0x100u) == 0xEE);
	REQUIRE(apu->_internalRead(apu->_internalRegisters.sp | 0x100u) == 0xDD);
}

TEST_CASE("RET Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x12);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x34);
	apu->_internalRegisters.sp -= 2;
	result = apu->RET();
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRegisters.pch == 0x12);
	REQUIRE(apu->_internalRegisters.pcl == 0x34);
}

TEST_CASE("RETI Subroutine", "[Subroutine]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x12);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x34);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x56);
	apu->_internalRegisters.sp -= 3;
	result = apu->RETI();
	REQUIRE(result == 6);
	REQUIRE(apu->_internalRegisters.psw == 0x12);
	REQUIRE(apu->_internalRegisters.pch == 0x34);
	REQUIRE(apu->_internalRegisters.pcl == 0x56);
}

////////////////////////
//					  //
// Program Flow tests //
//					  //
////////////////////////

TEST_CASE("BRA ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	result = apu->BRA(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 24);
}

TEST_CASE("BEQ ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BEQ(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.z = true;
	result = apu->BEQ(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BNE ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.z = true;
	result = apu->BNE(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.z = false;
	result = apu->BNE(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BCS ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BCS(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.c = true;
	result = apu->BCS(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BCC ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.c = true;
	result = apu->BCC(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.c = false;
	result = apu->BCC(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BVS ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BVS(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.v = true;
	result = apu->BVS(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BVC ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.v = true;
	result = apu->BVC(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.v = false;
	result = apu->BVC(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BMI ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BMI(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.n = true;
	result = apu->BMI(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BPL ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.n = true;
	result = apu->BPL(apu->_getImmediateData());
	REQUIRE(result == 2);
	apu->_internalRegisters.n = false;
	result = apu->BPL(apu->_getImmediateData());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.pc == 25);
}

TEST_CASE("BBS ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalWrite(23, 0);
	result = apu->BBS(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	REQUIRE(result == 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(23, 100);
	result = apu->BBS(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	REQUIRE(result == 7);
	REQUIRE(apu->_internalRegisters.pc == 12);
}

TEST_CASE("BBC ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 10);
	apu->_internalWrite(10, 255);
	result = apu->BBC(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	REQUIRE(result == 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 0);
	result = apu->BBC(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	REQUIRE(result == 7);
	REQUIRE(apu->_internalRegisters.pc == 12);
}

TEST_CASE("CBNE ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.a = 4;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 10);
	apu->_internalWrite(10, 4);
	result = apu->CBNE(apu->_getDirectAddr(), apu->_getImmediateData());
	REQUIRE(result == 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.a = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	result = apu->CBNE(apu->_getDirectAddrByX(), apu->_getImmediateData(), true);
	REQUIRE(result == 8);
	REQUIRE(apu->_internalRegisters.pc == 12);
}

TEST_CASE("DBNZ ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.y = 1;
	result = apu->DBNZ(apu->_getImmediateData());
	REQUIRE(result == 4);
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 5);
	apu->_internalWrite(5, 55);
	result = apu->DBNZ(apu->_getImmediateData(), true);
	REQUIRE(result == 7);
	REQUIRE(	apu->_internalRead(5) == 54);
	REQUIRE(apu->_internalRegisters.pc == 13);
}

TEST_CASE("JMP ProgramFlow", "[ProgramFlow]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	result = apu->JMP(apu->_getAbsoluteAddr());
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.pc == 61455);
	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 0b000000001;
	apu->_internalWrite(0b1111000000001111 + 1, 0b00010000);
	apu->_internalWrite(0b1111000000001111 + 2, 0b11110001);
	result = apu->JMP(apu->_getAbsoluteByXAddr(), true);
	REQUIRE(result == 6);
	REQUIRE(apu->_internalRegisters.pc == 61712);
}

////////////////////////////////
//							  //
// Decimal Compensation tests //
//							  //
////////////////////////////////

TEST_CASE("DAA DecimalCompensation", "[DecimalCompensation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = true;
	apu->_internalRegisters.h = true;
	apu->_internalRegisters.a = 0x1A;
	result = apu->DAA();
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 0x80);
}

TEST_CASE("DAS DecimalCompensation", "[DecimalCompensation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	apu->_internalRegisters.h = false;
	apu->_internalRegisters.a = 0xFF;
	result = apu->DAS();
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 0x99);
}

///////////////////////////////////
//								 //
// Multiplication Division tests //
//								 //
///////////////////////////////////

TEST_CASE("MUL MultiplicationDivision", "[MultiplicationDivision]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 10;
	apu->_internalRegisters.y = 23;
	result = apu->MUL();
	REQUIRE(result == 9);
	REQUIRE(apu->_internalRegisters.ya == 230);
}

TEST_CASE("DIV MultiplicationDivision", "[MultiplicationDivision]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 235;
	apu->_internalRegisters.x = 10;
	result = apu->DIV();
	REQUIRE(result == 12);
	REQUIRE(apu->_internalRegisters.y == 5);
	REQUIRE(apu->_internalRegisters.a == 23);
	apu->_internalRegisters.ya = 12345;
	apu->_internalRegisters.x = 2;
	result = apu->DIV();
	REQUIRE(apu->_internalRegisters.y == 147);
	REQUIRE(apu->_internalRegisters.a == 211);
}

//////////////////////////////////
//								//
// (XVI)16-bit Arithmetic tests //
//								//
//////////////////////////////////

TEST_CASE("INCW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xFF);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->INCW(apu->_getDirectAddr());
	REQUIRE(result == 6);
	REQUIRE(apu->_internalRead(0x55) == 0x00);
	REQUIRE(apu->_internalRead(0x55 + 1) == 0x23);
}

TEST_CASE("DECW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x00);
	apu->_internalWrite(0x55 + 1, 0x23);
	result = apu->DECW(apu->_getDirectAddr());
	REQUIRE(result == 6);
	REQUIRE(apu->_internalRead(0x55) == 0xFF);
	REQUIRE(apu->_internalRead(0x55 + 1) == 0x22);
}

TEST_CASE("ADDW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->ADDW(apu->_getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRegisters.ya == 0x6532);
	REQUIRE(apu->_internalRegisters.v == false);
	REQUIRE(apu->_internalRegisters.h == false);
	REQUIRE(apu->_internalRegisters.c == false);
}

TEST_CASE("SUBW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->SUBW(apu->_getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRegisters.ya == 0x2110);
	REQUIRE(apu->_internalRegisters.v == false);
	REQUIRE(apu->_internalRegisters.h == true);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("CMPW XVIbitArithmetic", "[XVIbitArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x2211;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->CMPW(apu->_getDirectAddr());
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.c == true);
}

/////////////////////////////////////////
//									   //
// (XVI)16-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

TEST_CASE("MOVW XVIbitDataTransmission", "[XVIbitDataTransmission]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.ya = 0x2211;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	result = apu->MOVW(apu->_getDirectAddr());
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(0x55) == 0x11);
	REQUIRE(apu->_internalRead(0x56) == 0x22);
	apu->_internalRegisters.ya = 0x0000;
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(0x55, 0x33);
	apu->_internalWrite(0x55 + 1, 0x44);
	apu->MOVW(apu->_getDirectAddr(), true);
	REQUIRE(apu->_internalRegisters.ya == 0x4433);
}

//////////////////////////////////////
//									//
// (VIII)8-bit Shift Rotation tests //
//									//
//////////////////////////////////////

TEST_CASE("ASL VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ASL(apu->_internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0xCC);
	REQUIRE(apu->_internalRegisters.c == false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ASL(apu->_getDirectAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(0x55) == 0xBA);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("LSR VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->LSR(apu->_internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0x33);
	REQUIRE(apu->_internalRegisters.c == false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->LSR(apu->_getDirectAddr(), 5);
	REQUIRE(result  == 5);
	REQUIRE(apu->_internalRead(0x55) == 0x6E);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("ROL VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROL(apu->_internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0xCC);
	REQUIRE(apu->_internalRegisters.c == false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROL(apu->_getDirectAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(0x55) == 0xBA);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("ROR VIIIbitShiftRotation", "[VIIIbitShiftRotation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROR(apu->_internalRegisters.a, 2, true);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0x33);
	REQUIRE(apu->_internalRegisters.c == false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROR(apu->_getDirectAddr(), 5);
	REQUIRE(result  == 5);
	REQUIRE(apu->_internalRead(0x55) == 0x6E);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("XCN VIIIShiftRotation", "[VIIIShiftRotation]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0b10101010;
	result = apu->XCN();
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRegisters.a == 0xAA);
}

///////////////////////////////////////////
//										 //
// (VIII)8-bit Increment Decrement tests //
//										 //
///////////////////////////////////////////

TEST_CASE("INC VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->INC(apu->_getDirectAddr(), 4);
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(0x55) == 0xDE);
}

TEST_CASE("INCreg VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->INCreg(apu->_internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0x77);
}

TEST_CASE("DEC VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->DEC(apu->_getDirectAddr(), 4);
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(0x55) == 0xDC);
}

TEST_CASE("DECreg VIIIbitIncrementDecrement", "[VIIIbitIncrementDecrement]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->DECreg(apu->_internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 0x75);
}

///////////////////////////////
//							 //
// (VIII)8-bit Logical tests //
//							 //
///////////////////////////////

TEST_CASE("ANDacc VIIILogical", "[VIIILogical]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->ANDacc(apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 16);
}

TEST_CASE("AND VIIILogical", "[VIIILogical]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->AND(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(4) == 12);
}

TEST_CASE("ORacc VIIILogical", "[VIIILogical]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->ORacc(apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 31);
}

TEST_CASE("OR VIIILogical", "[VIIILogical]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->OR(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(4) == 44);
}

TEST_CASE("EORacc VIIILogical", "[VIIILogical]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->EORacc(apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 15);
}

TEST_CASE("EOR VIIILogical", "[VIIILogical]")
{
	Init()
	auto &apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->EOR(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(4) == 32);
}

//////////////////////////////////
//								//
// (VIII)8-bit Arithmetic tests //
//								//
//////////////////////////////////

TEST_CASE("ADC VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 53);
	apu->_internalWrite(7, 76);
	result = apu->ADC(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(4) == 130);
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(apu->_internalRegisters.h == true);
	REQUIRE(apu->_internalRegisters.v == true);
}

TEST_CASE("ADCacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 53;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 76);
	result = apu->ADCacc(apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 130);
	REQUIRE(apu->_internalRegisters.c == false);
	REQUIRE(apu->_internalRegisters.h == true);
	REQUIRE(apu->_internalRegisters.v == true);
}

TEST_CASE("SBC VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 67);
	apu->_internalWrite(7, 45);
	result = apu->SBC(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(4) == 22);
	REQUIRE(apu->_internalRegisters.c == true);
	REQUIRE(apu->_internalRegisters.h == false);
	REQUIRE(apu->_internalRegisters.v == false);
}

TEST_CASE("SBCacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 67;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 45);
	result = apu->SBCacc(apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.a == 22);
	REQUIRE(apu->_internalRegisters.c == true);
	REQUIRE(apu->_internalRegisters.h == false);
	REQUIRE(apu->_internalRegisters.v == false);
}

TEST_CASE("CMP VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 67);
	apu->_internalWrite(7, 45);
	result = apu->CMP(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRegisters.c == true);
}

TEST_CASE("CMPacc VIIIArithmetic", "[VIIIArithmetic]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 67;
	apu->_internalWrite(4, 45);
	result = apu->CMPreg(apu->_internalRegisters.a, apu->_getIndexXAddr(), 3);
	REQUIRE(result == 3);
	REQUIRE(apu->_internalRegisters.c == true);
}

/////////////////////////////////////////
//									   //
// (VIII)8-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

TEST_CASE("MovRegToReg VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 23;
	apu->_internalRegisters.x = 45;
	result = apu->MOV(apu->_internalRegisters.x, apu->_internalRegisters.a);
	REQUIRE(result == 2);
	REQUIRE(apu->_internalRegisters.a == 45);
}

TEST_CASE("MovMemToMem VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x23;
	apu->_internalWrite(0x23, 0x56);
	apu->_internalWrite(0x24, 0x33);
	apu->_internalWrite(0x56, 99);
	apu->_internalWrite(0x33, 66);
	result = apu->MOV(apu->_getDirectAddr(), apu->_getImmediateData());
	REQUIRE(result == 5);
	REQUIRE(apu->_internalRead(0x33) == 0x56);
}

TEST_CASE("MovRegToMem VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 0x23;
	apu->_internalRegisters.a = 0x44;
	apu->_internalWrite(0x23, 0x56);
	result = apu->MOV(apu->_internalRegisters.a, apu->_getIndexXAddr(), 4, true);
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRead(0x23) == 0x44);
	REQUIRE(apu->_internalRegisters.x == 0x24);
}

TEST_CASE("MovMemToReg VIIIDataTransmission", "[VIIIDataTransmission]")
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 0x23;
	apu->_internalRegisters.a = 0x44;
	result = apu->MOV(apu->_getIndexXAddr(), apu->_internalRegisters.a, 4, true);
	REQUIRE(result == 4);
	REQUIRE(apu->_internalRegisters.x == 0x24);
	REQUIRE(apu->_internalRegisters.a == 0x23);
}