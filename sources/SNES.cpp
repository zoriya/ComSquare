//
// Created by anonymus-raccoon on 1/27/20.
//

#include "SNES.hpp"

namespace ComSquare
{
	SNES::SNES(const std::shared_ptr<MemoryBus> &bus, const std::string &romPath)
		: _cpu(bus), _cartridge(romPath)
	{ }
}
