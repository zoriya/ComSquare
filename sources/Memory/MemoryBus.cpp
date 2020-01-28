//
// Created by anonymus-raccoon on 1/23/20.
//

#include <algorithm>
#include <iostream>
#include "MemoryBus.hpp"

namespace ComSquare
{
	std::shared_ptr<IMemory> MemoryBus::getAccessor(uint24_t addr)
	{
		auto it = std::find_if(this->_memoryAccessors.begin(), this->_memoryAccessors.end(), [addr](std::shared_ptr<IMemory> &accessor)
		{
			return accessor->hasMemoryAt(addr);
		});
		if (it == this->_memoryAccessors.end())
			return nullptr;
		return *it;
	}

	uint8_t MemoryBus::read(uint24_t addr)
	{
		std::shared_ptr<IMemory> handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Using open bus." << std::endl;
			return this->_openbus;
		}
		uint8_t data =  handler->read(addr - handler->getStart());
		this->_openbus = data;
		return data;
	}

	void MemoryBus::write(uint24_t addr, uint8_t data)
	{
		std::shared_ptr<IMemory> handler = this->getAccessor(addr);

		if (!handler) {
			std::cout << "Unknown memory accessor for address " << std::hex << addr << ". Warning, it was a write." << std::endl;
			return;
		}
		handler->write(addr - handler->getStart(), data);
	}
}