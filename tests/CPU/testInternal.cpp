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
	Init()
	snes.cpu->SEP(0xFF);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0xFF, "The flag should be 0xFF but it was %x", data);
}

Test(SEP, setsome)
{
	Init()
	snes.cpu->_registers.p.flags = 0b01000000;
	snes.cpu->SEP(0b10110101);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b11110101, "The flag should be 245 but it was %i", data);
}

Test(REP, resetall)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->REP(0xFF);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x00, "The flag should be 0x00 but it was %x", data);
}

Test(REP, resetsome)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.flags = 0b01000000;
	snes.cpu->REP(0b01000000);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x0, "The flag should be 0 but it was %x", data);
}

Test(REP, resetallEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->REP(0xFF);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flag should be 0b00110000 but it was %x", data);
}

Test(REP, resetsomeEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0b01000101;
	snes.cpu->REP(0b01000001);
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110100, "The flag should be 0b00110100 but it was %x", data);
}

Test(JSR, jump)
{
	Init()
	snes.cpu->_registers.pc = 0xABCD;
	snes.cpu->_registers.s = 0x0123;
	snes.cpu->JSR(0xABFF);
	auto pc = snes.cpu->_registers.pc;
	cr_assert_eq(pc, 0xABFF, "The PC should be 0xABFF but it was %x", pc);
	cr_assert_eq(snes.cpu->_registers.s, 0x0121, "The stack pointer should be 0x0121 but it was %x", snes.cpu->_registers.s);
	auto pushed = snes.cpu->_pop16();
	cr_assert_eq(pushed, 0xABCC, "The value pushed to the stack should be 0xABCC but it was %x", pushed);
}

Test(JSL, jump)
{
	Init()
	snes.cpu->_registers.pbr = 0xFF;
	snes.cpu->_registers.pc = 0xABCD;
	snes.cpu->_registers.s = 0x0123;
	snes.cpu->JSL(0xCDABFF);
	auto pac = snes.cpu->_registers.pac;
	cr_assert_eq(pac, 0xCDABFF, "The PC should be 0xCDABFF but it was %x", pac);
	cr_assert_eq(snes.cpu->_registers.s, 0x0120, "The stack pointer should be 0x0120 but it was %x", snes.cpu->_registers.s);
	auto pushed = snes.cpu->_pop16() + (snes.cpu->_pop() << 16u);
	cr_assert_eq(pushed, 0xFFABCC, "The value pushed to the stack should be 0xFFABCD but it was %x", pushed);
}

