//
// Created by anonymus-raccoon on 1/24/20.
//

#include "Cpu.hpp"

namespace ComSquare::CPU
{
	CPU::CPU(std::shared_ptr<ComSquare::MemoryBus> bus)
		: _bus(bus)
	{ }
}