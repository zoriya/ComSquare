//
// Created by anonymus-raccoon on 1/23/20.
//

#include "AMemory.hpp"

namespace ComSquare::Memory
{
	uint24_t AMemory::getRelativeAddress(uint24_t addr) const
	{
		return addr - this->_start;
	}

	void AMemory::setMemoryRegion(uint24_t start, uint24_t end)
	{
		this->_start = start;
		this->_end = end;
	}

	bool AMemory::hasMemoryAt(uint24_t addr) const
	{
		return this->_start <= addr && addr <= this->_end;
	}

	bool AMemory::isMirror() const
	{
		return false;
	}

	std::shared_ptr<IMemory> AMemory::getMirrored() const
	{
		return nullptr;
	}

	std::string AMemory::getValueName(uint24_t) const
	{
		return "???";
	}
}