//
// Created by Melefo on 12/02/2020.
//

#include <criterion/criterion.h>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/APU/APU.hpp"
#include "../../sources/Exceptions/InvalidAddress.hpp"
#include "../../sources/Exceptions/InvalidOpcode.hpp"

using namespace ComSquare;

//////////////////////////////
//							//
// APU::_internalRead tests	//
//							//
//////////////////////////////

Test(_internalRead, register)
{
	Init()
	auto apu = snes.apu;int8_t result = 0;

	apu->_registers.counter0 = 123;
	result = apu->_internalRead(0x00FD);
	cr_assert_eq(result, 123);
}

Test(_internalRead, Page0)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_map->Page0._data[0x0010] = 123;
	result = apu->_internalRead(0x0010);
	cr_assert_eq(result, 123);
}

Test(_internalRead, Page1)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_map->Page1._data[0x0042] = 123;
	result = apu->_internalRead(0x0142);
	cr_assert_eq(result, 123);
}

Test(_internalRead, Memory)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_map->Memory._data[0xFCDC] = 123;
	result = apu->_internalRead(0xFEDC);
	cr_assert_eq(result, 123);
}

Test(_internalRead, IPL)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_map->IPL._data[0x001F] = 123;
	result = apu->_internalRead(0xFFDF);
	cr_assert_eq(result, 123);
}

Test(_internalRead, Invalid)
{
	Init()
	auto apu = snes.apu;

	cr_assert_throw(apu->_internalRead(0x10000), InvalidAddress);
}

///////////////////////////////
//							 //
// APU::_internalWrite tests //
//							 //
///////////////////////////////

Test(_internalWrite, Page0)
{
	Init()
	auto apu = snes.apu;

	apu->_internalWrite(0x0001, 123);
	cr_assert_eq(apu->_map->Page0._data[0x0001], 123);
}

Test(_internalWrite, register)
{
	Init()
	auto apu = snes.apu;

	apu->_internalWrite(0x00F4, 123);
	cr_assert_eq(apu->_registers.port0, 123);
}

Test(_internalWrite, Page1)
{
	Init()
	auto apu = snes.apu;

	apu->_internalWrite(0x01FF, 123);
	cr_assert_eq(apu->_map->Page1._data[0x00FF], 123);
}

Test(_internalWrite, Memory)
{
	Init()
	auto apu = snes.apu;

	apu->_internalWrite(0x0789, 123);
	cr_assert_eq(apu->_map->Memory._data[0x0589], 123);
}

Test(_internalWrite, IPL)
{
	Init()
	auto apu = snes.apu;

	apu->_internalWrite(0xFFF0, 123);
	cr_assert_eq(apu->_map->IPL._data[0x0030], 123);
}

Test(_internalWrite, Invalid)
{
	Init()
	auto apu = snes.apu;

	cr_assert_throw(apu->_internalWrite(0x10000, 123), InvalidAddress);
}

/////////////////////
//				   //
// APU::read tests //
//				   //
/////////////////////

Test(read, Valid)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_registers.port2 = 123;
	result = apu->read(0x02);
	cr_assert_eq(result, 123);
}

Test(read, Invalid)
{
	Init()
	auto apu = snes.apu;

	cr_assert_throw(apu->read(0x10000), InvalidAddress);
}

//////////////////////
//					//
// APU::write tests	//
//					//
//////////////////////

Test(write, Valid)
{
	Init()
	auto apu = snes.apu;

	apu->write(0x03, 123);
	cr_assert_eq(apu->_registers.port3, 123);
}

Test(write, Invalid)
{
	Init()
	auto apu = snes.apu;

	cr_assert_throw(apu->write(0x04, 123), InvalidAddress);
}

///////////////////////////////////
//								 //
// APU::executeInstruction tests //
//								 //
///////////////////////////////////

Test(executeInstruction, Valid)
{
	Init()
	auto apu = snes.apu;
	uint8_t result = 0;

	apu->_internalRegisters.pc = 0x00;
	result = apu->_executeInstruction();
	cr_assert_eq(result, 2);
}

///////////////////////
//					 //
// APU::update tests //
//					 //
///////////////////////

Test(update, running)
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x00;
	apu->update(1);
	cr_assert_eq(apu->_paddingCycles, 1);
}

Test(update, stopped)
{
	Init()
	auto apu = snes.apu;

	apu->_state = APU::Stopped;
	apu->update(1);
	cr_assert_eq(apu->_paddingCycles, 0);
}

//////////////////////////
//						//
// APU::_get*Addr tests	//
//						//
//////////////////////////

Test(_get, direct)
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 123);
	cr_assert_eq(apu->_getDirectAddr(), 123);
}

Test(_get, absolute)
{
	Init()
	auto apu = snes.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	cr_assert_eq(apu->_getAbsoluteAddr(), 61455);
}