Test(PHA, basic)
{
	Init()
	snes.cpu->_registers.a = 0xABCD;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHA();
	cr_assert_eq(snes.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", snes.cpu->_registers.s);
}

Test(PHB, basic)
{
	Init()
	snes.cpu->_registers.dbr = 0xFF;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHB();
	cr_assert_eq(snes.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PHD, basic)
{
	Init()
	snes.cpu->_registers.d = 0xABCD;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHD();
	cr_assert_eq(snes.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", snes.cpu->_registers.s);
}

Test(PHK, basic)
{
	Init()
	snes.cpu->_registers.pbr = 0xFF;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHK();
	cr_assert_eq(snes.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PHP, basic)
{
	Init()
	snes.cpu->_registers.p.flags = 0xFF;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHP();
	cr_assert_eq(snes.wram->_data[2], 0xFF, "The first value pushed to the stack should be 0xFF but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PHX, basic)
{
	Init()
	snes.cpu->_registers.x = 0xABCD;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHX();
	cr_assert_eq(snes.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", snes.cpu->_registers.s);
}

Test(PHY, basic)
{
	Init()
	snes.cpu->_registers.y = 0xABCD;
	snes.cpu->_registers.s = 0x02;
	snes.cpu->PHY();
	cr_assert_eq(snes.wram->_data[1], 0xCD, "The second value pushed to the stack should be 0xCD but it was %x", snes.wram->_data[1]);
	cr_assert_eq(snes.wram->_data[2], 0xAB, "The first value pushed to the stack should be 0xAB but it was %x", snes.wram->_data[2]);
	cr_assert_eq(snes.cpu->_registers.s, 0x0, "The Stack pointer should be equal to 0x0 but it was %x", snes.cpu->_registers.s);
}

Test(PLA, basic)
{
	Init()
	snes.wram->_data[1] = 0xCD;
	snes.wram->_data[2] = 0x7B;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLA();
	auto data = snes.cpu->_registers.a;
	cr_assert_eq(data, 0x7BCD, "The accumulator should be 0x7BCD but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLA, zero)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLA();
	auto data = snes.cpu->_registers.a;
	cr_assert_eq(data, 0x0000, "The accumulator should be 0x0000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLA, negative)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0xA0;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLA();
	auto data = snes.cpu->_registers.a;
	cr_assert_eq(data, 0xA000, "The accumulator should be 0xA000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag not should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLX, basic)
{
	Init()
	snes.wram->_data[1] = 0xCD;
	snes.wram->_data[2] = 0x7B;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLX();
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x7BCD, "The X register should be 0x7BCD but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLX, zero)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLX();
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0x0000, "The x register should be 0x0000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLX, negative)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0xA0;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLX();
	auto data = snes.cpu->_registers.x;
	cr_assert_eq(data, 0xA000, "The x register should be 0xA000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag not should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLY, basic)
{
	Init()
	snes.wram->_data[1] = 0xCD;
	snes.wram->_data[2] = 0x7B;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLY();
	auto data = snes.cpu->_registers.y;
	cr_assert_eq(data, 0x7BCD, "The Y register should be 0x7BCD but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLY, zero)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLY();
	auto data = snes.cpu->_registers.y;
	cr_assert_eq(data, 0x0000, "The y register should be 0x0000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLY, negative)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0xA0;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLY();
	auto data = snes.cpu->_registers.y;
	cr_assert_eq(data, 0xA000, "The y register should be 0xA000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag not should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLD, basic)
{
	Init()
	snes.wram->_data[1] = 0xCD;
	snes.wram->_data[2] = 0x7B;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLD();
	auto data = snes.cpu->_registers.d;
	cr_assert_eq(data, 0x7BCD, "The D register should be 0x7BCD but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLD, zero)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLD();
	auto data = snes.cpu->_registers.d;
	cr_assert_eq(data, 0x0000, "The d register should be 0x0000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLD, negative)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.wram->_data[2] = 0xA0;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLD();
	auto data = snes.cpu->_registers.d;
	cr_assert_eq(data, 0xA000, "The D register should be 0xA000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag not should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x2, "The Stack pointer should be equal to 0x2 but it was %x", snes.cpu->_registers.s);
}

Test(PLB, basic)
{
	Init()
	snes.wram->_data[1] = 0x7D;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLB();
	auto data = snes.cpu->_registers.dbr;
	cr_assert_eq(data, 0x7D, "The DBR should be 0x7D but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PLB, zero)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLB();
	auto data = snes.cpu->_registers.dbr;
	cr_assert_eq(data, 0x00, "The dbr should be 0x00 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PLB, negative)
{
	Init()
	snes.wram->_data[1] = 0xA0;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->PLB();
	auto data = snes.cpu->_registers.dbr;
	cr_assert_eq(data, 0xA0, "The D register should be 0xA0 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag not should be set.", snes.cpu->_registers.p.z);
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set.", snes.cpu->_registers.p.n);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PLP, basic)
{
	Init()
	snes.wram->_data[1] = 0x7D;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->_isEmulationMode = false;
	snes.cpu->PLP();
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0x7D, "The flags should be 0x7D but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(PLP, emulation)
{
	Init()
	snes.wram->_data[1] = 0x00;
	snes.cpu->_registers.s = 0x00;
	snes.cpu->_isEmulationMode = true;
	snes.cpu->PLP();
	auto data = snes.cpu->_registers.p.flags;
	cr_assert_eq(data, 0b00110000, "The flags should be 0b00110000 but it was %x", data);
	cr_assert_eq(snes.cpu->_registers.s, 0x1, "The Stack pointer should be equal to 0x1 but it was %x", snes.cpu->_registers.s);
}

Test(CLC, clear)
{
	Init()
	snes.cpu->_registers.p.flags = 0xFF;
	snes.cpu->CLC();
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CLI, clear)
{
	Init()
	snes.cpu->_registers.p.flags = 0xFF;
	snes.cpu->CLI();
	cr_assert_eq(snes.cpu->_registers.p.i, false, "The interrupt flag should not be set");
}

Test(CLD, clear)
{
	Init()
	snes.cpu->_registers.p.flags = 0xFF;
	snes.cpu->CLD();
	cr_assert_eq(snes.cpu->_registers.p.d, false, "The decimal flag should not be set");
}

Test(CLV, clear)
{
	Init()
	snes.cpu->_registers.p.flags = 0xFF;
	snes.cpu->CLV();
	cr_assert_eq(snes.cpu->_registers.p.v, false, "The overflow flag should not be set");
}

Test(SEC, set)
{
	Init()
	snes.cpu->_registers.p.flags = 0x00;
	snes.cpu->SEC();
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(SEI, set)
{
	Init()
	snes.cpu->_registers.p.flags = 0x00;
	snes.cpu->SEI();
	cr_assert_eq(snes.cpu->_registers.p.i, true, "The interrupt disabled flag should be set");
}

Test(SED, set)
{
	Init()
	snes.cpu->_registers.p.flags = 0x00;
	snes.cpu->SED();
	cr_assert_eq(snes.cpu->_registers.p.d, true, "The decimal flag should be set");
}

Test(XCE, enableEmulation)
{
	Init()
	snes.cpu->_isEmulationMode = false;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.xh = 0xFF;
	snes.cpu->_registers.yh = 0xFF;
	snes.cpu->XCE();
	cr_assert_eq(snes.cpu->_isEmulationMode, true, "The e flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.m, false, "The memory width flag should be untouched (unset)");
	cr_assert_eq(snes.cpu->_registers.p.x_b, false, "The index width flag should be untouched (unset)");
	cr_assert_eq(snes.cpu->_registers.xh, 0xFF, "The high byte of the x index flag should be untouched (0xFF)");
	cr_assert_eq(snes.cpu->_registers.yh, 0xFF, "The high byte of the y index flag should be untouched (0xFF)");
}

Test(XCE, enableNative)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.xh = 0xFF;
	snes.cpu->_registers.yh = 0xFF;
	snes.cpu->XCE();
	cr_assert_eq(snes.cpu->_isEmulationMode, false, "The e flag should be not set");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.m, true, "The memory width flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.x_b, true, "The index width flag should be set");
	cr_assert_eq(snes.cpu->_registers.xh, 0, "The high byte of the x index flag should be set to 0");
	cr_assert_eq(snes.cpu->_registers.yh, 0, "The high byte of the y index flag should be set to 0");
}

Test(INX, basic)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0xFF;
	snes.cpu->INX();
	cr_assert_eq(snes.cpu->_registers.x, 0x0100, "The x register should be equal to 0x0100 but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INX, 8bits)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.x = 0xFF;
	snes.cpu->INX();
	cr_assert_eq(snes.cpu->_registers.x, 0x00, "The x register should be equal to 0x00 but it was 0x%x.", snes.cpu->_registers.x);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INY, basic)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.y = 0xFF;
	snes.cpu->INY();
	cr_assert_eq(snes.cpu->_registers.y, 0x0100, "The y register should be equal to 0x0100 but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(INY, 8bits)
{
	Init()
	snes.cpu->_isEmulationMode = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.y = 0xFF;
	snes.cpu->INY();
	cr_assert_eq(snes.cpu->_registers.y, 0x00, "The y register should be equal to 0x00 but it was 0x%x.", snes.cpu->_registers.y);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
}

Test(CPX, basic)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.x = 0xFF;
	snes.wram->_data[0] = 0xFF;
	snes.cpu->CPX(0x0);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(CPX, negative)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.x = 0x80;
	snes.wram->_data[0] = 0xFF;
	snes.cpu->CPX(0x0);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CPX, 16bits)
{
	Init()
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.p.x_b = false;
	snes.cpu->_registers.x = 0x8888;
	snes.wram->_data[0] = 0x88;
	snes.wram->_data[1] = 0x98;
	snes.cpu->CPX(0x0);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(CPY, basic)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.y = 0xFF;
	snes.wram->_data[0] = 0xFF;
	snes.cpu->CPY(0x0);
	cr_assert_eq(snes.cpu->_registers.p.z, true, "The zero flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.n, false, "The negative flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.c, true, "The carry flag should be set");
}

Test(CPY, negative)
{
	Init()
	snes.cpu->_registers.p.x_b = true;
	snes.cpu->_registers.p.flags = 0;
	snes.cpu->_registers.y = 0x80;
	snes.wram->_data[0] = 0xFF;
	snes.cpu->CPY(0x0);
	cr_assert_eq(snes.cpu->_registers.p.z, false, "The zero flag should not be set");
	cr_assert_eq(snes.cpu->_registers.p.n, true, "The negative flag should be set");
	cr_assert_eq(snes.cpu->_registers.p.c, false, "The carry flag should not be set");
}

Test(BCC, basic)
{
	Init()
	snes.cpu->_registers.p.c = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BCC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BCC, negativeJump)
{
	Init()
	snes.cpu->_registers.p.c = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BCC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BCC, noJump)
{
	Init()
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BCC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BCS, basic)
{
	Init()
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BCS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BCS, negativeJump)
{
	Init()
	snes.cpu->_registers.p.c = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BCS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BCS, noJump)
{
	Init()
	snes.cpu->_registers.p.c = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BCS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BEQ, basic)
{
	Init()
	snes.cpu->_registers.p.z = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BEQ(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BEQ, negativeJump)
{
	Init()
	snes.cpu->_registers.p.z = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BEQ(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BEQ, noJump)
{
	Init()
	snes.cpu->_registers.p.z = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BEQ(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BNE, basic)
{
	Init()
	snes.cpu->_registers.p.z = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BNE(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BNE, negativeJump)
{
	Init()
	snes.cpu->_registers.p.z = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BNE(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BNE, noJump)
{
	Init()
	snes.cpu->_registers.p.z = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BNE(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BMI, basic)
{
	Init()
	snes.cpu->_registers.p.n = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BMI(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BMI, negativeJump)
{
	Init()
	snes.cpu->_registers.p.n = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BMI(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BMI, noJump)
{
	Init()
	snes.cpu->_registers.p.n = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BMI(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BPL, basic)
{
	Init()
	snes.cpu->_registers.p.n = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BPL(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BPL, negativeJump)
{
	Init()
	snes.cpu->_registers.p.n = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BPL(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BPL, noJump)
{
	Init()
	snes.cpu->_registers.p.n = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BPL(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BRA, basic)
{
	Init()
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BRA(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BRA, negativeJump)
{
	Init()
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BRA(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BRL, basic)
{
	Init()
	snes.cpu->_registers.pc = 0x8080;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0x10;
	snes.cpu->BRL(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x9080, "The program counter should be equal to 0x9080 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BRL, negativeJump)
{
	Init()
	snes.cpu->_registers.pc = 0x8080;
	snes.wram->_data[0] = 0x00;
	snes.wram->_data[1] = 0xF0;
	snes.cpu->BRL(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x7080, "The program counter should be equal to 0x7080 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BVC, basic)
{
	Init()
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BVC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BVC, negativeJump)
{
	Init()
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BVC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BVC, noJump)
{
	Init()
	snes.cpu->_registers.p.v = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BVC(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}


Test(BVS, basic)
{
	Init()
	snes.cpu->_registers.p.v = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x50;
	snes.cpu->BVS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0xD0, "The program counter should be equal to 0xD0 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BVS, negativeJump)
{
	Init()
	snes.cpu->_registers.p.v = true;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0xF0;
	snes.cpu->BVS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x70, "The program counter should be equal to 0x70 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(BVS, noJump)
{
	Init()
	snes.cpu->_registers.p.v = false;
	snes.cpu->_registers.pc = 0x80;
	snes.wram->_data[0] = 0x90;
	snes.cpu->BVS(0x0);
	cr_assert_eq(snes.cpu->_registers.pc, 0x80, "The program counter should be equal to 0x80 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(JMP, simpleJump)
{
	Init()
	snes.cpu->_registers.pc = 0x8000;
	snes.cpu->JMP(0x1000);
	cr_assert_eq(snes.cpu->_registers.pc, 0x1000, "The program counter should be equal to 0x9000 but it was 0x%x.", snes.cpu->_registers.pc);
}

Test(JML, simpleJump)
{
	Init()
	snes.cpu->_registers.pc = 0x8000;
	snes.cpu->JML(0x10AB00);
	cr_assert_eq(snes.cpu->_registers.pac, 0x10AB00, "The program counter should be equal to 0x10AB00 but it was 0x%x.", snes.cpu->_registers.pac);
}