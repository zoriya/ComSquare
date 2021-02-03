//
// Created by anonymus-raccoon on 4/6/20.
//

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "tests.hpp"
#include "../sources/Memory/RectangleShadow.hpp"

using namespace ComSquare;

Test(RectangleMemory, HorizontalRamRead)
{
	Ram::Ram ram(0xFF, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0001);
	for (int i = 0x00; i < 0xFF; i++)
		ram._data[i] = i;
	for (uint24_t i = 0x000000; i < 0xFF0000; i += 0x010000) {
		uint8_t value = ram.read(ram.getRelativeAddress(i));
		cr_assert_eq(value, i >> 16u, "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", value, i >> 16, i);
	}
}

Test(RectangleMemory, HorizontalRamWrite)
{
	Ram::Ram ram(0xFF, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0001);
	for (uint24_t i = 0x000000; i < 0xFF0000; i += 0x010000)
		ram.write(ram.getRelativeAddress(i), i >> 16u);
	for (int i = 0x00; i < 0xFF; i++)
		cr_assert_eq(ram._data[i], i, "The ram's write put 0x%x but it should had put: 0x%x (addr: 0x%06x)", ram._data[i], i, i << 16u);
}

Test(RectangleMemory, DualLineRamRead)
{
	Ram::Ram ram(0xFF * 2, Component::Rom, "Rom");
	ram.setMemoryRegion(0x00, 0xFF, 0x0000, 0x0002);
	for (int i = 0x00; i < 0xFF * 2; i++)
		ram._data[i] = i;
	for (uint24_t i = 0x000000, v = 0; v < 0xFF * 2; i += 0x010000, v += 2) {
		uint8_t value = ram.read(ram.getRelativeAddress(i));
		cr_assert_eq(value, (uint8_t)(v), "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", value, (uint8_t)(v), i);
		value = ram.read(ram.getRelativeAddress(i + 1));
		cr_assert_eq(value, (uint8_t)(v + 1), "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", value, (uint8_t)(v + 1), i + 1);
	}
}

Test(RectangleMemory, HorizontalRamShadowRead)
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0xFF, Component::Rom, "Rom");
	ram->setMemoryRegion(0x00, 0xFF, 0x0000, 0x0001);
	Memory::RectangleShadow shadow(ram, 0x00, 0xFF, 0x8000, 0x8001);
	for (int i = 0x00; i < 0xFF; i++)
		ram->_data[i] = i;
	for (uint24_t i = 0x008000; i < 0xFF8000; i += 0x010000) {
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		cr_assert_eq(v, i >> 16u, "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", v, i >> 16, i);
	}}

Test(RectangleMemory, HorizontalRamShadowReadWithBankOffset)
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0xFF, Component::Rom, "Rom");
	ram->setMemoryRegion(0x00, 0xFF, 0x0000, 0x0001);
	Memory::RectangleShadow shadow(ram, 0x80, 0xFF, 0x8000, 0x8001);
	for (int i = 0x00; i < 0xFF; i++)
		ram->_data[i] = i;
	shadow.setBankOffset(0x80);
	for (uint24_t i = 0x808000; i < 0xFF8000; i += 0x010000) {
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		cr_assert_eq(v, i >> 16u, "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", v, i >> 16, i);
	}
}

Test(RectangleMemory, ShadowOffsetCartridge)
{
	std::shared_ptr<Ram::Ram> ram = std::make_shared<Ram::Ram>(0x3fff80, Component::Rom, "Rom");
	ram->setMemoryRegion(0x80, 0xFF, 0x8000, 0xFFFF);
	Memory::RectangleShadow shadow(ram, 0xC0, 0xEF, 0x0000, 0x7FFF);
	for (int i = 0x00; i < 0x3fff80; i++)
		ram->_data[i] = i;
	shadow.setBankOffset(0x40);
	for (uint24_t i = 0xC00000; i < 0xEF7FFF; i += 0x1) {
		if ((uint16_t)i > 0x7FFFu)
			i += 0x010000 - 0x8000;
		uint8_t v = shadow.read(shadow.getRelativeAddress(i));
		uint8_t r = ram->read(ram->getRelativeAddress(i + 0x8000));
		cr_assert_eq(v, r, "The ram's read returned 0x%x but the internal ram value was: 0x%x (addr: 0x%06x)", v, r, i);
	}
}