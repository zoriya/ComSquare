//
// Created by cbihan on 9/29/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"

using namespace ComSquare;

Test(backgroundGetTilePixelReference, basicTest)
{
	Init()
	snes.bus->write(0x2100, 0b11111111);
	cr_assert_eq(snes.ppu->_registers._inidisp.fblank, true);
	cr_assert_eq(snes.ppu->_registers._inidisp.brightness, 0xF);
}