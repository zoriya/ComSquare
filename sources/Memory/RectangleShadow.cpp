//
// Created by anonymus-raccoon on 2/4/20.
//

#include "RectangleShadow.hpp"

#include <utility>

namespace ComSquare::Memory
{
	RectangleShadow::RectangleShadow(std::shared_ptr<IRectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage)
		: _initial(std::move(initial))
	{
		this->setMemoryRegion(startBank, endBank, startPage, endPage);
	}

	std::shared_ptr<IMemory> RectangleShadow::createShadow(std::shared_ptr<IRectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage)
	{
		return static_cast<std::shared_ptr<IMemory>>(new RectangleShadow(std::move(initial), startBank, endBank, startPage, endPage));
	}

	uint8_t RectangleShadow::read_internal(uint24_t addr)
	{
		return this->_initial->read_internal(addr);
	}

	void RectangleShadow::write_internal(uint24_t addr, uint8_t data)
	{
		this->_initial->write_internal(addr, data);
	}
}