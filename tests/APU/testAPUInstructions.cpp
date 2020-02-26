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

////////////////////
//				  //
// Standbys tests //
//				  //
////////////////////

Test(Standbys, NOP)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->NOP();
	cr_assert_eq(result, 2);
}

Test(Standbys, SLEEP)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->SLEEP();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_state, APU::Sleeping);
}

Test(Standbys, STOP)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->CLRC();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.c, false);
}

Test(PSW, SETC)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->SETC();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(PSW, NOTC)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	result = apu->NOTC();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(PSW, CLRV)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->CLRV();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, false);
}

Test(PSW, CLRP)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->CLRP();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.p, false);
}

Test(PSW, SETP)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->SETP();
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.p, true);
}

Test(PSW, EI)
{
	auto apu = Init().second.apu;
	int result = 0;

	result = apu->EI();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.i, true);
}

Test(PSW, DI)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 56;
	result = apu->PUSH(apu->_internalRegisters.a);
	apu->_internalRegisters.sp++;
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.sp | 0x100u), 56);
}

Test(Stack, POP)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 123);
	result = apu->PCALL();
	cr_assert_eq(result, 6);
	cr_assert_eq(apu->_internalRegisters.pc, 65403);
}

Test(Subroutine, TCALL)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalWrite(0xFFD0, 45);
	result = apu->TCALL(7);
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.pc, 45);
}

Test(Subroutine, BRK)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(apu->_internalRegisters.pc + 24, 101);
	result = apu->BRA();
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(apu->_internalRegisters.pc), 101);
}

Test(ProgramFlow, BEQ)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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

Test(ProgramFlow, BBS)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 23);
	apu->_internalWrite(23, 0);
	result = apu->BBS(apu->_getDirectAddr(), 2);
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 10);
	apu->_internalWrite(23, 255);
	result = apu->BBS(apu->_getDirectAddr(), 2);
	cr_assert_eq(result, 7);
	cr_assert_eq(apu->_internalRegisters.pc, 12);
}

Test(ProgramFlow, BBC)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite( 10, 255);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->BBC(apu->_getDirectAddr(), 2);
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite( 10, 0);
	result = apu->BBC(apu->_getDirectAddr(), 2);
	cr_assert_eq(result, 7);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, CBNE)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.a = 4;
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite( 10, 4);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	result = apu->CBNE(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	apu->_internalRegisters.pc = 0;
	apu->_internalWrite( 10, 0);
	result = apu->CBNE(apu->_getDirectAddrByX(), true);
	cr_assert_eq(result, 8);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, DBNZ)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0;
	apu->_internalRegisters.y = 1;
	result = apu->DBNZ();
	cr_assert_eq(result, 4);
	apu->_internalWrite(apu->_internalRegisters.pc, 10);
	apu->_internalWrite(apu->_internalRegisters.pc + 1, 23);
	apu->_internalWrite( 10, 0);
	result = apu->DBNZ(true);
	cr_assert_eq(result, 7);
	cr_assert_eq(apu->_internalRegisters.pc, 25);
}

Test(ProgramFlow, JMP)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	result = apu->JMP(apu->_getAbsoluteAddr());
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.pc, 61455);
	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 0b000000001;
	result = apu->JMP(apu->_getAbsoluteAddrByX(), true);
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
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.c = true;
	apu->_internalRegisters.h = true;
	apu->_internalRegisters.a = 0x1A;
	result = apu->DAA();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 0x80);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

Test(DecimalCompensation, DAS)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.c = false;
	apu->_internalRegisters.h = false;
	apu->_internalRegisters.a = 0xFF;
	result = apu->DAS();
	cr_assert_eq(result, 3);
	cr_assert_eq(apu->_internalRegisters.a, 0x99);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

///////////////////////////////////
//								 //
// Multiplication Division tests //
//								 //
///////////////////////////////////

