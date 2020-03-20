//
// Created by cbihan on 1/27/20.
//

#include <iostream>
#include <bitset>
#include "PPU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::PPU
{
	uint8_t PPU::read(uint24_t addr)
	{
		switch (addr) {
		case 0x34:
			return  this->_registers.mpy.mpyl;
		case 0x35:
			return this->_registers.mpy.mpym;
		case 0x36:
			return this->_registers.mpy.mpyh;
		default:
			throw InvalidAddress("PPU Internal Registers read", addr);
		}
	}

	void PPU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case ppuRegisters::inidisp:
			this->_registers._inidisp.raw = data;
			break;
		case ppuRegisters::obsel:
			this->_registers._obsel.raw = data;
			break;
		case ppuRegisters::oamaddl:
			this->_registers._oamadd.oamaddl = data;
			break;
		case ppuRegisters::oamaddh:
			this->_registers._oamadd.oamaddh = data;
			break;
		case ppuRegisters::oamdata:
			this->_registers._oamdata = data;
			//throw InvalidAddress("oamdata", addr);
			std::cout << "oamdata" << std::endl;
			// the oamAddress have to be calculated if fblank or not (not implemented)
			_oamram.write(this->_registers._oamadd.oamAddress, this->_registers._oamdata);
			this->_registers._oamadd.oamAddress++;
			break;
		case ppuRegisters::bgmode:
			this->_registers._bgmode.raw = data;
			break;
		case ppuRegisters::mosaic:
			this->_registers._mosaic.raw = data;
			break;
		case ppuRegisters::bg1sc:
		case ppuRegisters::bg2sc:
		case ppuRegisters::bg3sc:
		case ppuRegisters::bg4sc:
			this->_registers._bgsc[addr - 0x07].raw = data;
			break;
		case ppuRegisters::bg12nba:
		case ppuRegisters::bg34nba:
			this->_registers._bgnba[addr - 0x0B].raw = data;
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
				this->_registers._m7ofs[addr - ppuRegisters::bg1hofs].raw = data;
			this->_registers._bgofs[addr - ppuRegisters::bg1hofs].raw = data;
			break;
		case ppuRegisters::vmain:
			this->_registers._vmain.raw = data;
			switch (this->_registers._vmain.incrementAmount) {
			case 0b00:
				this->_registers._incrementAmount = 1;
				break;
			case 0b01:
				this->_registers._incrementAmount = 32;
				break;
			case 0b10:
			case 0b11:
				this->_registers._incrementAmount = 128;
			}
			break;
		case ppuRegisters::vmaddl:
			this->_registers._vmadd.vmaddl = data;
			break;
		case ppuRegisters::vmaddh:
			this->_registers._vmadd.vmaddh = data;
			break;
		case ppuRegisters::vmdatal:
			//throw InvalidAddress("vmdata", addr);
			std::cout << "vmdatal" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatal = data;
				this->_vram.write(getVramAddress(), this->_registers._vmdata.vmdata);
			}
			if (!this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case ppuRegisters::vmdatah:
			std::cout << "vmdatah" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatah = data;
				this->_vram.write(getVramAddress(), this->_registers._vmdata.vmdata);
			}
			if (this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case ppuRegisters::m7sel:
			this->_registers._m7sel.raw = data;
			break;
		case ppuRegisters::cgadd:
			std::cout << "cgadd addr : " <<  std::bitset<8>(data)  << std::endl;
			this->_registers._cgadd = data;
			this->_registers._isLowByte = true;
			break;
		case ppuRegisters::cgdata:
			//throw InvalidAddress("cgdata", addr);
			if (this->_registers._isLowByte) {
				std::cout << "cgadatal w data " <<  std::bitset<8>(data) << std::endl;
				this->_registers._cgdata.cgdatal = data;
				//this->_registers._cgram.write(this->_registers._cgadd, this->_registers._cgdata.raw);
				//this->_registers._cgadd++;
			}
			else {
				std::cout << "cgdatah w data " <<  std::bitset<8>(data) << std::endl;
				this->_registers._cgdata.cgdatah = data;
				this->_cgram.write(this->_registers._cgadd, this->_registers._cgdata.raw);
				std::cout << "cgadatah at addr: " << this->_registers._cgadd << " valeur : " <<  std::bitset<16>(this->_registers._cgdata.raw)  << std::endl;
				this->_registers._cgadd++;
			}
			this->_registers._isLowByte = !this->_registers._isLowByte;
			break;
		case ppuRegisters::w12sel:
		case ppuRegisters::w34sel:
		case ppuRegisters::wobjsel:
			this->_registers._wsel[addr - ppuRegisters::w12sel].raw = data;
			break;
		case ppuRegisters::wh0:
			this->_registers._wh0 = data;
			break;
		case ppuRegisters::wh1:
			this->_registers._wh1 = data;
			break;
		case ppuRegisters::wh2:
			this->_registers._wh2 = data;
			break;
		case ppuRegisters::wh3:
			this->_registers._wh3 = data;
			break;
		case ppuRegisters::wbjlog:
			this->_registers._wbglog.raw = data;
			break;
		case ppuRegisters::wobjlog:
			this->_registers._wobjlog.raw = data;
			break;
		case ppuRegisters::tm:
		case ppuRegisters::ts:
			this->_registers._t[addr - ppuRegisters::tm].raw = data;
			break;
		case ppuRegisters::tmw:
		case ppuRegisters::tsw:
			this->_registers._tw[addr - ppuRegisters::tmw].raw = data;
			break;
		case ppuRegisters::cgwsel:
			this->_registers._cgwsel.raw = data;
			break;
		case ppuRegisters::cgadsub:
			this->_registers._cgadsub.raw = data;
			break;
		case ppuRegisters::coldata:
			this->_registers._coldata.raw = data;
			break;
		case ppuRegisters::setini:
			this->_registers._setini.raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	uint8_t PPU::getVramAddress()
	{
		uint16_t vanillaAddress = this->_registers._vmadd.vmadd;

		switch (this->_registers._vmain.addressRemapping) {
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
		uint16_t tmp;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint32_t pixelTmp = 0x000000FF;
		//std::cout << "update" << std::endl;
		if (!this->_registers._inidisp.fblank) {
				for (int y = 0; y <= 255; y++) {
					tmp = this->_cgram.read(y);

					//std::cout << "tmp " << std::bitset<16>(tmp) << std::endl;
					blue = (tmp & 0x7D00U) >> 10U;
					green = (tmp & 0x03E0U) >> 5U;
					red = (tmp & 0x001FU);

					//std::cout << "red " << std::bitset<8>(red) << std::endl;
					//std::cout << "green " << std::bitset<8>(green) << std::endl;
					//std::cout << "blue " << std::bitset<8>(blue) << std::endl;
					pixelTmp += (red * 256U / 32U) << 24U;
					pixelTmp += (green * 256U / 32U) << 16U;
					pixelTmp += (blue * 256U / 32U) << 8U;
					//std::cout << "value of pixel " << std::hex << pixelTmp << " pour inc " << std::dec << y << std::endl;
					for (int x = 0; x < 100; x++)
						this->_renderer.putPixel(x, y, pixelTmp);
					pixelTmp = 0xFF;
				}
		}
		this->_renderer.drawScreen();
	}

	PPU::PPU(const std::shared_ptr<Memory::MemoryBus> &bus, Renderer::IRenderer &renderer):
		_renderer(renderer),
		_bus(std::move(bus)),
		_vram(65536),
		_oamram(544),
		_cgram(512)
	{
		this->_registers._isLowByte = true;
	}
}