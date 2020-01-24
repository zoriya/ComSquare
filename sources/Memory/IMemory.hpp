//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_IMEMORY_HPP
#define COMSQUARE_IMEMORY_HPP


#include <cstdint>
#include <vector>

namespace ComSquare
{
	class IMemory {
	private:
		uint32_t _start = 0;
		uint32_t _end = 0;
	public:
		virtual uint8_t read(uint32_t addr) = 0;
		virtual void write(uint32_t addr, uint8_t data) = 0;
		void setMemoryRegion(uint32_t start, uint32_t end);
		bool hasMemoryAt(uint32_t addr);
		uint32_t getStart();
	};
};


#endif //COMSQUARE_IMEMORY_HPP