Test(MultiplicationDivision, MUL)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 10;
	apu->_internalRegisters.y = 23;
	result = apu->MUL();
	cr_assert_eq(result, 9);
	cr_assert_eq(apu->_internalRegisters.ya, 230);
}

Test(MultiplicationDivision, DIV)
{
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
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
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->ADDW(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.ya, 0x6532);
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, false);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.c, false);
}

Test(XVIbitArithmetic, SUBW)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x4321;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->SUBW(apu->_getDirectAddr());
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.ya, 0x2110);
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.v, false);
	cr_assert_eq(apu->_internalRegisters.h, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.c, true);
}

Test(XVIbitArithmetic, CMPW)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.ya = 0x2211;
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0x11);
	apu->_internalWrite(0x55 + 1, 0x22);
	result = apu->CMPW(apu->_getDirectAddr());
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRegisters.z, true);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.n, false);
}

/////////////////////////////////////////
//									   //
// (XVI)16-bit Data Transmission tests //
//									   //
/////////////////////////////////////////

Test(XVIbitDataTransmission, MOVW)
{
	auto apu = Init().second.apu;
	int result = 0;

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
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

//////////////////////////////////////
//									//
// (VIII)8-bit Shift Rotation tests //
//									//
//////////////////////////////////////

Test(VIIIbitShiftRotation, ASL)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ASL(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0xCC);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ASL(apu->_getDirectAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x55), 0xBA);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

Test(VIIIbitShiftRotation, LSR)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->LSR(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x33);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->LSR(apu->_getDirectAddr(), 5);
	cr_assert_eq(result , 5);
	cr_assert_eq(apu->_internalRead(0x55), 0x6E);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

Test(VIIIbitShiftRotation, ROL)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROL(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0xCC);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROL(apu->_getDirectAddr(), 5);
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRead(0x55), 0xBA);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

Test(VIIIbitShiftRotation, ROR)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x66;
	result = apu->ROR(apu->_internalRegisters.a, 2, true);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x33);
	cr_assert_eq(apu->_internalRegisters.c, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->ROR(apu->_getDirectAddr(), 5);
	cr_assert_eq(result , 5);
	cr_assert_eq(apu->_internalRead(0x55), 0x6E);
	cr_assert_eq(apu->_internalRegisters.c, true);
	cr_assert_eq(apu->_internalRegisters.n, false);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

Test(VIIIShiftRotation, XCN)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0b10101010;
	result = apu->XCN();
	cr_assert_eq(result, 5);
	cr_assert_eq(apu->_internalRegisters.a, 0xAA);
	cr_assert_eq(apu->_internalRegisters.n, true);
	cr_assert_eq(apu->_internalRegisters.z, false);
}

///////////////////////////////////////////
//										 //
// (VIII)8-bit Increment Decrement tests //
//										 //
///////////////////////////////////////////

Test(VIIIbitIncrementDecrement, INC)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->INC(apu->_getDirectAddr(), 4);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(0x55), 0xDE);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
}

Test(VIIIbitIncrementDecrement, INCreg)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->INCreg(apu->_internalRegisters.a);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x77);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.n, false);
}

Test(VIIIbitIncrementDecrement, DEC)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalWrite(apu->_internalRegisters.pc, 0x55);
	apu->_internalWrite(0x55, 0xDD);
	result = apu->DEC(apu->_getDirectAddr(), 4);
	cr_assert_eq(result, 4);
	cr_assert_eq(apu->_internalRead(0x55), 0xDC);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.n, true);
}

Test(VIIIbitIncrementDecrement, DECreg)
{
	auto apu = Init().second.apu;
	int result = 0;

	apu->_internalRegisters.a = 0x76;
	result = apu->DECreg(apu->_internalRegisters.a);
	cr_assert_eq(result, 2);
	cr_assert_eq(apu->_internalRegisters.a, 0x75);
	cr_assert_eq(apu->_internalRegisters.z, false);
	cr_assert_eq(apu->_internalRegisters.n, false);
}