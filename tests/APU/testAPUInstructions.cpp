//
// Created by Melefo on 11/02/2020.
//

#include <criterion/criterion.h>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/APU/APU.hpp"

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