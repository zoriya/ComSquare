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

	apu->_internalRegisters.pc = 0;
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

////////////////////////
//					  //
// Program Flow tests //
//					  //
////////////////////////

Test(ProgramFlow, BRA)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	result = apu->BRA(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 24);
}

Test(ProgramFlow, BEQ)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BEQ(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.z = true;
	result = apu->BEQ(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BNE)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.z = true;
	result = apu->BNE(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.z = false;
	result = apu->BNE(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BCS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BCS(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.c = true;
	result = apu->BCS(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BCC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.c = true;
	result = apu->BCC(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.c = false;
	result = apu->BCC(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BVS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BVS(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.v = true;
	result = apu->BVS(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BVC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.v = true;
	result = apu->BVC(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.v = false;
	result = apu->BVC(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BMI)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BMI(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.n = true;
	result = apu->BMI(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BPL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalRegisters.n = true;
	result = apu->BPL(apu->_getImmediateData());
	cr_assert_eq(result, 2);
	apu->_internalRegisters.n = false;
	result = apu->BPL(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, BBS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalWrite(23, 0);
	result = apu->BBS(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(23, 100);
	result = apu->BBS(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	cr_assert_eq(result, 7);
	cr_assert_eq(apu->_internalRegisters.pc, 12);
}

Test(ProgramFlow, BBC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 10);
	apu->_internalWrite(10, 255);
	result = apu->BBC(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 0);
	result = apu->BBC(apu->_getDirectAddr(), apu->_getImmediateData(), 2);
	cr_assert_eq(result, 7);
	cr_assert_eq(apu->_internalRegisters.pc, 12);
}

Test(ProgramFlow, CBNE)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.a = 4;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 10);
	apu->_internalWrite(10, 4);
	result = apu->CBNE(apu->_getDirectAddr(), apu->_getImmediateData());
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.a = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	result = apu->CBNE(apu->_getDirectAddrByX(), apu->_getImmediateData(), true);
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.pc, 12);
}

Test(ProgramFlow, DBNZ)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.y = 1;
	result = apu->DBNZ(apu->_getImmediateData());
	cr_assert_eq(result, 4);
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 5);
	apu->_internalWrite(5, 55);
	result = apu->DBNZ(apu->_getImmediateData(), true);
	cr_assert_eq(result, 7);
	cr_assert_eq(	apu->_internalRead(5), 54);
	cr_assert_eq(apu->_internalRegisters.pc, 13);
}

Test(ProgramFlow, JMP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	result = apu->JMP(apu->_getAbsoluteAddr());
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.pc, 61455);
	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 0b000000001;
	apu->_internalWrite(0b1111000000001111 + 1, 0b00010000);
	apu->_internalWrite(0b1111000000001111 + 2, 0b11110001);
	result = apu->JMP(apu->_getAbsoluteByXAddr(), true);
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRegisters.pc, 61712);
}

////////////////////////////////
//							  //
// Decimal Compensation tests //
//							  //
////////////////////////////////

Test(DecimalCompensation, DAA)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = true;
	apu->_internalRegisters.h = true;
	apu->_internalRegisters.a = 0x1A;
	result = apu->DAA();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 0x80);
}

Test(DecimalCompensation, DAS)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	apu->_internalRegisters.h = false;
	apu->_internalRegisters.a = 0xFF;
	result = apu->DAS();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 0x99);
}

///////////////////////////////////
//								 //
// Multiplication Division tests //
//								 //
///////////////////////////////////

Test(MultiplicationDivision, MUL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 10;
	apu->_internalRegisters.y = 23;
	result = apu->MUL();
	cr_assert_eq(result, 9);
	cr_assert_eq(apu->_internalRegisters.ya, 230);
}

Test(MultiplicationDivision, DIV)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 235;
	apu->_internalRegisters.x = 10;
	result = apu->DIV();
	cr_assert_eq(result, 12);
	cr_assert_eq(apu->_internalRegisters.y, 5);
	cr_assert_eq(apu->_internalRegisters.a, 23);
	apu->_internalRegisters.ya = 12345;
	apu->_internalRegisters.x = 2;
	result = apu->DIV();
	cr_assert_eq(apu->_internalRegisters.y, 147);
	cr_assert_eq(apu->_internalRegisters.a, 211);
}

//////////////////////////////////
//								//
// (XVI)16-bit Arithmetic tests //
//								//
//////////////////////////////////

Test(XVIbitArithmetic, INCW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xFF);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->INCW(apu->_getDirectAddr());
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRead(0x55), 0x00);
	cr_assert_eq(apu->_internalRead(0x55 + 1), 0x23);
}

Test(XVIbitArithmetic, DECW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x00);
	apu->_internalWrite(0x55 + 1, 0x23);
	result = apu->DECW(apu->_getDirectAddr());
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRead(0x55), 0xFF);
	cr_assert_eq(apu->_internalRead(0x55 + 1), 0x22);
}

Test(XVIbitArithmetic, ADDW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->ADDW(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.ya, 0x6532);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, false);
	cr_assert_eq(apu->_internalRegisters.c, false);
}

Test(XVIbitArithmetic, SUBW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->SUBW(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.ya, 0x2110);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, true);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(XVIbitArithmetic, CMPW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x2211;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->CMPW(apu->_getDirectAddr());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

/////////////////////////////////////////
//									   //
// (XVI)16-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

Test(XVIbitDataTransmission, MOVW)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.ya = 0x2211;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	result = apu->MOVW(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x55), 0x11);
	cr_assert_eq(apu->_internalRead(0x56), 0x22);
	apu->_internalRegisters.ya = 0x0000;
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(0x55, 0x33);
	apu->_internalWrite(0x55 + 1, 0x44);
	apu->MOVW(apu->_getDirectAddr(), true);
	cr_assert_eq(apu->_internalRegisters.ya, 0x4433);
}

