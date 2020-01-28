//
// Created by anonymus-raccoon on 1/24/20.
//

#include "CPU.hpp"
#include "../Exceptions/NotImplementedException.hpp"

namespace ComSquare::CPU
{
	CPU::CPU(std::shared_ptr<ComSquare::MemoryBus> bus)
		: _bus(bus)
	{ }

	uint8_t CPU::read(uint24_t addr)
	{
		(void)addr;
		throw NotImplementedException();
	}

	void CPU::write(uint24_t addr, uint8_t data)
	{
		(void)addr;
		(void)data;
		throw NotImplementedException();
	}

	int CPU::update()
	{
		throw NotImplementedException();
	}

	int CPU::executeInstruction()
	{
		throw NotImplementedException();
	}
}