//
// Created by Melefo on 05/02/2020.
//

#include <criterion/criterion.h>
#include <iostream>
#include "communism.hpp"
#include "../sources/Memory/MemoryBus.hpp"
#include "../sources/Memory/IMemory.hpp"
#include "../sources/SNES.hpp"
#include "../sources/Renderer/NoRenderer.hpp"
#include "../sources/Memory/MemoryShadow.hpp"
#include "../sources/Memory/RectangleShadow.hpp"


using namespace ComSquare;

std::pair<Memory::MemoryBus, SNES> Init()
{
	Memory::MemoryBus bus;
	Renderer::NoRenderer norenderer(0, 0, 0);
	SNES snes(std::make_shared<Memory::MemoryBus>(bus), "", norenderer);
	snes.cartridge->_size = 10;
	snes.cartridge->_data = new uint8_t[snes.cartridge->_size];
	snes.cartridge->header.mappingMode = Cartridge::LoRom;
	bus.mapComponents(snes);
	return std::make_pair(bus, snes);
}

//////////////////////////////////
//								//
// MemoryBus::getAccessor tests //
//								//
//////////////////////////////////

Test(BusAccessor, GetWramStart)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x7E0000);
	cr_assert_eq(accessor.get(), pair.second.wram.get());
}

Test(BusAccessor, GetWramEnd)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x7FFFFF);
	cr_assert_eq(accessor.get(), pair.second.wram.get());
}

Test(BusAccessor, GetWramMirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(pair.first.getAccessor(0x2F11FF));
	cr_assert_eq(accessor->_initial.get(), pair.second.wram.get());
}

Test(BusAccessor, GetOpenBus)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = pair.first.getAccessor(0x897654);
	cr_assert_eq(accessor.get(), nullptr);
}

Test(BusAccessor, GetSramStart)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0x700000));
	cr_assert_eq(accessor->_initial.get(), pair.second.sram.get());
}

Test(BusAccessor, GetSramEnd)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0x7D7FFF));
	cr_assert_eq(accessor->_initial.get(), pair.second.sram.get());
}

Test(BusAccessor, GetSramMirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::IRectangleMemory> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::IRectangleMemory>(pair.first.getAccessor(0xF00123));
	cr_assert_eq(accessor.get(), pair.second.sram.get());
}

Test(BusAccessor, GetSramMirror2)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	// TODO implement the SRam accessor for the FE/FF.
	//std::cout << pair.first.getAccessor(0xFE0123) << std::endl;
	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0xFE0123));
	cr_assert_eq(accessor->_initial.get(), pair.second.sram.get());
}

Test(BusAccessor, GetAPUStart)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x002140);
	cr_assert_eq(accessor.get(), pair.second.apu.get());
}

Test(BusAccessor, GetAPUEnd)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x002143);
	cr_assert_eq(accessor.get(), pair.second.apu.get());
}

Test(BusAccessor, GetAPUMirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(pair.first.getAccessor(0xAB2143));
	cr_assert_eq(accessor->_initial.get(), pair.second.apu.get());
}

Test(BusAccessor, GetCPUStart)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x004200);
	cr_assert_eq(accessor.get(), pair.second.cpu.get());
}

Test(BusAccessor, GetCPUEnd)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x00421F);
	cr_assert_eq(accessor.get(), pair.second.cpu.get());
}

Test(BusAccessor, GetPPU1Start)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x00213E);
	cr_assert_eq(accessor.get(), pair.second.ppu.get());
}

Test(BusAccessor, GetPPU1End)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x00213F);
	cr_assert_eq(accessor.get(), pair.second.ppu.get());
}

Test(BusAccessor, GetCPU)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x004212);
	cr_assert_eq(accessor.get(), pair.second.cpu.get());
}

Test(BusAccessor, GetPPU1Mirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(pair.first.getAccessor(0x80213F));
	cr_assert_eq(accessor->_initial.get(), pair.second.ppu.get());
}

Test(BusAccessor, GetCPU2Mirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::MemoryShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::MemoryShadow>(pair.first.getAccessor(0x804212));
	cr_assert_eq(accessor->_initial.get(), pair.second.cpu.get());
}

Test(BusAccessor, GetRomStart)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x808000);
	cr_assert_eq(accessor.get(), pair.second.cartridge.get());
}

Test(BusAccessor, GetRomEnd)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0xFFFFFF);
	cr_assert_eq(accessor.get(), pair.second.cartridge.get());
}

Test(BusAccessor, GetRomMirror)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0x694200));
	cr_assert_eq(accessor->_initial.get(), pair.second.cartridge.get());
}

Test(BusAccessor, GetRomMirror2)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0x01FEDC));
	cr_assert_eq(accessor->_initial.get(), pair.second.cartridge.get());
}

Test(BusAccessor, GetRomMirror3)
{
	auto pair = Init();
	std::shared_ptr<Memory::RectangleShadow> accessor = nullptr;

	accessor = std::static_pointer_cast<Memory::RectangleShadow>(pair.first.getAccessor(0xDE1248));
	cr_assert_eq(accessor->_initial.get(), pair.second.cartridge.get());
}

////////////////////////////
//						  //
// MemoryBus::write tests //
//						  //
////////////////////////////

Test(BusWrite, )
{

}

///////////////////////////
//						 //
// MemoryBus::read tests //
//						 //
///////////////////////////

Test(BusRead, )
{

}

////////////////////////////////////
//								  //
// MemoryBus::mapComponents tests //
//								  //
////////////////////////////////////

Test(BusMapping, )
{

}

////////////////////////////////////////
//									  //
// MemoryBus::_mirrorComponents tests //
//									  //
////////////////////////////////////////

Test(BusMirror, )
{

}