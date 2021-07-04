//
// Created by Melefo on 05/02/2020.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "tests.hpp"
#include "../sources/Memory/MemoryShadow.hpp"
#include "../sources/Memory/RectangleShadow.hpp"
#include "../sources/Exceptions/InvalidAction.hpp"


using namespace ComSquare;

//////////////////////////////////
//								//
// MemoryBus::getAccessor tests //
//								//
//////////////////////////////////

TEST_CASE("GetWramStart BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x7E0000);
	REQUIRE(accessor.get() == snes.wram.get());
}

TEST_CASE("GetWramEnd BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x7FFFFF);
	REQUIRE(accessor.get() == snes.wram.get());
}

TEST_CASE("GetWramMirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x2F11FF));
	REQUIRE(accessor != nullptr);
	REQUIRE(accessor->_initial.get() == snes.wram.get());
}

TEST_CASE("GetWramMirror2 BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x100000));
	REQUIRE(accessor != nullptr);
	REQUIRE(accessor->_initial.get() == snes.wram.get());
}

TEST_CASE("GetWramMirror3 BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x1010));
	REQUIRE(accessor != nullptr);
	REQUIRE(accessor->_initial.get() == snes.wram.get());
}

TEST_CASE("GetOpenBus BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor = snes.bus.getAccessor(0x897654);
	REQUIRE(accessor.get() == nullptr);
}

TEST_CASE("GetSramStart BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x700000));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.sram.get());
}

TEST_CASE("GetSramEnd BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x7D7FFF));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.sram.get());
}

TEST_CASE("GetSramMirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::ARectangleMemory> accessor;

	accessor = std::static_pointer_cast<Memory::ARectangleMemory>(snes.bus.getAccessor(0xF00123));
	REQUIRE(accessor.get() == snes.sram.get());
}

TEST_CASE("GetAPUStart BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x002140);
	REQUIRE(accessor.get() == snes.apu.get());
}

TEST_CASE("GetAPUEnd BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x002143);
	REQUIRE(accessor.get() == snes.apu.get());
}

TEST_CASE("GetAPUMirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes.bus.getAccessor(0xAB2143));
	REQUIRE(accessor->_initial.get() == snes.apu.get());
}

TEST_CASE("GetAPUMirrorFirstHalf BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes.bus.getAccessor(0x052143));
	REQUIRE(accessor->_initial.get() == snes.apu.get());
}

TEST_CASE("GetCPUStart BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x004200);
	REQUIRE(accessor.get() == snes.cpu.get());
}

TEST_CASE("GetCPUEnd BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x00421F);
	REQUIRE(accessor.get() == snes.cpu.get());
}

TEST_CASE("GetPPU1Start BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x00213E);
	REQUIRE(accessor.get() == snes.ppu.get());
}

TEST_CASE("GetPPU1End BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x00213F);
	REQUIRE(accessor.get() == snes.ppu.get());
}

TEST_CASE("GetCPU BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x004212);
	REQUIRE(accessor.get() == snes.cpu.get());
}

TEST_CASE("GetPPU1Mirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes.bus.getAccessor(0x80213F));
	REQUIRE(accessor->_initial.get() == snes.ppu.get());
}

TEST_CASE("GetCPU2Mirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::MemoryShadow> accessor;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(snes.bus.getAccessor(0x804212));
	REQUIRE(accessor->_initial.get() == snes.cpu.get());
}

TEST_CASE("GetRomStart BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0x808000);
	REQUIRE(accessor.get() == snes.cartridge.get());
}

TEST_CASE("GetRomEnd BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::IMemory> accessor;

	accessor = snes.bus.getAccessor(0xFFFFFF);
	REQUIRE(accessor.get() == snes.cartridge.get());
}

TEST_CASE("GetRomMirror BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x694200));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.cartridge.get());
}

TEST_CASE("GetRomMirror2 BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x01FEDC));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.cartridge.get());
}

TEST_CASE("GetRomMirror3 BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0xDE1248));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.cartridge.get());
}

TEST_CASE("Get0x0 BusAccessor", "[BusAccessor]")
{
	Init()
	std::shared_ptr<Memory::RectangleShadow> accessor;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(snes.bus.getAccessor(0x0));
	REQUIRE(accessor);
	REQUIRE(accessor->_initial.get() == snes.wram.get());
}

