//
// Created by anonymus-raccoon on 2/13/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/CPU/CPU.hpp"

using namespace ComSquare;

Test(SEP, setall)
{
	auto pair = Init();
	pair.second.cpu->SEP(0xFF);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0xFF, "The flag should be 0xFF but it was %x", data);
}

Test(SEP, setsome)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->SEP(0b10110101);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b11110101, "The flag should be 245 but it was %i", data);
}

Test(REP, resetall)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->REP(0xFF);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x00, "The flag should be 0x00 but it was %x", data);
}

Test(REP, resetsome)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.flags = 0b01000000;
	pair.second.cpu->REP(0b01000000);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x0, "The flag should be 0 but it was %x", data);
}

Test(REP, resetallEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->REP(0xFF);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flag should be 0b00110000 but it was %x", data);
}

Test(REP, resetsomeEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0b01000101;
	pair.second.cpu->REP(0b01000001);
	auto data = pair.second.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110100, "The flag should be 0b00110100 but it was %x", data);
}

Test(JSR, jump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0xABCD;
	pair.second.cpu->_registers.s = 0x0123;
	pair.second.cpu->JSR(0xABFF);
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
	pair.second.cpu->JSL(0xCDABFF);
	auto pac = pair.second.cpu->_registers.pac;
	cr_assert_eq(pac, 0xCDABFF, "The PC should be 0xCDABFF but it was %x", pac);
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

Test(SEC, set)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0x00;
	pair.second.cpu->SEC();
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(SEI, set)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0x00;
	pair.second.cpu->SEI();
	cr_assert_eq(pair.second.cpu->_registers.p.i, true, "The interrupt disabled flag should be set");
}

Test(SED, set)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0x00;
	pair.second.cpu->SED();
	cr_assert_eq(pair.second.cpu->_registers.p.d, true, "The decimal flag should be set");
}

Test(XCE, enableEmulation)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = false;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.c = true;
	pair.second.cpu->_registers.xh = 0xFF;
	pair.second.cpu->_registers.yh = 0xFF;
	pair.second.cpu->XCE();
	cr_assert_eq(pair.second.cpu->_isEmulationMode, true, "The e flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.m, false, "The memory width flag should be untouched (unset)");
	cr_assert_eq(pair.second.cpu->_registers.p.x_b, false, "The index width flag should be untouched (unset)");
	cr_assert_eq(pair.second.cpu->_registers.xh, 0xFF, "The high byte of the x index flag should be untouched (0xFF)");
	cr_assert_eq(pair.second.cpu->_registers.yh, 0xFF, "The high byte of the y index flag should be untouched (0xFF)");
}

Test(XCE, enableNative)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.xh = 0xFF;
	pair.second.cpu->_registers.yh = 0xFF;
	pair.second.cpu->XCE();
	cr_assert_eq(pair.second.cpu->_isEmulationMode, false, "The e flag should be not set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.m, true, "The memory width flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.x_b, true, "The index width flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.xh, 0, "The high byte of the x index flag should be set to 0");
	cr_assert_eq(pair.second.cpu->_registers.yh, 0, "The high byte of the y index flag should be set to 0");
}

Test(INX, basic)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.x = 0xFF;
	pair.second.cpu->INX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0x0100, "The x register should be equal to 0x0100 but it was 0x%x.", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INX, 8bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.x = 0xFF;
	pair.second.cpu->INX();
	cr_assert_eq(pair.second.cpu->_registers.x, 0x00, "The x register should be equal to 0x00 but it was 0x%x.", pair.second.cpu->_registers.x);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INY, basic)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.y = 0xFF;
	pair.second.cpu->INY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0x0100, "The y register should be equal to 0x0100 but it was 0x%x.", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INY, 8bits)
{
	auto pair = Init();
	pair.second.cpu->_isEmulationMode = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.y = 0xFF;
	pair.second.cpu->INY();
	cr_assert_eq(pair.second.cpu->_registers.y, 0x00, "The y register should be equal to 0x00 but it was 0x%x.", pair.second.cpu->_registers.y);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(CPX, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.x = 0xFF;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->CPX(0x0);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(CPX, negative)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.x = 0x80;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->CPX(0x0);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CPX, 16bits)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.p.x_b = false;
	pair.second.cpu->_registers.x = 0x8888;
	pair.second.wram->_data[0] = 0x88;
	pair.second.wram->_data[1] = 0x98;
	pair.second.cpu->CPX(0x0);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CPY, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.y = 0xFF;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->CPY(0x0);
	cr_assert_eq(pair.second.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, false, "The negative flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(CPY, negative)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.x_b = true;
	pair.second.cpu->_registers.p.flags = 0;
	pair.second.cpu->_registers.y = 0x80;
	pair.second.wram->_data[0] = 0xFF;
	pair.second.cpu->CPY(0x0);
	cr_assert_eq(pair.second.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(pair.second.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(pair.second.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(BCC, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BCC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BCC, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BCC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BCC, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BCC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BCS, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BCS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BCS, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BCS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BCS, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.c = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BCS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BEQ, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BEQ(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BEQ, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BEQ(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BEQ, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BEQ(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BNE, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BNE(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BNE, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BNE(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BNE, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.z = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BNE(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BMI, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BMI(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BMI, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BMI(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BMI, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BMI(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BPL, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BPL(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BPL, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BPL(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BPL, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.n = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BPL(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BRA, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BRA(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BRA, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BRA(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BRL, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x8080;
	pair.second.wram->_data[0] = 0x00;
	pair.second.wram->_data[1] = 0x10;
	pair.second.cpu->BRL(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x9080, "The program counter should be equal to 0x9080 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BRL, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x8080;
	pair.second.wram->_data[0] = 0x00;
	pair.second.wram->_data[1] = 0xF0;
	pair.second.cpu->BRL(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x7080, "The program counter should be equal to 0x7080 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BVC, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BVC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BVC, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BVC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BVC, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BVC(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}


Test(BVS, basic)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x50;
	pair.second.cpu->BVS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BVS, negativeJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = true;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0xF0;
	pair.second.cpu->BVS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(BVS, noJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.p.v = false;
	pair.second.cpu->_registers.pc = 0x80;
	pair.second.wram->_data[0] = 0x90;
	pair.second.cpu->BVS(0x0);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(JMP, simpleJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x8000;
	pair.second.cpu->JMP(0x1000);
	cr_assert_eq(pair.second.cpu->_registers.pc, 0x1000, "The program counter should be equal to 0x9000 but it was 0x%x.", pair.second.cpu->_registers.pc);
}

Test(JML, simpleJump)
{
	auto pair = Init();
	pair.second.cpu->_registers.pc = 0x8000;
	pair.second.cpu->JML(0x10AB00);
	cr_assert_eq(pair.second.cpu->_registers.pac, 0x10AB00, "The program counter should be equal to 0x10AB00 but it was 0x%x.", pair.second.cpu->_registers.pac);
}