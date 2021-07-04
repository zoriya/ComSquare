//
// Created by anonymus-raccoon on 1/27/20.
//

#include "SNES.hpp"
#include <ios>
#ifdef DEBUGGER_ENABLED
#include "Debugger/APUDebug.hpp"
#include "Debugger/CPU/CPUDebug.hpp"
#include "Debugger/MemoryBusDebug.hpp"
#include "Debugger/TileViewer/TileViewer.hpp"
#endif

namespace ComSquare
{
	SNES::SNES(Renderer::IRenderer &renderer)
	    : bus(),
	      cartridge(),
	      wram(16384, WRam, "WRam"),
	      sram(this->cartridge.header.sramSize, SRam, "SRam"),
	      cpu(this->bus, cartridge.header),
	      ppu(renderer),
	      apu(renderer)
	{}

	SNES::SNES(const std::string &romPath, Renderer::IRenderer &renderer)
	    : bus(),
	      cartridge(romPath),
	      wram(16384, WRam, "WRam"),
	      sram(this->cartridge.header.sramSize, SRam, "SRam"),
	      cpu(this->bus, cartridge.header),
	      ppu(renderer),
	      apu(renderer)
	{
		this->bus.mapComponents(*this);
		if (this->cartridge.getType() == Cartridge::Audio)
			this->apu.loadFromSPC(this->cartridge);
	}

	void SNES::update()
	{
		if (this->cartridge.getType() == Cartridge::Audio) {
			this->apu.update(0x01);
			return;
		}

		unsigned cycleCount = this->cpu.update();
		this->ppu.update(cycleCount);
		this->apu.update(cycleCount);
	}

	void SNES::loadRom(const std::string &path)
	{
		this->cartridge.loadRom(path);
		this->bus.mapComponents(*this);
		if (this->cartridge.getType() == Cartridge::Audio)
			this->apu.loadFromSPC(this->cartridge);
	}

#ifdef DEBUGGER_ENABLED

	void SNES::enableCPUDebuggingWithError(const DebuggableError &exception)
	{
		this->enableCPUDebugging(true);
		auto cpuDebug = std::static_pointer_cast<Debugger::CPUDebug>(this->cpu);
		cpuDebug->showError(exception);
	}

	void SNES::enableCPUDebugging(bool pause)
	{
		if (this->cpu->isDebugger()) {
			auto cpuDebug = std::static_pointer_cast<Debugger::CPUDebug>(this->cpu);
			cpuDebug->focus();
			if (pause)
				cpuDebug->pause(true);
		} else {
			this->cpu = std::make_shared<Debugger::CPUDebug>(*this->cpu, *this);
			this->bus.mapComponents(*this);
		}
	}

	void SNES::disableCPUDebugging()
	{
		this->cpu = std::make_shared<CPU::CPU>(*this->cpu);
		this->bus.mapComponents(*this);
	}

	void SNES::enableRamViewer()
	{
		if (this->_ramViewer)
			this->_ramViewer->focus();
		else
			this->_ramViewer.emplace(*this, *this->bus);
	}

	void SNES::disableRamViewer()
	{
		this->_ramViewer = std::nullopt;
	}

	void SNES::enableHeaderViewer()
	{
		if (this->_headerViewer)
			this->_headerViewer->focus();
		else
			this->_headerViewer.emplace(*this);
	}

	void SNES::disableHeaderViewer()
	{
		this->_headerViewer = std::nullopt;
	}

	void SNES::enableAPUDebugging()
	{
		if (this->apu->isDebugger())
			std::static_pointer_cast<Debugger::APUDebug>(this->apu)->focus();
		else {
			this->apu = std::make_shared<Debugger::APUDebug>(*this->apu, *this);
			this->bus.mapComponents(*this);
		}
	}

	void SNES::disableAPUDebugging()
	{
		this->apu = std::make_shared<APU::APU>(*this->apu);
		this->bus.mapComponents(*this);
	}

	void SNES::enableMemoryBusDebugging()
	{
		if (this->bus.isDebugger())
			std::static_pointer_cast<Debugger::MemoryBusDebug>(this->bus)->focus();
		else {
			this->bus = std::make_shared<Debugger::MemoryBusDebug>(*this, *this->bus);
			this->cpu->setMemoryBus(this->bus);
		}
	}

	void SNES::disableMemoryBusDebugging()
	{
		this->bus = std::make_shared<Memory::MemoryBus>(*this->bus);
		this->cpu->setMemoryBus(this->bus);
	}

	void SNES::enableCgramDebugging()
	{
		if (this->_cgramViewer)
			this->_cgramViewer->focus();
		else
			this->_cgramViewer.emplace(*this, *this->ppu);
	}

	void SNES::disableCgramDebugging()
	{
		this->_cgramViewer = std::nullopt;
	}

	void SNES::disableRegisterDebugging()
	{
		this->_registerViewer = std::nullopt;
	}

	void SNES::enableRegisterDebugging()
	{
		if (this->_registerViewer)
			this->_registerViewer->focus();
		else
			this->_registerViewer.emplace(*this);
	}

	void SNES::disableTileViewerDebugging()
	{
		this->_tileViewer = std::nullopt;
	}

	void SNES::enableTileViewerDebugging()
	{
		if (this->_tileViewer)
			this->_tileViewer->focus();
		else
			this->_tileViewer.emplace(*this, *this->ppu);
	}

#endif
}// namespace ComSquare
