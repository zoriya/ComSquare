//
// Created by anonymus-raccoon on 2/10/20.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
using namespace ComSquare;


Test(AddrModeInit, LegitBus)
{
	auto pair = Init();
	cr_assert_eq(pair.first.get(), pair.second.cpu->_bus.get(), "Warning, the CPU's bus is not the same the SNES's bus. Next tests of the CPU may fail.");
}

Test(AddrMode, Immediate)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x000015;
	cr_assert_eq(pair.second.cpu->_GetImmediateAddr(), 0x000015, "Got %i, Expected 0x000015");
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x000016);
}

Test(AddrMode, ImmediateBankChange)
{
	auto pair = Init();
	pair.second.cpu->_registers.pac = 0x00FFFF;
	cr_assert_eq(pair.second.cpu->_GetImmediateAddr(), 0x00FFFF);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x010000);
}

//Test(AddrMode, Direct)
//{
//	auto pair = Init();
//	pair.second.cartridge->_data[0] = 0x15;
//	pair.second.cpu->_registers.pac = 0x808000;
//	pair.second.cpu->_registers.d = 0x1000;
//	cr_assert_eq(pair.second.cpu->_GetDirectAddr(), 0x1015, "Returned address was %i but was expecting 0x1015.");
//	cr_assert_eq(pair.second.cpu->_registers.pac, 0x808001);
//}