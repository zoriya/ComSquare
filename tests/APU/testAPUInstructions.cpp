//
// Created by Melefo on 11/02/2020.
//

#include <criterion/criterion.h>
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

Test(Standbys, NOP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->NOP();
	cr_assert_eq(result, 2);
}

Test(Standbys, SLEEP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SLEEP();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_state, APU::Sleeping);
}

Test(Standbys, STOP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->STOP();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_state, APU::Stopped);
}

///////////////
//			 //
// PSW tests //
//			 //
///////////////

Test(PSW, CLRC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRC();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.c, false);
}

Test(PSW, SETC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SETC();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(PSW, NOTC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	result = apu->NOTC();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(PSW, CLRV)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRV();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, false);
}

Test(PSW, CLRP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->CLRP();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.p, false);
}

Test(PSW, SETP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->SETP();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.p, true);
}

Test(PSW, EI)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->EI();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.i, true);
}

Test(PSW, DI)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	result = apu->DI();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.i, false);
}

///////////////
//			 //
// Bit tests //
//			 //
///////////////

Test(Bit, SET1)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(apu->_getDirectAddr(), 0b00000000);
	apu->_internalRegisters.pc--;
	result = apu->SET1(apu->_getDirectAddr(), 0);
	apu->_internalRegisters.pc--;
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_getDirectAddr()), 1);
}

Test(Bit, CLR1)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(apu->_getDirectAddr(), 0b11111111);
	apu->_internalRegisters.pc--;
	result = apu->CLR1(apu->_getDirectAddr(), 0);
	apu->_internalRegisters.pc--;
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_getDirectAddr()), 0b11111110);
}

Test(Bit, TSET1)
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
	cr_assert_eq(apu->_internalRead(apu->_getAbsoluteAddr()), 0x7B);
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(result, 6);
}

Test(Bit, TCLR1)
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
	cr_assert_eq(apu->_internalRead(apu->_getAbsoluteAddr()), 0x00);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(result, 6);
}

Test(Bit, AND1)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 4);
}

Test(Bit, AND1_invert)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 4);
}

Test(Bit, OR1)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 5);
}

Test(Bit, OR1_invert)
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
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(result, 5);
}

Test(Bit, EOR1)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 5);
}

Test(Bit, NOT1)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 5);
}

Test(Bit, MOV1)
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
	cr_assert_eq(apu->_internalRead(apu->_getAbsoluteAddr() & 0x1FFFu), 123);
	cr_assert_eq(result, 6);
}

Test(Bit, MOV1_carry)
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
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(result, 4);
}

/////////////////
//			   //
// Stack tests //
//			   //
/////////////////

Test(Stack, PUSH)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 56;
	result = apu->PUSH(apu->_internalRegisters.a);
	apu->_internalRegisters.sp++;
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.sp | 0x100u), 56);
}

Test(Stack, POP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 82);
	apu->_internalRegisters.sp--;
	result = apu->POP(apu->_internalRegisters.y);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.y, 82);
}

//////////////////////
//					//
// Subroutine tests //
//					//
//////////////////////

Test(Subroutine, CALL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_getAbsoluteAddr(), 23);
	apu->_internalRegisters.pc -= 2;
	result = apu->CALL(apu->_getAbsoluteAddr());
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.pc, 23);
	cr_assert_eq(apu->_internalRead(++apu->_internalRegisters.sp + 0x0100u), 2);
	cr_assert_eq(apu->_internalRead(++apu->_internalRegisters.sp + 0x0100u), 0);
}

Test(Subroutine, PCALL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 123);
	result = apu->PCALL();
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRegisters.pc, 65403);
}

Test(Subroutine, TCALL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(0xFFD0, 45);
	result = apu->TCALL(7);
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.pc, 45);
}

Test(Subroutine, BRK)
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
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.i, false);
	cr_assert_eq(apu->_internalRegisters.b, true);
	cr_assert_eq(apu->_internalRegisters.pc, 0xAABB);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.sp-- | 0x100u), 0xFF);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.sp-- | 0x100u), 0xEE);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.sp | 0x100u), 0xDD);
}

Test(Subroutine, RET)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x12);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x34);
	apu->_internalRegisters.sp -= 2;
	result = apu->RET();
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.pch, 0x12);
	cr_assert_eq(apu->_internalRegisters.pcl, 0x34);
}

Test(Subroutine, RETI)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x12);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x34);
	apu->_internalWrite(++apu->_internalRegisters.sp | 0x100u, 0x56);
	apu->_internalRegisters.sp -= 3;
	result = apu->RETI();
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRegisters.psw, 0x12);
	cr_assert_eq(apu->_internalRegisters.pch, 0x34);
	cr_assert_eq(apu->_internalRegisters.pcl, 0x56);
}

/////////////////////////////
//						   //
// Subroutine Program Flow //
//						   //
/////////////////////////////

Test(ProgramFlow, BRA)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BRA();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BEQ)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BEQ();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.z = true;
	result = apu->BEQ();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BNE)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	apu->_internalRegisters.z = true;
	result = apu->BNE();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.z = false;
	result = apu->BNE();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BCS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BCS();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.c = true;
	result = apu->BCS();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BCC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	apu->_internalRegisters.c = true;
	result = apu->BCC();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.c = false;
	result = apu->BCC();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BVS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BVS();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.v = true;
	result = apu->BVS();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BVC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	apu->_internalRegisters.v = true;
	result = apu->BVC();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.v = false;
	result = apu->BVC();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BMI)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BMI();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.n = true;
	result = apu->BMI();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BPL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	apu->_internalRegisters.n = true;
	result = apu->BPL();
	cr_assert_eq(result, 2);
	apu->_internalRegisters.n = false;
	result = apu->BPL();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}