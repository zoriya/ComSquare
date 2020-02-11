//
// Created by cbihan on 1/27/20.
//

#include "PPU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::PPU
{
	uint8_t PPU::read(uint24_t addr)
	{
		switch (addr) {
		case 0x34:
			return this->mpy.mpyl;
		case 0x35:
			return this->mpy.mpym;
		case 0x36:
			return this->mpy.mpyh;
		default:
			throw InvalidAddress("PPU Internal Registers read", addr);
		}
	}

	void PPU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case ppuRegisters::inidisp:
			this->_inidisp.raw = data;
			break;
		case ppuRegisters::obsel:
			this->_obsel.raw = data;
			break;
		case ppuRegisters::oamaddl:
			this->_oamadd.oamaddl = data;
			break;
		case ppuRegisters::oamaddh:
			this->_oamadd.oamaddh = data;
			break;
		case ppuRegisters::oamdata:
			//! @brief not implemented yet.
			this->_oamdata = data;
			break;
		case ppuRegisters::bgmode:
			this->_bgmode.raw = data;
			break;
		case ppuRegisters::mosaic:
			this->_mosaic.raw = data;
			break;
		case ppuRegisters::bg1sc:
		case ppuRegisters::bg2sc:
		case ppuRegisters::bg3sc:
		case ppuRegisters::bg4sc:
			this->_bgsc[addr - 0x07].raw = data;
			break;
		case ppuRegisters::bg12nba:
		case ppuRegisters::bg34nba:
			this->_bgnba[addr - 0x0B].raw = data;
			break;
		case ppuRegisters::bg1hofs:
			// Work in progress ! Non functional !
			this->_bgofs[0].raw = data;
			break;
		case ppuRegisters::vmain:
			this->_vmain.raw = data;
			break;
		case ppuRegisters::vmaddl:
			this->_vmadd.vmaddl = data;
			break;
		case ppuRegisters::vmaddh:
			this->_vmadd.vmaddh = data;
			break;
		//! @info should must be in vblank for the write (and write it to the screen )? and increment vram address after;
		case ppuRegisters::vmdatal:
			this->_vmdata.vmdatal = data;
			break;
		case ppuRegisters::vmdatah:
			this->_vmdata.vmdatah = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	void PPU::update(unsigned cycles)
	{
		(void)cycles;
		uint32_t pixelTmp = 0xFFFFFF00;
		pixelTmp |= this->_inidisp.brightness;
		if (!this->_inidisp.fblank)
			this->_renderer.putPixel(0, 0,pixelTmp);
		this->_renderer.drawScreen();
	}

	PPU::PPU(const std::shared_ptr<Memory::MemoryBus> &bus, Renderer::IRenderer &renderer):
		_bus(std::move(bus)),
		_renderer(renderer)
	{
	}
}