//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_MEMORYBUS_HPP
#define COMSQUARE_MEMORYBUS_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include "IMemory.hpp"

namespace ComSquare
{
	class MemoryBus {
	private:
		std::vector<std::shared_ptr<IMemory>> _memoryAccessors;
		std::shared_ptr<IMemory> getAccessor(uint24_t addr);
		uint8_t _openbus;
	public:
		uint8_t read(uint24_t addr);
		void write(uint24_t addr, uint8_t data);
	};
}


#endif //COMSQUARE_MEMORYBUS_HPP
