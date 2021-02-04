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

	uint8_t MemoryShadow::read(uint24_t addr)
	{
		return this->_initial->read(addr);
	}

	void MemoryShadow::write(uint24_t addr, uint8_t data)
	{
		return this->_initial->write(addr, data);
	}

	uint24_t MemoryShadow::getSize() const
	{
		return this->_initial->getSize();
	}

	bool MemoryShadow::isMirror() const
	{
		return true;
	}

	std::shared_ptr<IMemory> MemoryShadow::getMirrored() const
	{
		return this->_initial;
	}

	std::string MemoryShadow::getName() const
	{
		return this->_initial->getName();
	}

	Component MemoryShadow::getComponent() const
	{
		return this->_initial->getComponent();
	}
}