//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_MEMORYBUS_HPP
#define COMSQUARE_MEMORYBUS_HPP

#include <cstdint>
#include <vector>
#include "IMemory.hpp"

namespace ComSquare
{
	class MemoryBus {
	private:
		std::vector<IMemory> _memoryAccessors;
		IMemory * getAccessor(uint32_t addr);
		uint8_t _openbus;
	public:
		uint8_t read(uint32_t addr);
		void write(uint32_t addr, uint8_t data);
	};
}


#endif //COMSQUARE_MEMORYBUS_HPP
