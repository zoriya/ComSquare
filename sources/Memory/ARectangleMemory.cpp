//
// Created by anonymus-raccoon on 1/29/20.
//

#include <iostream>
#include "ARectangleMemory.hpp"
#include "Exceptions/InvalidAddress.hpp"

namespace ComSquare::Memory
{
	uint24_t ARectangleMemory::getRelativeAddress(uint24_t addr) const
	{
		uint8_t bank = addr >> 16u;
		uint16_t page = addr;
		unsigned bankCount = bank - this->_startBank;
		unsigned pageCount = this->_endPage + 1 - this->_startPage;

		if (bank < this->_startBank || bank > this->_endBank)
			throw InvalidAddress("Rectangle memory: Invalid Bank", addr);
		if (page < this->_startPage || page > this->_endPage)
			throw InvalidAddress("Rectangle memory: Invalid Page", addr);
		page -= this->_startPage;
		return pageCount * bankCount + page;
	}

	bool ARectangleMemory::hasMemoryAt(uint24_t addr) const
	{
		uint8_t bank = addr >> 16u;
		uint16_t page = addr;

		if (this->_startBank <= bank && bank <= this->_endBank)
			if (this->_startPage <= page && page <= this->_endPage)
				return true;
		return false;
	}

	void ARectangleMemory::setMemoryRegion(uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage)
	{
		this->_startBank = startBank;
		this->_endBank = endBank;
		this->_startPage = startPage;
		this->_endPage = endPage;
	}

	std::string ARectangleMemory::getValueName(uint24_t) const
	{
		return "???";
	}
}