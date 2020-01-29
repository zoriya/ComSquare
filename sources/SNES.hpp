//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_SNES_HPP
#define COMSQUARE_SNES_HPP

#include "Memory/MemoryBus.hpp"
#include "CPU/CPU.hpp"
#include "Cartridge/Cartridge.hpp"
#include "Ram/Ram.hpp"
#include "PPU/PPU.hpp"
#include "APU/APU.hpp"

namespace ComSquare
{
	//! @brief Container of all the components of the SNES.
	struct SNES {
	public:
		std::shared_ptr<CPU::CPU> cpu;
		std::shared_ptr<PPU::PPU> ppu;
		std::shared_ptr<APU::APU> apu;
		std::shared_ptr<Cartridge::Cartridge> cartridge;
		std::shared_ptr<Ram::Ram> wram;
		//! @brief Create all the components using a common memory bus for all of them.
		SNES(const std::shared_ptr<Memory::MemoryBus> &bus, const std::string &ramPath);
	};
}

#endif //COMSQUARE_SNES_HPP
