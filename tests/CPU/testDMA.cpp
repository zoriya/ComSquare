//
// Created by anonymus-raccoon on 2/1/21.
//

#include <catch2/catch.hpp>
#include <bitset>
#include "../tests.hpp"
using namespace ComSquare;

TEST_CASE("RomToVRAM DMA", "[DMA]")
{
	Init()
	snes.cartridge._data.resize(4000000);
	for (unsigned i = 0; i < 0x400; i++) {
		snes.cartridge._data[0x9be00 + i * 2] = i;
		snes.cartridge._data[0x9be00 + i * 2 + 1] = i >> 8;
	}

	// Transferring $800 bytes from ROM ($13BE00) to VRam ($2000) via DMA channel 0

	// Setting VRam address (since this is an indirect write)
	snes.bus.write(0x2115, 0b10000000);
	snes.bus.write(0x2117, 0x20);
	snes.bus.write(0x2116, 0);

	snes.bus.write(0x4301, 0x18);
	REQUIRE(snes.cpu._dmaChannels[0]._port == 0x18);
	snes.bus.write(0x4304, 0x13);
	snes.bus.write(0x4303, 0xBE);
	snes.bus.write(0x4302, 0x00);
	REQUIRE(snes.cpu._dmaChannels[0]._aAddress.raw == 0x13BE00);
	snes.bus.write(0x4306, 0x08);
	snes.bus.write(0x4305, 0);
	REQUIRE(snes.cpu._dmaChannels[0]._count.raw == 0x0800);
	snes.bus.write(0x4300, 1);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.direction == CPU::DMA::AtoB);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister._ == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.increment == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.fixed == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.mode == CPU::DMA::TwoToTwo);
	REQUIRE(snes.cpu._dmaChannels[0].enabled == false);
	// Enabling DMA's channel 0
	snes.bus.write(0x420B, 1);
	REQUIRE(snes.cpu._dmaChannels[0].enabled == true);
	// TODO There is an overhead of 12-24 cycles for the whole transfer. How should I know how many cycles there is?
	auto cycles = snes.cpu._dmaChannels[0].run(1000000);
	REQUIRE(cycles == 8 + 8 * 0x800);
	REQUIRE(snes.cpu._dmaChannels[0]._count.raw == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._aAddress.raw == 0x13C600);
	REQUIRE(snes.cpu._dmaChannels[0]._port == 0x18);
	REQUIRE(snes.ppu._registers._vmadd.vmadd == 0x2400);
	for(unsigned i = 0; i < 0x400; i++) {
		uint16_t value = snes.ppu.vram._data[0x2000 * 2 + i * 2] | (snes.ppu.vram._data[0x2000 * 2 + i * 2 + 1] << 8);
		REQUIRE(value == i);
	}
	REQUIRE(snes.cpu._dmaChannels[0].enabled == false);
}

TEST_CASE("VramWrite DMA", "[DMA]")
{
	Init()
	snes.bus.write(0x2117, 0x20);
	snes.bus.write(0x2116, 0x0);
	for (unsigned i = 0; i < 0x400; i++) {
		snes.bus.write(0x2119, i >> 8);
		snes.bus.write(0x2118, i);
		REQUIRE(snes.ppu._registers._vmadd.vmadd == 0x2001 + i);
	}
	for(unsigned i = 0; i < 0x400; i++) {
		uint16_t value = snes.ppu.vram._data[0x2000 * 2 + i * 2] | (snes.ppu.vram._data[0x2000 * 2 + i * 2 + 1] << 8);
		REQUIRE(value == (uint16_t)i);
	}
}

TEST_CASE("VramWriteInvertedOrder DMA", "[DMA]")
{
	Init()
	snes.bus.write(0x2115, 0b10000000);
	snes.bus.write(0x2117, 0x20);
	snes.bus.write(0x2116, 0x0);
	for (unsigned i = 0; i < 0x400; i++) {
		snes.bus.write(0x2118, i);
		snes.bus.write(0x2119, i >> 8);
		REQUIRE(snes.ppu._registers._vmadd.vmadd == 0x2001 + i);
	}
	for(unsigned i = 0; i < 0x400; i++) {
		uint16_t value = snes.ppu.vram._data[0x2000 * 2 + i * 2] | (snes.ppu.vram._data[0x2000 * 2 + i * 2 + 1] << 8);
		REQUIRE(value == (uint16_t)i);
	}
}

TEST_CASE("WRamToVRAM DMA", "[DMA]")
{
	Init()
	for (unsigned i = 0; i < 0x400; i++) {
		snes.wram._data[i * 2] = i;
		snes.wram._data[i * 2 + 1] = i >> 8;
	}

	// Transferring $800 bytes from WRAM ($00) to VRam ($2000) via DMA channel 0

	// Setting VRam address (since this is an indirect write)
	snes.bus.write(0x2115, 0b10000000);
	snes.bus.write(0x2117, 0);
	snes.bus.write(0x2116, 0);

	snes.bus.write(0x4301, 0x18);
	REQUIRE(snes.cpu._dmaChannels[0]._port == 0x18);
	snes.bus.write(0x4304, 0x7E);
	snes.bus.write(0x4303, 0x00);
	snes.bus.write(0x4302, 0x00);
	REQUIRE(snes.cpu._dmaChannels[0]._aAddress.raw == 0x7E0000);
	snes.bus.write(0x4306, 0x08);
	snes.bus.write(0x4305, 0);
	REQUIRE(snes.cpu._dmaChannels[0]._count.raw == 0x0800);
	snes.bus.write(0x4300, 1);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.direction == CPU::DMA::AtoB);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister._ == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.increment == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.fixed == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._controlRegister.mode == CPU::DMA::TwoToTwo);
	REQUIRE(snes.cpu._dmaChannels[0].enabled == false);
	// Enabling DMA's channel 0
	snes.bus.write(0x420B, 1);
	REQUIRE(snes.cpu._dmaChannels[0].enabled == true);
	// TODO There is an overhead of 12-24 cycles for the whole transfer. How should I know how many cycles there is?
	auto cycles = snes.cpu._dmaChannels[0].run(1000000);
	REQUIRE(cycles == 8 + 8 * 0x800);
	REQUIRE(snes.cpu._dmaChannels[0]._count.raw == 0);
	REQUIRE(snes.cpu._dmaChannels[0]._aAddress.raw == 0x7E0800);
	REQUIRE(snes.cpu._dmaChannels[0]._port == 0x18);
	REQUIRE(snes.ppu._registers._vmadd.vmadd == 0x0400);
	for(unsigned i = 0; i < 0x400; i++) {
		uint16_t value = snes.ppu.vram._data[i * 2] | (snes.ppu.vram._data[i * 2 + 1] << 8);
		REQUIRE(value == i);
	}
	REQUIRE(snes.cpu._dmaChannels[0].enabled == false);
}