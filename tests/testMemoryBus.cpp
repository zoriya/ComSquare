//
// Created by Melefo on 05/02/2020.
//

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include "tests.hpp"
#include "../sources/Memory/MemoryBus.hpp"
#include "../sources/Memory/AMemory.hpp"
#include "../sources/SNES.hpp"
#include "../sources/Renderer/NoRenderer.hpp"
#include "../sources/Memory/MemoryShadow.hpp"
#include "../sources/Memory/RectangleShadow.hpp"
#include "../sources/PPU/PPU.hpp"
#include "../sources/Exceptions/InvalidAction.hpp"


using namespace ComSquare;

//////////////////////////////////
//								//
// MemoryBus::getAccessor tests //
//								//
//////////////////////////////////

Test(BusAccessor, GetWramStart)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x7E0000);
	cr_assert_eq(accessor.get(), snes.wram.get());
}

Test(BusAccessor, GetWramEnd)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x7FFFFF);
	cr_assert_eq(accessor.get(), snes.wram.get());
}

Test(BusAccessor, GetWramMirror)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x2F11FF));
	cr_assert_neq(accessor, nullptr);
	cr_assert_eq(accessor->_initial.get(), snes.wram.get());
}

Test(BusAccessor, GetWramMirror2)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x100000));
	cr_assert_neq(accessor, nullptr);
	cr_assert_eq(accessor->_initial.get(), snes.wram.get());
}

Test(BusAccessor, GetWramMirror3)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x1010));
	cr_assert_neq(accessor, nullptr);
	cr_assert_eq(accessor->_initial.get(), snes.wram.get());
}

Test(BusAccessor, GetOpenBus)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = snes._bus->getAccessor(0x897654);
	cr_assert_eq(accessor.get(), nullptr);
}

Test(BusAccessor, GetSramStart)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x700000));
	cr_assert_eq(accessor->_initial.get(), snes.sram.get());
}

Test(BusAccessor, GetSramEnd)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x7D7FFF));
	cr_assert_eq(accessor->_initial.get(), snes.sram.get());
}

Test(BusAccessor, GetSramMirror)
{
	Init()
	std::shared_ptr<Memory::ARectangleMemory> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::ARectangleMemory>(snes._bus->getAccessor(0xF00123));
	cr_assert_eq(accessor.get(), snes.sram.get());
}

Test(BusAccessor, GetAPUStart)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x002140);
	cr_assert_eq(accessor.get(), snes.apu.get());
}

Test(BusAccessor, GetAPUEnd)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x002143);
	cr_assert_eq(accessor.get(), snes.apu.get());
}

Test(BusAccessor, GetAPUMirror)
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes._bus->getAccessor(0xAB2143));
	cr_assert_eq(accessor->_initial.get(), snes.apu.get());
}

Test(BusAccessor, GetAPUMirrorFirstHalf)
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes._bus->getAccessor(0x052143));
	cr_assert_eq(accessor->_initial.get(), snes.apu.get());
}

Test(BusAccessor, GetCPUStart)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x004200);
	cr_assert_eq(accessor.get(), snes.cpu.get());
}

Test(BusAccessor, GetCPUEnd)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x00421F);
	cr_assert_eq(accessor.get(), snes.cpu.get());
}

Test(BusAccessor, GetPPU1Start)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x00213E);
	cr_assert_eq(accessor.get(), snes.ppu.get());
}

Test(BusAccessor, GetPPU1End)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x00213F);
	cr_assert_eq(accessor.get(), snes.ppu.get());
}

Test(BusAccessor, GetCPU)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x004212);
	cr_assert_eq(accessor.get(), snes.cpu.get());
}

Test(BusAccessor, GetPPU1Mirror)
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes._bus->getAccessor(0x80213F));
	cr_assert_eq(accessor->_initial.get(), snes.ppu.get());
}

Test(BusAccessor, GetCPU2Mirror)
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes._bus->getAccessor(0x804212));
	cr_assert_eq(accessor->_initial.get(), snes.cpu.get());
}

Test(BusAccessor, GetRomStart)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0x808000);
	cr_assert_eq(accessor.get(), snes.cartridge.get());
}

Test(BusAccessor, GetRomEnd)
{
	Init()
	std::shared_ptr<Memory::AMemory> accessor = nullptr;

	accessor = snes._bus->getAccessor(0xFFFFFF);
	cr_assert_eq(accessor.get(), snes.cartridge.get());
}

