//
// Created by anonymus-raccoon on 1/23/20.
//

#include "IMemory.hpp"
#include <algorithm>

namespace ComSquare
{
	void IMemory::setMemoryRegion(uint24_t start, uint24_t end)
	{
		this->_start = start;
		this->_end = end;
	}

	bool IMemory::hasMemoryAt(uint24_t addr)
	{
		return this->_start <= addr && addr <= this->_end;
	}

	uint32_t IMemory::getStart()
	{
		return this->_start;
	}
}