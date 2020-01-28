//
// Created by anonymus-raccoon on 1/28/20.
//

#include "MemoryShadow.hpp"

#include <utility>

namespace ComSquare::Memory
{
	MemoryShadow::MemoryShadow(std::shared_ptr<IMemory> initial)
		: _initial(std::move(initial))
	{ }

	uint8_t MemoryShadow::read(uint24_t addr)
	{
		return this->_initial->read(addr);
	}

	void MemoryShadow::write(uint24_t addr, uint8_t data)
	{
		return this->_initial->write(addr, data);
	}
}