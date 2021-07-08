//
// Created by anonymus-raccoon on 1/27/20.
//

#pragma once

#include "APU/APU.hpp"
#include "CPU/CPU.hpp"
#include "Cartridge/Cartridge.hpp"
#include "Exceptions/DebuggableError.hpp"
#include "Memory/MemoryBus.hpp"
#include "PPU/PPU.hpp"
#include "Ram/Ram.hpp"
#include "Renderer/IRenderer.hpp"
#include <optional>

#ifdef DEBUGGER_ENABLED
#include "Debugger/CPU/CPUDebug.hpp"
#include "Debugger/MemoryViewer.hpp"
#include "Debugger/HeaderViewer.hpp"
#include "Debugger/MemoryBusDebug.hpp"
#include "Debugger/APUDebug.hpp"
#include "Debugger/CGramDebug.hpp"
#include "Debugger/RegisterViewer.hpp"
#include "Debugger/TileViewer/TileViewer.hpp"
#endif


namespace ComSquare
{
	//! @brief Container of all the components of the SNES.
	class SNES
	{
	private:
#ifdef DEBUGGER_ENABLED
		//! @brief The CPU's debugger with disassembly, pause, step by step...
		std::optional<Debugger::CPU::CPUDebug> _cpuDebugger;
		//! @brief A debugger that shows every read and write made over the bus.
		std::optional<Debugger::MemoryBusDebug> _busDebugger;
		//! @brief The APU's debugger with disassembly, pause, step by step...
		std::optional<Debugger::APU::APUDebug> _apuDebugger;

		//! @brief The window that allow the user to view a memory.
		std::optional<Debugger::MemoryViewer> _ramViewer;
		//! @brief The window that allow the user to view the cartridge's header.
		std::optional<Debugger::HeaderViewer> _headerViewer;
		//! @brief The window that allow the user to view the CGRAM.
		std::optional<Debugger::CGramDebug> _cgramViewer;
		//! @brief The window that allow the user to view registers.
		std::optional<Debugger::RegisterViewer> _registerViewer;
		//! @brief The window that allow the user to view the CGRAM as tiles.
		std::optional<Debugger::TileViewer> _tileViewer;
#endif
	public:
		//! @brief The memory bus that map addresses to components.
		Memory::MemoryBus bus;

		//! @brief Cartridge containing instructions (ROM).
		Cartridge::Cartridge cartridge;
		//! @brief Work Ram shared by all the components.
		Ram::Ram wram;
		//! @brief Save Ram residing inside the Cartridge in a real SNES.
		Ram::Ram sram;
		//! @brief Central Processing Unit of the SNES.
		CPU::CPU cpu;
		//! @brief Picture Processing Unit of the SNES
		PPU::PPU ppu;
		//! @brief Audio Processing Unit if the SNES
		APU::APU apu;

		//! @brief Create all the components using a common memory bus for all of them.
		//! @param renderer The renderer to use.
		explicit SNES(Renderer::IRenderer &renderer);
		//! @brief Create all the components using a common memory bus for all of them and load a rom
		//! @param ramPath The rom to load.
		//! @param renderer The renderer to use.
		SNES(const std::string &ramPath, Renderer::IRenderer &renderer);
		//! @brief A SNES is not copyable.
		SNES(const SNES &) = delete;
		//! @brief A SNES can't be assigned
		SNES &operator=(const SNES &) = delete;
		//! @brief A default destructor.
		~SNES() = default;

		//! @brief Call this function to update all the components
		void update();

		//! @brief Load the rom at the given path
		//! @param rom The path of the rom.
		//! @throws InvalidRomException If the rom is invalid, this exception is thrown.
		void loadRom(const std::string& path);

#ifdef DEBUGGER_ENABLED
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
		//! @brief Disable the CGRAM's debugging window.
		void disableCgramViewer();
		//! @brief Enable the CGRAM's debugging window.
		void enableCgramViewer();
		//! @brief Disable the Register's debugging window.
		void disableRegisterViewer();
		//! @brief Enable the Register's debugging window.
		void enableRegisterViewer();
		//! @brief Disable the TileViewer's debugging window.
		void disableTileViewer();
		//! @brief Enable the TileViewer's debugging window.
		void enableTileViewer();
#endif
	};
}// namespace ComSquare