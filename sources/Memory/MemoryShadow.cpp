//
// Created by anonymus-raccoon on 1/28/20.
//

#include "MemoryShadow.hpp"

#include <utility>

namespace ComSquare::Memory
{
	MemoryShadow::MemoryShadow(std::shared_ptr<AMemory> initial, uint24_t start, uint24_t end)
		: _initial(std::move(initial))
	{
		this->setMemoryRegion(start, end);
	}

	uint8_t MemoryShadow::read(uint24_t addr)
	{
		return this->_initial->read(addr);
	}

	void MemoryShadow::write(uint24_t addr, uint8_t data)
	{
		return this->_initial->write(addr, data);
	}

	bool MemoryShadow::isMirror()
	{
		return true;
	}

	std::shared_ptr<AMemory> MemoryShadow::getMirrored()
	{
		return this->_initial;
	}

	std::string MemoryShadow::getName()
	{
		return this->_initial->getName();
	}

	Component MemoryShadow::getComponent()
	{
		return this->_initial->getComponent();
	}
}