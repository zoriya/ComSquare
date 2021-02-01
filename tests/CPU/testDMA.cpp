//
// Created by anonymus-raccoon on 2/1/21.
//

#include <criterion/criterion.h>
#include <bitset>
#include "../tests.hpp"
using namespace ComSquare;

//Test(DMA, RomToVRAM)
//{
//	Init()
//	snes.cartridge->_size = 4000000;
//	snes.cartridge->_data = new uint8_t[snes.cartridge->_size];
//	for(unsigned i = 0xBE00, j = 0; i < 0xBE00 + 0x800; i++, j++)
//		snes.cartridge->_data[i] = j;
//
//	// Transferring $800 bytes from ROM ($13BE00) to VRam ($2000) via DMA channel 0
//
//	// Setting VRam address (since this is an indirect write)
//	snes.bus->write(0x2117, 0x20);
//	snes.bus->write(0x2116, 0);
//
//	snes.bus->write(0x4301, 0x18);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._port, 0x18,  "The dma's b port was $%x but it should have been $18.", snes.cpu->_dmaChannels[0]._port);
//	snes.bus->write(0x4304, 0x13);
//	snes.bus->write(0x4303, 0xBE);
//	snes.bus->write(0x4302, 0x00);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._aAddress.raw, 0x13BE00,  "The dma's a address was $%x but it should have been $13BE00.", snes.cpu->_dmaChannels[0]._aAddress.raw);
//	snes.bus->write(0x4306, 0x08);
//	snes.bus->write(0x4305, 0);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._count.raw, 0x0800,  "The dma's count was $%x but it should have been $0800.", snes.cpu->_dmaChannels[0]._count.raw);
//	snes.bus->write(0x4300, 1);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._controlRegister.direction, CPU::DMA::AtoB, "Direction should have been 0 (A to B) but it was %x.", snes.cpu->_dmaChannels[0]._controlRegister.direction);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._controlRegister._, 0, "The unused byte should be 0.");
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._controlRegister.increment, 0, "The increment byte should be set to 0.");
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._controlRegister.fixed, 0, "The increment byte should be set to 0.");
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._controlRegister.mode, CPU::DMA::TwoToTwo, "The DMA mode should have been TwoToTwo (%%001) but it was) $%x", snes.cpu->_dmaChannels[0]._controlRegister.mode);
//	cr_assert_eq(snes.cpu->_dmaChannels[0].enabled, false, "The DMA channel should be disabled.");
//	// Enabling DMA's channel 0
//	snes.bus->write(0x420B, 1);
//	cr_assert_eq(snes.cpu->_dmaChannels[0].enabled, true, "The DMA channel should be enabled.");
//	// TODO There is an overhead of 12-24 cycles for the whole transfer. How should I know how many cycles there is?
//	auto cycles = snes.cpu->_dmaChannels[0].run(1000000);
//	cr_assert_eq(cycles, 8 + 8 * 0x800, "The dma should take $4008 cycles but it took $%x.", cycles);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._count.raw, 0, "The dma count should be 0 but it was $%x.", snes.cpu->_dmaChannels[0]._count.raw);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._aAddress.raw, 0x13C600, "The dma count should be $13C600 but it was $%x.", snes.cpu->_dmaChannels[0]._aAddress.raw);
//	cr_assert_eq(snes.cpu->_dmaChannels[0]._port, 0x18, "The dma count should be $18 but it was $%x.", snes.cpu->_dmaChannels[0]._port);
//	cr_assert_eq(snes.ppu->_registers._vmadd.vmadd, 0x2400, "The vram address should be $2400 but it was %x.", snes.ppu->_registers._vmadd.vmadd);
//	for(unsigned i = 0x2000, j = 0; i < 0x2000 + 0x800; i++, j++)
//		std::cout << std::hex << i << ": " << (unsigned)snes.ppu->vram->_data[i] << std::endl;
////		cr_assert_eq(snes.ppu->vram->_data[i], j, "The memory at %x should be %x but it was %x", i, (uint16_t)j, snes.ppu->vram->_data[i]);
//}

Test(DMA, VramWrite)
{
	Init()
	//snes.bus->write(0x2115, 0b10000000);
	snes.bus->write(0x2117, 0x20);
	snes.bus->write(0x2116, 0x0);
	for (unsigned i = 0; i < 0x400; i++) {
		snes.bus->write(0x2119, i >> 8);
		snes.bus->write(0x2118, i);
		cr_assert_eq(snes.ppu->_registers._vmadd.vmadd, 0x2001 + i, "The vram address was %x but it should have been %x", snes.ppu->_registers._vmadd.vmadd, 0x2001 + i);
	}
	for(unsigned i = 0; i < 0x400; i++) {
		uint16_t value = snes.ppu->vram->_data[0x2000 * 2 + i * 2] | (snes.ppu->vram->_data[0x2000 * 2 + i * 2 + 1] << 8);
		//std::cout << std::hex << 0x2000 + i << ": " << value << std::endl;
		cr_assert_eq(value, (uint16_t)i, "The memory at %x should be %x but it was %x", 0x2000 + i, (uint16_t)i, value);
	}
}