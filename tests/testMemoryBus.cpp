//
// Created by Melefo on 05/02/2020.
//

#include <criterion/criterion.h>
#include "communism.hpp"
#include "../sources/Memory/MemoryBus.hpp"
#include "../sources/Memory/IMemory.hpp"
#include "../sources/SNES.hpp"
#include "../sources/Renderer/NoRenderer.hpp"


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
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x2F11FF);
	cr_assert_eq(accessor.get(), pair.second.wram.get());
}

Test(BusAccessor, GetSram)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x700123);
	cr_assert_eq(accessor.get(), pair.second.sram.get());
}

Test(BusAccessor, GetAPU)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x002142);
	cr_assert_eq(accessor.get(), pair.second.apu.get());
}

Test(BusAccessor, GetCPU)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x00421F);
	cr_assert_eq(accessor.get(), pair.second.cpu.get());
}

Test(BusAccessor, GetPPU)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0x00213F);
	cr_assert_eq(accessor.get(), pair.second.ppu.get());
}

Test(BusAccessor, GetRom)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0XFFFFFF);
	cr_assert_eq(accessor.get(), pair.second.cartridge.get());
}

Test(BusWrite, )
{

}

Test(BusRead, )
{

}

Test(BusMapping, )
{

}

Test(BusMirror, )
{

}