//
// Created by anonymus-raccoon on 1/27/20.
//

#include <ios>
#include <iostream>
#include "SNES.hpp"
#ifdef DEBUGGER_ENABLED
#include "Debugger/CPU/CPUDebug.hpp"
#include "Debugger/APUDebug.hpp"
#include "Debugger/MemoryBusDebug.hpp"
#include "Debugger/CGramDebug.hpp"
#include "Debugger/TileViewer/TileViewer.hpp"
#endif

namespace ComSquare
{
	SNES::SNES(const std::string &romPath, Renderer::IRenderer &renderer) :
		bus(std::make_shared<Memory::MemoryBus>()),
		cartridge(new Cartridge::Cartridge(romPath)),
		wram(new Ram::Ram(16384, WRam, "WRam")),
		sram(new Ram::Ram(this->cartridge->header.sramSize, SRam, "SRam")),
		cpu(new CPU::CPU(this->bus, cartridge->header)),
		ppu(new PPU::PPU(renderer)),
		apu(new APU::APU(renderer))
	{
		this->bus->mapComponents(*this);
	}

	void SNES::update()
	{
		unsigned cycleCount = this->cpu->update();
		this->ppu->update(cycleCount);
		this->apu->update(cycleCount);
	}

	void SNES::enableCPUDebuggingWithError(const DebuggableError &exception)
	{
		this->enableCPUDebugging(true);
		#ifdef DEBUGGER_ENABLED
			auto cpuDebug = std::static_pointer_cast<Debugger::CPUDebug>(this->cpu);
			cpuDebug->showError(exception);
		#else
			(void)exception;
		#endif
	}

	void SNES::enableCPUDebugging(bool pause)
	{
		#ifdef DEBUGGER_ENABLED
			if (this->cpu->isDebugger()) {
				auto cpuDebug = std::static_pointer_cast<Debugger::CPUDebug>(this->cpu);
				cpuDebug->focus();
				if (pause)
					cpuDebug->pause(true);
			} else {
				this->cpu = std::make_shared<Debugger::CPUDebug>(*this->cpu, *this);
				this->bus->mapComponents(*this);
			}
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
			(void)pause;
		#endif
	}

	void SNES::disableCPUDebugging()
	{
		this->cpu = std::make_shared<CPU::CPU>(*this->cpu);
		this->bus->mapComponents(*this);
	}

	void SNES::enableRamViewer()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_ramViewer)
				this->_ramViewer->focus();
			else
				this->_ramViewer = std::make_unique<Debugger::MemoryViewer>(*this, *this->bus);
		#endif
	}

	void SNES::disableRamViewer()
	{
		#ifdef DEBUGGER_ENABLED
			this->_ramViewer = nullptr;
		#endif
	}

	void SNES::enableHeaderViewer()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_headerViewer)
				this->_headerViewer->focus();
			else
				this->_headerViewer = std::make_unique<Debugger::HeaderViewer>(*this);
		#endif
	}

	void SNES::disableHeaderViewer()
	{
		#ifdef DEBUGGER_ENABLED
			this->_headerViewer = nullptr;
		#endif
	}

	void SNES::enableAPUDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->apu->isDebugger())
				std::static_pointer_cast<Debugger::APUDebug>(this->apu)->focus();
			else {
				this->apu = std::make_shared<Debugger::APUDebug>(*this->apu, *this);
				this->bus->mapComponents(*this);
			}
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::disableAPUDebugging()
	{
		this->apu = std::make_shared<APU::APU>(*this->apu);
		this->bus->mapComponents(*this);
	}

	void SNES::enableMemoryBusDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->bus->isDebugger())
				std::static_pointer_cast<Debugger::MemoryBusDebug>(this->bus)->focus();
			else
			{
				this->bus = std::make_shared<Debugger::MemoryBusDebug>(*this, *this->bus);
				this->cpu->setMemoryBus(this->bus);
			}
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::disableMemoryBusDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->bus = std::make_shared<Memory::MemoryBus>(*this->bus);
			this->cpu->setMemoryBus(this->bus);
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::enableCgramDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_cgramViewer)
				this->_cgramViewer->focus();
			else
				this->_cgramViewer = std::make_unique<Debugger::CGramDebug>(*this, *this->ppu);
		#endif
	}

	void SNES::disableCgramDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->_cgramViewer = nullptr;
		#endif
	}

	void SNES::disableRegisterDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->_registerViewer = nullptr;
		#endif
	}

	void SNES::enableRegisterDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_registerViewer)
				this->_registerViewer->focus();
			else
				this->_registerViewer = std::make_unique<Debugger::RegisterViewer>(*this);
		#endif
	}

	void SNES::disableTileViewerDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->_tileViewer = nullptr;
		#endif
	}

	void SNES::enableTileViewerDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_tileViewer)
				this->_tileViewer->focus();
			else
				this->_tileViewer = std::make_unique<Debugger::TileViewer>(*this, *this->ppu);
		#endif
	}
}
