//
// Created by anonymus-raccoon on 1/29/20.
//

#include "IRectangleMemory.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Memory
{
	uint8_t IRectangleMemory::read(uint24_t addr)
	{
		uint8_t bank = addr >> 16u;
		uint16_t page = addr;
		unsigned bankCount = bank - this->_startBank;
		unsigned pageCount = this->_endPage - this->_startPage;

		if (bank < this->_startBank || bank >= this->_endBank)
			throw InvalidAddress("Rectangle memory read Invalid Bank", addr);
		if (page < this->_startPage || page >= this->_endPage)
			throw InvalidAddress("Rectangle memory read Invalid Page", addr);
		page -= this->_startPage;
		page += pageCount * bankCount;
		return this->read_internal(page);
	}

	void IRectangleMemory::write(uint24_t addr, uint8_t data)
	{
		uint8_t bank = addr >> 16u;
		uint16_t page = addr;
		unsigned bankCount = bank - this->_startBank;
		unsigned pageCount = this->_endPage - this->_startPage;

		if (bank < this->_startBank || bank >= this->_endBank)
			throw InvalidAddress("Rectangle memory write Invalid Bank", addr);
		if (page < this->_startPage || page >= this->_endPage)
			throw InvalidAddress("Rectangle memory write Invalid Page", addr);
		page -= this->_startPage;
		page += pageCount * bankCount;
		this->write_internal(page, data);
	}

	bool IRectangleMemory::hasMemoryAt(uint24_t addr)
	{
		uint8_t bank = addr >> 16u;
		uint16_t page = addr;

		if (this->_startBank <= bank && bank < this->_endBank)
			if (this->_startPage <= page && page < this->_endPage)
				return true;
		return false;
	}

	uint24_t IRectangleMemory::getStart()
	{
		return this->_startBank + this->_startPage;
	}

	void IRectangleMemory::setMemoryRegion(uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage)
	{
		this->_startBank = startBank;
		this->_endBank = endBank;
		this->_startPage = startPage;
		this->_endPage = endPage;
	}
}