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
#include "Renderer/IRenderer.hpp"

namespace ComSquare
{
	//! @brief Container of all the components of the SNES.
	class SNES {
	public:
		//! @brief Cartridge containing instructions (ROM).
		std::shared_ptr<Cartridge::Cartridge> cartridge;
		//! @brief Central Processing Unit of the SNES.
		std::shared_ptr<CPU::CPU> cpu;
		//! @brief Picture Processing Unit of the SNES
		std::shared_ptr<PPU::PPU> ppu;
		//! @brief Audio Processing Unit if the SNES
		std::shared_ptr<APU::APU> apu;
		//! @brief Work Ram shared by all the components.
		std::shared_ptr<Ram::Ram> wram;
		//! @brief Save Ram residing inside the Cartridge in a real SNES.
		std::shared_ptr<Ram::Ram> sram;

		//! @brief Call this function to update all the components
		void update();

		//! @brief Disable the CPU's debugging window.
		void disableCPUDebugging();
		//! @brief Enable the CPU's debugging window.
		void enableCPUDebugging();

		//! @brief Create all the components using a common memory bus for all of them.
		SNES(const std::shared_ptr<Memory::MemoryBus> &bus, const std::string &ramPath, Renderer::IRenderer &renderer);
		SNES(const SNES &) = default;
		SNES &operator=(const SNES &) = default;
		~SNES() = default;
	};
}

#endif //COMSQUARE_SNES_HPP
