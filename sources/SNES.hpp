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

#ifdef DEBUGGER_ENABLED
	#include "Debugger/MemoryViewer.hpp"
	#include "Debugger/HeaderViewer.hpp"
	#include "Debugger/CGramDebug.hpp"
	#include "Debugger/RegisterViewer.hpp"
#include "Exceptions/DebuggableError.hpp"

#endif

namespace ComSquare
{
	//! @brief Container of all the components of the SNES.
	class SNES {
	private:
#ifdef DEBUGGER_ENABLED
		//! @brief The window that allow the user to view a memory.
		std::unique_ptr<Debugger::MemoryViewer> _ramViewer;
		//! @brief The window that allow the user to view the cartridge's header.
		std::unique_ptr<Debugger::HeaderViewer> _headerViewer;
		//! @brief The window that allow the user to view the CGRAM.
		std::unique_ptr<Debugger::CGramDebug> _cgramViewer;
		//! @brief The window that allow the user to view registers.
		std::unique_ptr<Debugger::RegisterViewer> _registerViewer;
#endif
	public:
		//! @brief The memory bus that map addresses to components.
		std::shared_ptr<Memory::MemoryBus> bus;

		//! @brief Cartridge containing instructions (ROM).
		std::shared_ptr<Cartridge::Cartridge> cartridge;
		//! @brief Work Ram shared by all the components.
		std::shared_ptr<Ram::Ram> wram;
		//! @brief Save Ram residing inside the Cartridge in a real SNES.
		std::shared_ptr<Ram::Ram> sram;
		//! @brief External Ram used only by the Audio Processing Unit
		std::shared_ptr<APU::MemoryMap> apuRam;
		//! @brief Central Processing Unit of the SNES.
		std::shared_ptr<CPU::CPU> cpu;
		//! @brief Picture Processing Unit of the SNES
		std::shared_ptr<PPU::PPU> ppu;
		//! @brief Audio Processing Unit if the SNES
		std::shared_ptr<APU::APU> apu;

		//! @brief Call this function to update all the components
		void update();

		//! @brief Disable the CPU's debugging window.
		void disableCPUDebugging();
		//! @brief Enable the CPU's debugging window.
		void enableCPUDebugging(bool pause = false);
		//! @brief Enable the CPU's debugger and show an error message related to an exception.
		//! @param exception The exception to inform the user about.
		void enableCPUDebuggingWithError(const DebuggableError &exception);
		//! @brief Disable the Ram's debugging window.
		void disableRamViewer();
		//! @brief Enable the Ram's debugging window.
		void enableRamViewer();
		//! @brief Disable the Header's debugging window.
		void disableHeaderViewer();
		//! @brief Enable the Header's debugging window.
		void enableHeaderViewer();
		//! @brief Disable the APU's debugging window.
		void disableAPUDebugging();
		//! @brief Enable the APU's debugging window.
		void enableAPUDebugging();
		//! @brief Disable the Memory Bus's debugging window.
		void disableMemoryBusDebugging();
		//! @brief Enable the Memory Bus's debugging window.
		void enableMemoryBusDebugging();
		//! @brief Disable the Cgram's debugging window.
		void disableCgramDebugging();
		//! @brief Enable the Cgram's debugging window.
		void enableCgramDebugging();
		//! @brief Disable the Register's debugging window.
		void disableRegisterDebugging();
		//! @brief Enable the Register's debugging window.
		void enableRegisterDebugging();

		//! @brief Create all the components using a common memory bus for all of them.
		SNES(const std::string &ramPath, Renderer::IRenderer &renderer);
		SNES(const SNES &) = delete;
		SNES &operator=(const SNES &) = delete;
		~SNES() = default;
	};
}

#endif //COMSQUARE_SNES_HPP
