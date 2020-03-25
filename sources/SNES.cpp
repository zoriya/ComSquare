//
// Created by anonymus-raccoon on 1/27/20.
//

#include <ios>
#include <iostream>
#include "SNES.hpp"
#ifdef DEBUGGER_ENABLED
#include "Debugger/CPUDebug.hpp"
#include "Debugger/APUDebug.hpp"
#include "Debugger/MemoryBusDebug.hpp"

#endif

namespace ComSquare
{
	SNES::SNES(const std::string &romPath, Renderer::IRenderer &renderer) :
		_bus(std::make_shared<Memory::MemoryBus>()),
		cartridge(new Cartridge::Cartridge(romPath)),
		wram(new Ram::Ram(16384, WRam, "WRam")),
		sram(new Ram::Ram(this->cartridge->header.sramSize, SRam, "SRam")),
		apuRam(new APU::MemoryMap()),
		cpu(new CPU::CPU(this->_bus, cartridge->header)),
		ppu(new PPU::PPU(renderer)),
		apu(new APU::APU(this->apuRam))
	{
		this->_bus->mapComponents(*this);
	}

	void SNES::update()
	{
		unsigned cycleCount = this->cpu->update();
		this->ppu->update(cycleCount);
		this->apu->update(cycleCount);
	}

	void SNES::enableCPUDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->cpu->isDebugger())
				std::static_pointer_cast<Debugger::CPUDebug>(this->cpu)->focus();
			else
				this->cpu = std::make_shared<Debugger::CPUDebug>(*this->cpu, *this);
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::disableCPUDebugging()
	{
		this->cpu = std::make_shared<CPU::CPU>(*this->cpu);
	}

	void SNES::enableRamViewer()
	{
		#ifdef DEBUGGER_ENABLED
			if (this->_ramViewer)
				this->_ramViewer->focus();
			else
				this->_ramViewer = std::make_unique<Debugger::MemoryViewer>(*this, *this->_bus);
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
			else
				this->apu = std::make_shared<Debugger::APUDebug>(*this->apu, *this);
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}

	void SNES::disableAPUDebugging()
	{
		this->apu = std::make_shared<APU::APU>(*this->apu);
	}

	void SNES::enableMemoryBusDebugging()
	{
	#ifdef DEBUGGER_ENABLED
			if (this->_bus->isDebugger())
				std::static_pointer_cast<Debugger::MemoryBusDebug>(this->_bus)->focus();
			else
			{
				this->_bus = std::make_shared<Debugger::MemoryBusDebug>(*this, *this->_bus);
				this->cpu->setMemoryBus(this->_bus);
			}
	#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
	#endif
	}

	void SNES::disableMemoryBusDebugging()
	{
		#ifdef DEBUGGER_ENABLED
			this->_bus = std::make_shared<Memory::MemoryBus>(*this->_bus);
			this->cpu->setMemoryBus(this->_bus);
		#else
			std::cerr << "Debugging features are not enabled. You can't enable the debugger." << std::endl;
		#endif
	}
}
