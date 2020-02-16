//
// Created by cbihan on 1/27/20.
//

#include <iostream>
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
			this->_oamdata = data;
			throw InvalidAddress("oamdata", addr);
			std::cout << "oamdata" << std::endl;
			// the oamAddress have to be calculated if fblank or not (not implemented)
			_oamram.write(this->_oamadd.oamAddress, this->_oamdata);
			this->_oamadd.oamAddress++;
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
		case ppuRegisters::bg1vofs:
		case ppuRegisters::bg2hofs:
		case ppuRegisters::bg2vofs:
		case ppuRegisters::bg3hofs:
		case ppuRegisters::bg3vofs:
		case ppuRegisters::bg4hofs:
		case ppuRegisters::bg4vofs:
			// Work in progress !
			if (addr == ppuRegisters::bg1hofs || addr == ppuRegisters::bg1vofs)
				this->_m7ofs[addr - ppuRegisters::bg1hofs].raw = data;
			this->_bgofs[addr - ppuRegisters::bg1hofs].raw = data;
			break;
		case ppuRegisters::vmain:
			this->_vmain.raw = data;
			switch (this->_vmain.incrementAmount) {
			case 0b00:
				this->_incrementAmount = 1;
				break;
			case 0b01:
				this->_incrementAmount = 32;
				break;
			case 0b10:
			case 0b11:
				this->_incrementAmount = 128;
			}
		case ppuRegisters::vmaddl:
			this->_vmadd.vmaddl = data;
			break;
		case ppuRegisters::vmaddh:
			this->_vmadd.vmaddh = data;
			break;
		case ppuRegisters::vmdatal:
			throw InvalidAddress("vmdata", addr);
			std::cout << "vmdatal" << std::endl;
			if (!this->_inidisp.fblank) {
				this->_vmdata.vmdatal = data;
				this->_vram.write(getVramAddress(), this->_vmdata.vmdata);
			}
			if (!this->_vmain.incrementMode)
				this->_vmadd.vmadd += this->_incrementAmount;
			break;
		case ppuRegisters::vmdatah:
			std::cout << "vmdatah" << std::endl;
			if (!this->_inidisp.fblank) {
				this->_vmdata.vmdatah = data;
				this->_vram.write(getVramAddress(), this->_vmdata.vmdata);
			}
			if (this->_vmain.incrementMode)
				this->_vmadd.vmadd += this->_incrementAmount;
			break;
		case ppuRegisters::m7sel:
			this->_m7sel.raw = data;
			break;
		case ppuRegisters::cgadd:
			this->_cgadd = data;
			this->_isLowByte = true;
			break;
		case ppuRegisters::cgdata:
			throw InvalidAddress("cgdata", addr);
			if (this->_isLowByte) {
				std::cout << "cgadatal" << std::endl;
				this->_cgdata.cgdatal = data;
				this->_cgram.write(this->_cgadd, this->_cgdata.raw);
			}
			else {
				std::cout << "cgadatah" << std::endl;
				this->_cgdata.cgdatah = data;
				this->_cgram.write(this->_cgadd, this->_cgdata.raw);
			}
			this->_isLowByte = !this->_isLowByte;
			this->_cgadd++;
			break;
		case ppuRegisters::w12sel:
		case ppuRegisters::w34sel:
		case ppuRegisters::wobjsel:
			this->_wsel[addr - ppuRegisters::w12sel].raw = data;
			break;
		case ppuRegisters::wh0:
			this->_wh0 = data;
			break;
		case ppuRegisters::wh1:
			this->_wh1 = data;
			break;
		case ppuRegisters::wh2:
			this->_wh2 = data;
			break;
		case ppuRegisters::wh3:
			this->_wh3 = data;
			break;
		case ppuRegisters::wbjlog:
			this->_wbglog.raw = data;
			break;
		case ppuRegisters::wobjlog:
			this->_wobjlog.raw = data;
			break;
		case ppuRegisters::tm:
		case ppuRegisters::ts:
			this->_t[addr - ppuRegisters::tm].raw = data;
			break;
		case ppuRegisters::tmw:
		case ppuRegisters::tsw:
			this->_tw[addr - ppuRegisters::tmw].raw = data;
			break;
		case ppuRegisters::cgwsel:
			this->_cgwsel.raw = data;
			break;
		case ppuRegisters::cgadsub:
			this->_cgadsub.raw = data;
			break;
		case ppuRegisters::coldata:
			this->_coldata.raw = data;
			break;
		case ppuRegisters::setini:
			this->_setini.raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	uint8_t PPU::getVramAddress()
	{
		uint16_t vanillaAddress = this->_vmadd.vmadd;

		switch (this->_vmain.addressRemapping) {
		case 0b00:
			return vanillaAddress;
		case 0b01:
			return (vanillaAddress & 0xFF00U) | (vanillaAddress & 0x00E0U) >> 5U | (vanillaAddress & 0x001FU) << 3U;
		case 0b10:
			return (vanillaAddress & 0xFE00U) | (vanillaAddress & 0x01C0U) >> 6U | (vanillaAddress & 0x3FU) << 3U;
		case 0b11:
			return (vanillaAddress & 0xFC00U) | (vanillaAddress & 0x0380U) >> 7U | (vanillaAddress & 0x7FU) << 3U;
		}
	}

	void PPU::update(unsigned cycles)
	{
		(void)cycles;
		int inc = 0;
		uint32_t pixelTmp = 0xFFFFFFFF;
		pixelTmp |= this->_inidisp.brightness;
		std::cout << "update" << std::endl;
		if (!this->_inidisp.fblank) {
			for (int x = 0; x < 448; x++) {
				for (int y = 0; y < 512; y++) {
					if (inc == 0xFA00)
						inc = 0;
					//std::cout << "holy" << std::endl;
					this->_renderer.putPixel(x, y, (uint32_t)this->_vram.read(inc++));
				}
			}
		}
		//std::cout << "cgadata2" << std::endl;
		this->_renderer.drawScreen();
	}

	PPU::PPU(const std::shared_ptr<Memory::MemoryBus> &bus, Renderer::IRenderer &renderer):
		_renderer(renderer),
		_bus(std::move(bus)),
		_vram(64000),
		_oamram(544),
		_cgram(512)
	{
		this->_isLowByte = true;
	}
}