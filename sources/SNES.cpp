//
// Created by anonymus-raccoon on 1/27/20.
//

#include "SNES.hpp"
#include <ios>

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

		unsigned cycleCount = this->cpu.update(0x0C);
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
		this->_cpuDebugger->showError(exception);
	}

	void SNES::enableCPUDebugging(bool pause)
	{
		if (!this->_cpuDebugger.has_value())
			this->_cpuDebugger.emplace(this->cpu, *this);
		else {
			this->_cpuDebugger->focus();
			if (pause)
				this->_cpuDebugger->pause(true);
		}
	}

	void SNES::disableCPUDebugging()
	{
		this->_cpuDebugger = std::nullopt;
	}

	void SNES::enableRamViewer()
	{
		if (this->_ramViewer)
			this->_ramViewer->focus();
		else
			this->_ramViewer.emplace(*this, this->bus);
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
//		if (this->apu->isDebugger())
//			std::static_pointer_cast<Debugger::APUDebug>(this->apu)->focus();
//		else {
//			this->apu = std::make_shared<Debugger::APUDebug>(*this->apu, *this);
//			this->bus.mapComponents(*this);
//		}
	}

	void SNES::disableAPUDebugging()
	{
//		this->apu = std::make_shared<APU::APU>(*this->apu);
//		this->bus.mapComponents(*this);
	}

	void SNES::enableMemoryBusDebugging()
	{
		if (this->_busDebugger)
			this->_busDebugger->focus();
		else
			this->_busDebugger.emplace(*this, this->bus);
		this->cpu.setBus(this->_busDebugger.value());
	}

	void SNES::disableMemoryBusDebugging()
	{
		this->_busDebugger = std::nullopt;
		this->cpu.setBus(this->bus);
	}

	void SNES::enableCgramViewer()
	{
		if (this->_cgramViewer)
			this->_cgramViewer->focus();
		else
			this->_cgramViewer.emplace(*this, this->ppu);
	}

	void SNES::disableCgramViewer()
	{
		this->_cgramViewer = std::nullopt;
	}

	void SNES::disableRegisterViewer()
	{
		this->_registerViewer = std::nullopt;
	}

	void SNES::enableRegisterViewer()
	{
		if (this->_registerViewer)
			this->_registerViewer->focus();
		else
			this->_registerViewer.emplace(*this);
	}

	void SNES::disableTileViewer()
	{
		this->_tileViewer = std::nullopt;
	}

	void SNES::enableTileViewer()
	{
		if (this->_tileViewer)
			this->_tileViewer->focus();
		else
			this->_tileViewer.emplace(*this, this->ppu);
	}

#endif
}// namespace ComSquare
