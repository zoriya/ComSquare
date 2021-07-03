//
// Created by anonymus-raccoon on 4/6/20.
//

#include <catch2/catch.hpp>
#include "tests.hpp"
#include "../sources/Memory/RectangleShadow.hpp"

using namespace ComSquare;

TEST_CASE("HorizontalRamRead RectangleMemory", "[RectangleMemory]")
{
	Ram::Ram ram(0xFF, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0000);
	for (int i = 0x00; i < 0xFF; i++)
		ram._data[i] = i;
	for (uint24_t i = 0x000000; i < 0xFF0000; i += 0x010000) {
		uint8_t value = ram.read(ram.getRelativeAddress(i));
		REQUIRE(value == i >> 16u);
	}
}

TEST_CASE("HorizontalRamWrite RectangleMemory", "[RectangleMemory]")
{
	Ram::Ram ram(0xFF, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0000);
	for (uint24_t i = 0x000000; i < 0xFF0000; i += 0x010000)
		ram.write(ram.getRelativeAddress(i), i >> 16u);
	for (int i = 0x00; i < 0xFF; i++)
		REQUIRE(ram._data[i] == i);
}

TEST_CASE("DualLineRamRead RectangleMemory", "[RectangleMemory]")
{
	Ram::Ram ram(0xFF * 2, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0001);
	for (int i = 0x00; i < 0xFF * 2; i++)
		ram._data[i] = i;
	for (uint24_t i = 0x000000, v = 0; v < 0xFF * 2; i += 0x010000, v += 2) {
		uint8_t value = ram.read(ram.getRelativeAddress(i));
		REQUIRE(value == (uint8_t)(v));
		value = ram.read(ram.getRelativeAddress(i + 1));
		REQUIRE(value == (uint8_t)(v + 1));
	}
}

TEST_CASE("HorizontalRamShadowRead RectangleMemory", "[RectangleMemory]")
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0xFF, Component::Rom, "Rom");
	ram->setMemoryRegion(0x00, 0xFF, 0x0000, 0x0000);
	Memory::RectangleShadow shadow(ram, 0x00, 0xFF, 0x8000, 0x8000);
	for (int i = 0x00; i < 0xFF; i++)
		ram->_data[i] = i;
	for (uint24_t i = 0x008000; i < 0xFF8000; i += 0x010000) {
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		REQUIRE(v == i >> 16u);
	}}

TEST_CASE("HorizontalRamShadowReadWithBankOffset RectangleMemory", "[RectangleMemory]")
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0xFF, Component::Rom, "Rom");
	ram->setMemoryRegion(0x00, 0xFF, 0x0000, 0x0000);
	Memory::RectangleShadow shadow(ram, 0x80, 0xFF, 0x8000, 0x8000);
	for (int i = 0x00; i < 0xFF; i++)
		ram->_data[i] = i;
	shadow.setBankOffset(0x80);
	for (uint24_t i = 0x808000; i < 0xFF8000; i += 0x010000) {
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		REQUIRE(v == i >> 16u);
	}
}

TEST_CASE("ShadowOffsetCartridge RectangleMemory", "[RectangleMemory]")
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0x3fff80, Component::Rom, "Rom");
	ram->setMemoryRegion(0x80, 0xFF, 0x8000, 0xFFFF);
	Memory::RectangleShadow shadow(ram, 0xC0, 0xEF, 0x0000, 0x7FFF);
	for (int i = 0x00; i < 0x3fff80; i++)
		ram->_data[i] = i;
	shadow.setBankOffset(0x40);
	for (uint24_t i = 0xC00000; i <= 0xEF7FFF; i += 0x1) {
		if ((uint16_t)i > 0x7FFFu)
			i += 0x010000 - 0x8000;
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		uint8_t r = ram->read(ram->getRelativeAddress(i + 0x8000));
		REQUIRE(v == r);
	}
}