///////////////////////////
//						 //
// MemoryBus::read tests //
//						 //
///////////////////////////

TEST_CASE("Read0x0 BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.wram->_data[0] = 123;
	data = snes.bus.read(0x0);
	REQUIRE(data == 123);
}

TEST_CASE("Read ReadOutside", "[ReadOutside]")
{
	Init()
	uint8_t data;

	snes.bus._openBus = 123;
	data = snes.bus.read(0x002000);
	REQUIRE(data == 123);
}

TEST_CASE("ReadOutside2 Read][Bus", "[Read][Bus]")
{
	Init()
	uint8_t data;

	snes.bus._openBus = 123;
	data = snes.bus.read(0xBF2FFF);
	REQUIRE(data == 123);
}

TEST_CASE("ReadOutside3 Read][Bus", "[Read][Bus]")
{
	Init()
	uint8_t data;

	snes.bus._openBus = 123;
	data = snes.bus.read(0x127654);
	REQUIRE(data == 123);
}

TEST_CASE("ReadAPU BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.apu->_registers.port0 = 123;
	data = snes.bus.read(0x002140);
	REQUIRE(data == 123);
}

TEST_CASE("ReadROM BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.cartridge->_data[5] = 123;
	data = snes.bus.read(0x808005);
	REQUIRE(data == 123);
}

TEST_CASE("ReadROMStart BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.cartridge->_data[0] = 123;
	data = snes.bus.read(0x808000);
	REQUIRE(data == 123);
}

TEST_CASE("ReadCPU BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.cpu->_internalRegisters.wrio = 123;
	data = snes.bus.read(0x004201);
	REQUIRE(data == 123);
}

TEST_CASE("ReadPPU BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.ppu->_registers._mpy.mpyl = 123;
	data = snes.bus.read(0x002134);
	REQUIRE(data == 123);
}

TEST_CASE("ReadSRAM BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.sram->_data[7] = 123;
	data = snes.bus.read(0x700007);
	REQUIRE(data == 123);
}

TEST_CASE("ReadWRAM BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.wram->_data[3] = 123;
	data = snes.bus.read(0x7E0003);
	REQUIRE(data == 123);
}

TEST_CASE("ReadWRAM2 BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.wram->_data[0x1010] = 123;
	data = snes.bus.read(0x7E1010);
	REQUIRE(data == 123);
}


TEST_CASE("ReadWRAMMirror BusRead", "[BusRead]")
{
	Init()
	uint8_t data;

	snes.wram->_data[0x1010] = 123;
	data = snes.bus.read(0x1010);
	REQUIRE(data == 123);
}

////////////////////////////
//						  //
// MemoryBus::write tests //
//						  //
////////////////////////////

TEST_CASE("Write0x0 BusWrite", "[BusWrite]")
{
	Init()

	try {
		snes.bus.write(0x0, 123);
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
	REQUIRE(snes.wram->_data[0] == 123);
}


TEST_CASE("WriteAPU BusWrite", "[BusWrite]")
{
	Init()

	snes.bus.write(0x002143, 123);
	REQUIRE(snes.apu->_registers.port3 == 123);
}

TEST_CASE("WritePPU BusWrite", "[BusWrite]")
{
	Init()

	snes.bus.write(0x002106, 123);
	REQUIRE(snes.ppu->_registers._mosaic.raw == 123);
}

TEST_CASE("WriteCPU BusWrite", "[BusWrite]")
{
	Init()

	snes.bus.write(0x00420D, 123);
	REQUIRE(snes.cpu->_internalRegisters.memsel == 123);
}

TEST_CASE("WriteROM BusWrite", "[BusWrite]")
{
	Init()

	REQUIRE_THROWS_AS(snes.bus.write(0x808005, 123), InvalidAction);
}

TEST_CASE("WriteWRAM BusWrite", "[BusWrite]")
{
	Init()

	snes.bus.write(0x7E0002, 123);
	REQUIRE(snes.wram->_data[2] == 123);
}

TEST_CASE("WriteSRAM BusWrite", "[BusWrite]")
{
	Init()

	snes.bus.write(0x700009, 123);
	REQUIRE(snes.sram->_data[9] == 123);
}