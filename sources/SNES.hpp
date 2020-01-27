//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_SNES_HPP
#define COMSQUARE_SNES_HPP

#include "Memory/MemoryBus.hpp"
#include "CPU/CPU.hpp"
#include "Cartridge/Cartridge.hpp"

namespace ComSquare
{
	//! @brief Container of all the components of the SNES.
	class SNES {
	private:
		CPU::CPU _cpu;
		Cartridge::Cartridge _cartridge;
	public:
		//! @brief Create all the components using a common memory bus for all of them.
		SNES(const std::shared_ptr<MemoryBus> &bus, const std::string &ramPath);
	};
}

#endif //COMSQUARE_SNES_HPP
