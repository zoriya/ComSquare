//
// Created by anonymus-raccoon on 1/23/20.
//

#include "AMemory.hpp"
#include <algorithm>

namespace ComSquare::Memory
{
	void AMemory::setMemoryRegion(uint24_t start, uint24_t end)
	{
		this->_start = start;
		this->_end = end;
	}

	bool AMemory::hasMemoryAt(uint24_t addr)
	{
		return this->_start <= addr && addr <= this->_end;
	}

	uint32_t AMemory::getStart()
	{
		return this->_start;
	}

	bool AMemory::isMirror()
	{
		return false;
	}

	std::shared_ptr<AMemory> AMemory::getMirrored()
	{
		return nullptr;
	}

	std::string AMemory::getValueName(uint24_t)
	{
		return "???";
	}
}