Test(BusAccessor, GetRomMirror)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x694200));
	cr_assert_eq(accessor->_initial.get(), snes.cartridge.get());
}

Test(BusAccessor, GetRomMirror2)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x01FEDC));
	cr_assert_eq(accessor->_initial.get(), snes.cartridge.get());
}

Test(BusAccessor, GetRomMirror3)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0xDE1248));
	cr_assert_eq(accessor->_initial.get(), snes.cartridge.get());
}

Test(BusAccessor, Get0x0)
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes._bus->getAccessor(0x0));
	cr_assert_eq(accessor->_initial.get(), snes.wram.get());
}

///////////////////////////
//						 //
// MemoryBus::read tests //
//						 //
///////////////////////////

Test(BusRead, Read0x0)
{
	Init()
	uint8_t data;

	snes.wram->_data[0] = 123;
	data = snes._bus->read(0x0);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadOutside, .init = cr_redirect_stdout)
{
	Init()
	uint8_t data;

	snes._bus->_openBus = 123;
	data = snes._bus->read(0x002000);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadOutside2, .init = cr_redirect_stdout)
{
	Init()
	uint8_t data;

	snes._bus->_openBus = 123;
	data = snes._bus->read(0xBF2FFF);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadOutside3, .init = cr_redirect_stdout)
{
	Init()
	uint8_t data;

	snes._bus->_openBus = 123;
	data = snes._bus->read(0x127654);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadAPU)
{
	Init()
	uint8_t data;

	snes.apu->_registers.port0 = 123;
	data = snes._bus->read(0x002140);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadROM)
{
	Init()
	uint8_t data;

	snes.cartridge->_data[5] = 123;
	data = snes._bus->read(0x808005);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadROMStart)
{
	Init()
	uint8_t data;

	snes.cartridge->_data[0] = 123;
	data = snes._bus->read(0x808000);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadCPU)
{
	Init()
	uint8_t data;

	snes.cpu->_internalRegisters.wrio = 123;
	data = snes._bus->read(0x004201);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadPPU)
{
	Init()
	uint8_t data;

	snes.ppu->_registers._mpy.mpyl = 123;
	data = snes._bus->read(0x002134);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadSRAM)
{
	Init()
	uint8_t data;

	snes.sram->_data[7] = 123;
	data = snes._bus->read(0x700007);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadWRAM)
{
	Init()
	uint8_t data;

	snes.wram->_data[3] = 123;
	data = snes._bus->read(0x7E0003);
	cr_assert_eq(data, 123);
}

Test(BusRead, ReadWRAM2)
{
	Init()
	uint8_t data;

	snes.wram->_data[0x1010] = 123;
	data = snes._bus->read(0x7E1010);
	cr_assert_eq(data, 123);
}


Test(BusRead, ReadWRAMMirror)
{
	Init()
	uint8_t data;

	snes.wram->_data[0x1010] = 123;
	data = snes._bus->read(0x1010);
	cr_assert_eq(data, 123);
}

////////////////////////////
//						  //
// MemoryBus::write tests //
//						  //
////////////////////////////

Test(BusWrite, Write0x0)
{
	Init()

	try {
		snes._bus->write(0x0, 123);
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	cr_assert_eq(snes.wram->_data[0], 123);
}


Test(BusWrite, WriteAPU)
{
	Init()

	snes._bus->write(0x002143, 123);
	cr_assert_eq(snes.apu->_registers.port3, 123);
}

Test(BusWrite, WritePPU)
{
	Init()

	snes._bus->write(0x002106, 123);
	cr_assert_eq(snes.ppu->_registers._mosaic.raw, 123);
}

Test(BusWrite, WriteCPU)
{
	Init()

	snes._bus->write(0x00420D, 123);
	cr_assert_eq(snes.cpu->_internalRegisters.memsel, 123);
}

Test(BusWrite, WriteROM)
{
	Init()

	cr_assert_throw(snes._bus->write(0x808005, 123), InvalidAction);
}

Test(BusWrite, WriteWRAM)
{
	Init()

	snes._bus->write(0x7E0002, 123);
	cr_assert_eq(snes.wram->_data[2], 123);
}

Test(BusWrite, WriteSRAM)
{
	Init()

	snes._bus->write(0x700009, 123);
	cr_assert_eq(snes.sram->_data[9], 123);
}