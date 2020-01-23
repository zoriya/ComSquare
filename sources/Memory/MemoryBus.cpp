//
// Created by anonymus-raccoon on 1/23/20.
//

#include <ios>
#include <iostream>
#include "MemoryBus.hpp"

namespace ComSquare
{
	IMemory *MemoryBus::getAccessor(uint32_t addr)
	{
		return std::find_if(this->_memoryAccessors.begin(), this->_memoryAccessors.end(), [addr](IMemory *accessor)
		{
			return accessor->hasMemorydAt(addr);
		}).base();
	}

	uint8_t MemoryBus::read(uint32_t addr)
	{
		IMemory *handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Using open bus." << std::endl;
			return this->_openbus;
		}
		uint8_t data =  handler->read(addr - handler->getStart());
		this->_openbus = data;
		return data;
	}

	void MemoryBus::write(uint32_t addr, uint8_t data)
	{
		IMemory *handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Warning, it was a write." << std::endl;
			return;
		}
		handler->write(addr - handler->getStart(), data);
	}
}