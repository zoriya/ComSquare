//
// Created by anonymus-raccoon on 1/28/20.
//

#include "MemoryShadow.hpp"

#include <utility>

namespace ComSquare::Memory
{
	MemoryShadow::MemoryShadow(std::shared_ptr<IMemory> initial, uint24_t start, uint24_t end)
		: _initial(std::move(initial))
	{
		this->setMemoryRegion(start, end);
	}

	std::shared_ptr<IMemory> MemoryShadow::createShadow(std::shared_ptr<IMemory> initial, uint24_t start, uint24_t end)
	{
		return static_cast<std::shared_ptr<IMemory>>(new MemoryShadow(std::move(initial), start, end));
	}

	uint8_t MemoryShadow::read(uint24_t addr)
	{
		return this->_initial->read(addr);
	}

	void MemoryShadow::write(uint24_t addr, uint8_t data)
	{
		return this->_initial->write(addr, data);
	}
}