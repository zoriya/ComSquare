//
// Created by anonymus-raccoon on 2/4/20.
//

#include "RectangleShadow.hpp"

#include <utility>
#include <iostream>

namespace ComSquare::Memory
{
	RectangleShadow::RectangleShadow(std::shared_ptr<ARectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage)
		: _initial(std::move(initial))
	{
		this->setMemoryRegion(startBank, endBank, startPage, endPage);
	}

	uint8_t RectangleShadow::read_internal(uint24_t addr)
	{
		addr += this->_bankOffset << 16u;
		return this->_initial->read_internal(addr);
	}

	void RectangleShadow::write_internal(uint24_t addr, uint8_t data)
	{
		addr += this->_bankOffset << 16u;
		this->_initial->write_internal(addr, data);
	}

	RectangleShadow *RectangleShadow::setBankOffset(uint8_t bankOffset)
	{
		this->_bankOffset = bankOffset;
		return this;
	}

	bool RectangleShadow::isMirror()
	{
		return true;
	}

	std::shared_ptr<AMemory> RectangleShadow::getMirrored()
	{
		return this->_initial;
	}

	std::string RectangleShadow::getName()
	{
		return this->_initial->getName();
	}
}