//////////////////////////////////////
//									//
// (VIII)8-bit Shift Rotation tests //
//									//
//////////////////////////////////////

Test(VIIIbitShiftRotation, ASL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ASL(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0xCC);
	cr_assert_eq(apu->_internalRegisters.c, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ASL(apu->_getDirectAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x55), 0xBA);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(VIIIbitShiftRotation, LSR)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->LSR(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x33);
	cr_assert_eq(apu->_internalRegisters.c, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->LSR(apu->_getDirectAddr(), 5);
	cr_assert_eq(result , 5);
	cr_assert_eq(apu->_internalRead(0x55), 0x6E);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(VIIIbitShiftRotation, ROL)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROL(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0xCC);
	cr_assert_eq(apu->_internalRegisters.c, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROL(apu->_getDirectAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x55), 0xBA);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(VIIIbitShiftRotation, ROR)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROR(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x33);
	cr_assert_eq(apu->_internalRegisters.c, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROR(apu->_getDirectAddr(), 5);
	cr_assert_eq(result , 5);
	cr_assert_eq(apu->_internalRead(0x55), 0x6E);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(VIIIShiftRotation, XCN)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0b10101010;
	result = apu->XCN();
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.a, 0xAA);
}

///////////////////////////////////////////
//										 //
// (VIII)8-bit Increment Decrement tests //
//										 //
///////////////////////////////////////////

Test(VIIIbitIncrementDecrement, INC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->INC(apu->_getDirectAddr(), 4);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(0x55), 0xDE);
}

Test(VIIIbitIncrementDecrement, INCreg)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->INCreg(apu->_internalRegisters.a);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x77);
}

Test(VIIIbitIncrementDecrement, DEC)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->DEC(apu->_getDirectAddr(), 4);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(0x55), 0xDC);
}

Test(VIIIbitIncrementDecrement, DECreg)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->DECreg(apu->_internalRegisters.a);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x75);
}

///////////////////////////////
//							 //
// (VIII)8-bit Logical tests //
//							 //
///////////////////////////////

Test(VIIILogical, ANDacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->ANDacc(apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 16);
}

Test(VIIILogical, AND)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->AND(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(4), 12);
}

Test(VIIILogical, ORacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->ORacc(apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 31);
}

Test(VIIILogical, OR)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->OR(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(4), 44);
}

Test(VIIILogical, EORacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 24;
	apu->_internalWrite(4, 23);
	result = apu->EORacc(apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 15);
}

Test(VIIILogical, EOR)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 12);
	apu->_internalWrite(7, 44);
	result = apu->EOR(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(4), 32);
}

//////////////////////////////////
//								//
// (VIII)8-bit Arithmetic tests //
//								//
//////////////////////////////////

Test(VIIIArithmetic, ADC)
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
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(4), 130);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.h, true);
	cr_assert_eq(apu->_internalRegisters.v, true);
}

Test(VIIIArithmetic, ADCacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 53;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 76);
	result = apu->ADCacc(apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 130);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.h, true);
	cr_assert_eq(apu->_internalRegisters.v, true);
}

Test(VIIIArithmetic, SBC)
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
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(4), 22);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.h, false);
	cr_assert_eq(apu->_internalRegisters.v, false);
}

Test(VIIIArithmetic, SBCacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 67;
	apu->_internalRegisters.c = true;
	apu->_internalWrite(4, 45);
	result = apu->SBCacc(apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 22);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.h, false);
	cr_assert_eq(apu->_internalRegisters.v, false);
}

Test(VIIIArithmetic, CMP)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.y = 7;
	apu->_internalWrite(4, 67);
	apu->_internalWrite(7, 45);
	result = apu->CMP(apu->_getIndexXAddr(), apu->_getIndexYAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(VIIIArithmetic, CMPacc)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 4;
	apu->_internalRegisters.a = 67;
	apu->_internalWrite(4, 45);
	result = apu->CMPreg(apu->_internalRegisters.a, apu->_getIndexXAddr(), 3);
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

/////////////////////////////////////////
//									   //
// (VIII)8-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

Test(VIIIDataTransmission, MovRegToReg)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.a = 23;
	apu->_internalRegisters.x = 45;
	result = apu->MOV(apu->_internalRegisters.x, apu->_internalRegisters.a);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 45);
}

Test(VIIIDataTransmission, MovMemToMem)
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
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x33), 0x56);
}

Test(VIIIDataTransmission, MovRegToMem)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 0x23;
	apu->_internalRegisters.a = 0x44;
	apu->_internalWrite(0x23, 0x56);
	result = apu->MOV(apu->_internalRegisters.a, apu->_getIndexXAddr(), 4, true);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(0x23), 0x44);
	cr_assert_eq(apu->_internalRegisters.x, 0x24);
}

Test(VIIIDataTransmission, MovMemToReg)
{
	Init()
	auto apu = snes.apu;
	int result = 0;

	apu->_internalRegisters.x = 0x23;
	apu->_internalRegisters.a = 0x44;
	result = apu->MOV(apu->_getIndexXAddr(), apu->_internalRegisters.a, 4, true);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.x, 0x24);
	cr_assert_eq(apu->_internalRegisters.a, 0x23);
}