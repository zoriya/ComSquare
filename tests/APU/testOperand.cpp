//
// Created by Melefo on 26/02/2020.
//

#include <criterion/criterion.h>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/APU/APU.hpp"
#include "../../sources/Exceptions/InvalidAddress.hpp"
#include "../../sources/Exceptions/InvalidOpcode.hpp"

using namespace ComSquare;

Test(_get, immediate)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0x40);
	cr_assert_eq(apu->_getImmediateData(), 0x40);
}

Test(_get, direct)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalWrite(0x32, 0x40);
	cr_assert_eq(apu->_getDirectAddr(), 0x140);
}

Test(_get, X)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.x = 0x32;
	apu->_internalRegisters.p = true;
	cr_assert_eq(apu->_getIndexXAddr(), 0x132);
}

Test(_get, Y)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.y = 0x32;
	apu->_internalRegisters.p = true;
	cr_assert_eq(apu->_getIndexYAddr(), 0x132);
}

Test(_get, directbyX)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 0x03;
	apu->_internalWrite(0x32, 0x40);
	cr_assert_eq(apu->_getDirectAddrByX(), 0x43);
}

Test(_get, absolute)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	cr_assert_eq(apu->_getAbsoluteAddr(), 61455);
}

Test(_get, absolutebyx)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	cr_assert_eq(apu->_getAbsoluteByXAddr(), 64025);
}

Test(_get, absoluteaddrbyx)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.x = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	cr_assert_eq(apu->_getAbsoluteAddrByX(), 61465);
}

Test(_get, absoluteaddrbyy)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.y = 10;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	cr_assert_eq(apu->_getAbsoluteAddrByY(), 61465);
}

Test(_get, absolutebit)
{
	auto apu = Init().second.apu;
	std::pair<uint24_t, uint24_t> result;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalWrite(0x32, 0b00001111);
	apu->_internalWrite(0x33, 0b11110000);
	result = apu->_getAbsoluteBit();
	cr_assert_eq(result.first, 4111);
	cr_assert_eq(result.second, 7);
}

Test(_get, absolutebyxdirect)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalRegisters.x = 0x10;
	apu->_internalWrite(0x32, 0x42);
	apu->_internalWrite(0x152, 0b00001101);
	apu->_internalWrite(0x253, 0b01101011);
	cr_assert_eq(apu->_getAbsoluteDirectByXAddr(), 0b0110101100001101);
}

Test(_get, absolutedirectbyy)
{
	auto apu = Init().second.apu;

	apu->_internalRegisters.pc = 0x32;
	apu->_internalRegisters.p = true;
	apu->_internalRegisters.y = 0x10;
	apu->_internalWrite(0x32, 0x42);
	apu->_internalWrite(0x142, 0b00001101);
	apu->_internalWrite(0x243, 0b01101011);
	cr_assert_eq(apu->_getAbsoluteDirectAddrByY(), 0b0110101100011101);
}