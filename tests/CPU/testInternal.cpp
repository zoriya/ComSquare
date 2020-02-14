//
// Created by anonymus-raccoon on 2/13/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;

Test(SEP, setall)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->SEP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0xFF, "The flag should be 0xFF but it was %x", data);
}

Test(SEP, setsome)
{
	auto pair = Init();
	pair.second.wram->_data[0] = 0b10110101;
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->SEP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b11110101, "The flag should be 245 but it was %i", data);
}

Test(REP, resetall)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x00, "The flag should be 0x00 but it was %x", data);
}

Test(REP, resetsome)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.wram->_data[0] = 0b01000000;
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x0, "The flag should be 0 but it was %x", data);
}

Test(REP, resetallEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flag should be 0b00110000 but it was %x", data);
}

Test(REP, resetsomeEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.wram->_data[0] = 0b01000001;
	pair.second.cpu->_registers.p.flags = 0b01000101;
	pair.second.cpu->REP(0x0);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110100, "The flag should be 0b00110100 but it was %x", data);
}

Test(JSR, jump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0xABCD;
	pair.second.cpu->_registers.s = 0x0123;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.wram->_data[1] = 0xAB;
	pair.second.cpu->JSR(0x0);
	auto pc = pair.second.cpu->_registers.pc;
	cr_assert_eq(pc, 0xABFF, "The PC should be 0xABFF but it was %x", pc);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0121, "The stack pointer should be 0x0121 but it was %x", pair.second.cpu->_registers.s);
	auto pushed = pair.second.cpu->_pop16();
	cr_assert_eq(pushed, 0xABCC, "The value pushed to the stack should be 0xABCC but it was %x", pushed);
}

Test(JSL, jump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pbr = 0xFF;
	pair.second.cpu->_registers.pc = 0xABCD;
	pair.second.cpu->_registers.s = 0x0123;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.wram->_data[1] = 0xAB;
	pair.second.cpu->JSL(0x0);
	auto pc = pair.second.cpu->_registers.pc;
	cr_assert_eq(pc, 0xABFF, "The PC should be 0xABFF but it was %x", pc);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0120, "The stack pointer should be 0x0120 but it was %x", pair.second.cpu->_registers.s);
	auto pushed = pair.second.cpu->_pop16() + (pair.second.cpu->_pop() << 16u);
	cr_assert_eq(pushed, 0xFFABCC, "The value pushed to the stack should be 0xFFABCD but it was %x", pushed);
}

Test(PHA, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.a = 0xABCD;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHA();
	cr_assert_eq(pair.second.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", pair.second.wram->_data[1]);
	cr_assert_eq(pair.second.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHB, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.dbr = 0xFF;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHB();
	cr_assert_eq(pair.second.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHD, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.d = 0xABCD;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHD();
	cr_assert_eq(pair.second.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", pair.second.wram->_data[1]);
	cr_assert_eq(pair.second.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHK, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.pbr = 0xFF;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHK();
	cr_assert_eq(pair.second.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHP, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0xFF;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHP();
	cr_assert_eq(pair.second.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHX, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.x = 0xABCD;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHX();
	cr_assert_eq(pair.second.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", pair.second.wram->_data[1]);
	cr_assert_eq(pair.second.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", pair.second.cpu->_registers.s);
}

Test(PHY, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.y = 0xABCD;
	pair.second.cpu->_registers.s = 0x02;
	pair.second.cpu->PHY();
	cr_assert_eq(pair.second.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", pair.second.wram->_data[1]);
	cr_assert_eq(pair.second.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", pair.second.wram->_data[2]);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLA, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0xCD;
	pair.second.wram->_data[2] = 0x7B;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLA();
	auto data = pair.second.cpu->_registers.a;
	cr_assert_eq(data, 0x7BCD, "The accumulator should be 0x7BCD but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLA, zero)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLA();
	auto data = pair.second.cpu->_registers.a;
	cr_assert_eq(data, 0x0000, "The accumulator should be 0x0000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLA, negative)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0xA0;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLA();
	auto data = pair.second.cpu->_registers.a;
	cr_assert_eq(data, 0xA000, "The accumulator should be 0xA000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag not should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLX, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0xCD;
	pair.second.wram->_data[2] = 0x7B;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLX();
	auto data = pair.second.cpu->_registers.x;
	cr_assert_eq(data, 0x7BCD, "The X register should be 0x7BCD but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLX, zero)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLX();
	auto data = pair.second.cpu->_registers.x;
	cr_assert_eq(data, 0x0000, "The x register should be 0x0000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLX, negative)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0xA0;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLX();
	auto data = pair.second.cpu->_registers.x;
	cr_assert_eq(data, 0xA000, "The x register should be 0xA000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag not should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLY, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0xCD;
	pair.second.wram->_data[2] = 0x7B;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLY();
	auto data = pair.second.cpu->_registers.y;
	cr_assert_eq(data, 0x7BCD, "The Y register should be 0x7BCD but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLY, zero)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLY();
	auto data = pair.second.cpu->_registers.y;
	cr_assert_eq(data, 0x0000, "The y register should be 0x0000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLY, negative)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0xA0;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLY();
	auto data = pair.second.cpu->_registers.y;
	cr_assert_eq(data, 0xA000, "The y register should be 0xA000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag not should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLD, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0xCD;
	pair.second.wram->_data[2] = 0x7B;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLD();
	auto data = pair.second.cpu->_registers.d;
	cr_assert_eq(data, 0x7BCD, "The D register should be 0x7BCD but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLD, zero)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLD();
	auto data = pair.second.cpu->_registers.d;
	cr_assert_eq(data, 0x0000, "The d register should be 0x0000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLD, negative)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.wram->_data[2] = 0xA0;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLD();
	auto data = pair.second.cpu->_registers.d;
	cr_assert_eq(data, 0xA000, "The D register should be 0xA000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag not should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLB, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x7D;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLB();
	auto data = pair.second.cpu->_registers.dbr;
	cr_assert_eq(data, 0x7D, "The DBR should be 0x7D but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLB, zero)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLB();
	auto data = pair.second.cpu->_registers.dbr;
	cr_assert_eq(data, 0x00, "The dbr should be 0x00 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLB, negative)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0xA0;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->PLB();
	auto data = pair.second.cpu->_registers.dbr;
	cr_assert_eq(data, 0xA0, "The D register should be 0xA0 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag not should be set.", pair.second.cpu->_registers.p.z);
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set.", pair.second.cpu->_registers.p.n);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLP, basic)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x7D;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->PLP();
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x7D, "The flags should be 0x7D but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(PLP, emulation)
{
	auto pair = Init();
	pair.second.wram->_data[1] = 0x00;
	pair.second.cpu->_registers.s = 0x00;
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->PLP();
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flags should be 0b00110000 but it was %x", data);
	cr_assert_eq(pair.second.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", pair.second.cpu->_registers.s);
}

Test(CLC, clear)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0xFF;
	pair.second.cpu->CLC();
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CLI, clear)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0xFF;
	pair.second.cpu->CLI();
	cr_assert_eq(pair.second.cpu->_registers.p.i, false, "The interrupt flag should not be set");
}

Test(CLD, clear)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0xFF;
	pair.second.cpu->CLD();
	cr_assert_eq(pair.second.cpu->_registers.p.d, false, "The decimal flag should not be set");
}

Test(CLV, clear)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0xFF;
	pair.second.cpu->CLV();
	cr_assert_eq(pair.second.cpu->_registers.p.v, false, "The overflow flag should not be set");
}