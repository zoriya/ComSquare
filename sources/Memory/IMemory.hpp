//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_IMEMORY_HPP
#define COMSQUARE_IMEMORY_HPP


#include <cstdint>
#include <vector>
#include "../Models/Ints.hpp"

namespace ComSquare
{
	class IMemory {
	private:
		uint24_t _start = 0;
		uint24_t _end = 0;
	public:
		virtual uint8_t read(uint24_t addr) = 0;
		virtual void write(uint24_t addr, uint8_t data) = 0;
		void setMemoryRegion(uint24_t start, uint24_t end);
		bool hasMemoryAt(uint24_t addr);
		uint24_t getStart();
	};
};


#endif //COMSQUARE_IMEMORY_HPP
