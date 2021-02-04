//
// Created by anonymus-raccoon on 2/4/20.
//

#include "RectangleShadow.hpp"
#include <utility>
#include <iostream>

namespace ComSquare::Memory
{
	RectangleShadow::RectangleShadow(std::shared_ptr<IMemory> initial,
	                                 uint8_t startBank,
	                                 uint8_t endBank,
	                                 uint16_t startPage,
	                                 uint16_t endPage)
		: _initial(std::move(initial))
	{
		this->setMemoryRegion(startBank, endBank, startPage, endPage);
	}

	uint24_t RectangleShadow::getRelativeAddress(uint24_t addr) const
	{
		uint24_t base = ARectangleMemory::getRelativeAddress(addr);
		return base + this->_bankOffset * (this->_endPage - this->_startPage);
	}

	uint8_t RectangleShadow::read(uint24_t addr) const
	{
		return this->_initial->read(addr);
	}

	void RectangleShadow::write(uint24_t addr, uint8_t data)
	{
		return this->_initial->write(addr, data);
	}

	RectangleShadow *RectangleShadow::setBankOffset(uint8_t bankOffset)
	{
		this->_bankOffset = bankOffset;
		return this;
	}

	uint24_t RectangleShadow::getSize() const
	{
		return this->_initial->getSize();
	}

	bool RectangleShadow::isMirror() const
	{
		return true;
	}

	std::shared_ptr<IMemory> RectangleShadow::getMirrored() const
	{
		return this->_initial;
	}

	std::string RectangleShadow::getName() const
	{
		return this->_initial->getName();
	}

	Component RectangleShadow::getComponent() const
	{
		return this->_initial->getComponent();
	}
}