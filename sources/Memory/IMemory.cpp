//
// Created by anonymus-raccoon on 1/23/20.
//

#include "IMemory.hpp"
#include <algorithm>

namespace ComSquare
{
	void IMemory::setMemoryRegion(uint32_t start, uint32_t end)
	{
		this->_start = start;
		this->_end = end;
	}

	bool IMemory::hasMemorydAt(uint32_t addr)
	{
		return this->_start <= addr && addr <= this->_end;
	}

	uint32_t IMemory::getStart()
	{
		return this->_start;
	}
}