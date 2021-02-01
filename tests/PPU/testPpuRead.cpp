//
// Created by cbihan on 2/1/21.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
#include "../../sources/PPU/PPU.hpp"

using namespace ComSquare;

Test(PPU_read_1, vram_data_read_full)
{
	Init()
	snes._bus->write(0x2115, 0b10000000);
	snes._bus->write(0x2116, 0);
	snes._bus->write(0x2117, 0);
	snes.ppu->vram->write_internal(0, 0b11111111);
	snes.ppu->vram->write_internal(1, 0b11111111);

	snes._bus->write(0x2116, 0);
	snes._bus->write(0x2117, 0);
	uint8_t tmp = snes._bus->read(0x2139);
	cr_expect(tmp == 0b11111111, "Got %X expected 0xFF", tmp);
	tmp = snes._bus->read(0x213a);
	cr_expect(tmp == 0b11111111, "Got %X expected 0xFF", tmp);
}

Test(PPU_read_1, vram_data_read_half)
{
	Init()
	snes._bus->write(0x2116, 0);
	snes._bus->write(0x2117, 0);
	snes.ppu->vram->write_internal(0, 0b01101001);
	snes.ppu->vram->write_internal(1, 0b11111111);

	snes._bus->write(0x2116, 0);
	snes._bus->write(0x2117, 0);
	uint8_t tmp = snes._bus->read(0x2139);
	cr_expect(tmp == 0b01101001, "Got %X expected 0x69", tmp);
	tmp = snes._bus->read(0x213a);
	cr_expect(tmp == 0b11111111, "Got %X expected 0xFF", tmp);
}