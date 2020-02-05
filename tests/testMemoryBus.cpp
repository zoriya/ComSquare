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

Test(BusAccessor, GetRom)
{
	auto pair = Init();
	std::shared_ptr<Memory::IMemory> accessor = nullptr;

	accessor = pair.first.getAccessor(0xFFFFEF);
	printf("%p %p", accessor.get(), pair.second.cartridge.get());
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