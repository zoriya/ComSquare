//
// Created by cbihan on 1/27/20.
//

#include <iostream>
#include <bitset>
#include "PPU.hpp"
#include "PPUUtils.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Ram/Ram.hpp"
#include "../Models/Vector2.hpp"

namespace ComSquare::PPU
{
	PPU::PPU(Renderer::IRenderer &renderer):
		_renderer(renderer),
		vram(new Ram::Ram(65536, ComSquare::VRam, "VRAM")),
		oamram(new Ram::Ram(544, ComSquare::OAMRam, "OAMRAM")),
		cgram(new Ram::Ram(512, ComSquare::CGRam, "CGRAM"))
	{
		this->_registers._isLowByte = true;
		for (int i = 0; i < 512; i++) {
			this->cgram->write_internal(i, random() % 255);
		}
	}

	uint8_t PPU::read(uint24_t addr)
	{
		switch (addr) {
		case ppuRegisters::mpyl:
			return  this->_registers._mpy.mpyl;
		case ppuRegisters::mpym:
			return this->_registers._mpy.mpym;
		case ppuRegisters::mpyh:
			return this->_registers._mpy.mpyh;
		case ppuRegisters::slhv:
			return this->_registers._slhv;
		case ppuRegisters::oamdataread:
		case ppuRegisters::vmdatalread:
		case ppuRegisters::vmdatahread:
		case ppuRegisters::ophct:
		case ppuRegisters::opvct:
		case ppuRegisters::stat77:
		case ppuRegisters::stat78:
			return 0;
		default:
			throw InvalidAddress("PPU Internal Registers read ", addr + this->getStart());
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
			oamram->write_internal(this->_registers._oamadd.oamAddress, this->_registers._oamdata);
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
			//std::cout << "vmdatal" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatal = data;
				this->vram->write_internal(getVramAddress(), data);
			}
			if (!this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case ppuRegisters::vmdatah:
			//std::cout << "vmdatah" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatah = data;
				this->vram->write_internal(getVramAddress(), data);
			}
			if (this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case ppuRegisters::m7sel:
			this->_registers._m7sel.raw = data;
			break;
		case ppuRegisters::m7a:
		case ppuRegisters::m7b:
		case ppuRegisters::m7c:
		case ppuRegisters::m7d:
			this->_registers._m7[addr - ppuRegisters::m7a].m7 = (this->_registers._m7[addr - ppuRegisters::m7a].m7 << 8) | data;
			break;
		case ppuRegisters::m7x:
		case ppuRegisters::m7y:
			// TODO these registers
			break;
		case ppuRegisters::cgadd:
			this->_registers._cgadd = data;
			this->_registers._isLowByte = true;
			break;
		case ppuRegisters::cgdata:
			if (this->_registers._isLowByte) {
				this->_registers._cgdata.cgdatal = data;
			}
			else {
				this->_registers._cgdata.cgdatah = data;
				this->cgram->write_internal(this->_registers._cgadd, this->_registers._cgdata.cgdatal);
				this->_registers._cgadd++;
				this->cgram->write_internal(this->_registers._cgadd, this->_registers._cgdata.cgdatah);
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
			throw InvalidAddress("PPU Internal Registers write", addr + this->getStart());
		}
	}

	uint16_t PPU::getVramAddress()
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
		uint32_t pixelTmp = 0x0;
		if (!this->_registers._inidisp.fblank) {
				for (int y = 0; y <= 255; y += 2) {
					tmp = this->cgram->read_internal(y);
					tmp += this->cgram->read_internal(y + 1) << 8;
					blue = (tmp & 0x7D00U) >> 10U;
					green = (tmp & 0x03E0U) >> 5U;
					red = (tmp & 0x001FU);

					pixelTmp = this->_registers._inidisp.brightness * 255U / 15U;
					pixelTmp += (red * 255U / 31U) << 24U;
					pixelTmp += (green * 255U / 31U) << 16U;
					pixelTmp += (blue * 255U / 31U) << 8U;

					for (int x = 0; x < 100; x++)
						this->_renderer.putPixel(x, y, pixelTmp);
				}
		}
		this->renderBackground(1, {8, 8}, 4, false);
		this->_renderer.drawScreen();
	}

	std::string PPU::getName()
	{
		return "PPU";
	}

	std::string PPU::getValueName(uint24_t addr)
	{
		switch (addr) {
		case ppuRegisters::inidisp:
			return "INIDISP";
		case ppuRegisters::obsel:
			return "OBSEL";
		case ppuRegisters::oamaddl:
			return "OAMADDL";
		case ppuRegisters::oamaddh:
			return "OAMDDH";
		case ppuRegisters::oamdata:
			return "OAMDATA";
		case ppuRegisters::bgmode:
			return "BGMODE";
		case ppuRegisters::mosaic:
			return "MOSAIC";
		case ppuRegisters::bg1sc:
			return "BG1SC";
		case ppuRegisters::bg2sc:
			return "BG2SC";
		case ppuRegisters::bg3sc:
			return "BG3SC";
		case ppuRegisters::bg4sc:
			return "BG4SC";
		case ppuRegisters::bg12nba:
			return "BG12NBA";
		case ppuRegisters::bg34nba:
			return "BG34NBA";
		case ppuRegisters::bg1hofs:
			return "BG1HOFS";
		case ppuRegisters::bg1vofs:
			return "BG1VOFS";
		case ppuRegisters::bg2hofs:
			return "BG2HOFS";
		case ppuRegisters::bg2vofs:
			return "BG2VOFS";
		case ppuRegisters::bg3hofs:
			return "BG3HOFS";
		case ppuRegisters::bg3vofs:
			return "BG3VOFS";
		case ppuRegisters::bg4hofs:
			return "BG4HOFS";
		case ppuRegisters::bg4vofs:
			return "BG4VOFS";
		case ppuRegisters::vmain:
			return "VMAIN";
		case ppuRegisters::vmaddl:
			return "VMADDL";
		case ppuRegisters::vmaddh:
			return "VMADDH";
		case ppuRegisters::vmdatal:
			return "VMDATAL";
		case ppuRegisters::vmdatah:
			return "VMDATAH";
		case ppuRegisters::m7sel:
			return "M7SEL";
		case ppuRegisters ::m7a:
			return "M7A";
		case ppuRegisters ::m7b:
			return "M7B";
		case ppuRegisters ::m7c:
			return "M7C";
		case ppuRegisters ::m7d:
			return "M7D";
		case ppuRegisters ::m7x:
			return "M7X";
		case ppuRegisters ::m7y:
			return "M7Y";
		case ppuRegisters::cgadd:
			return "CGADD";
		case ppuRegisters::cgdata:
			return "CGDATA";
		case ppuRegisters::w12sel:
			return "W12SEL";
		case ppuRegisters::w34sel:
			return "W34SEL";
		case ppuRegisters::wobjsel:
			return "WOBJSEL";
		case ppuRegisters::wh0:
			return "WH0";
		case ppuRegisters::wh1:
			return "WH1";
		case ppuRegisters::wh2:
			return "WH2";
		case ppuRegisters::wh3:
			return "WH3";
		case ppuRegisters::wbjlog:
			return "WBJLOG";
		case ppuRegisters::wobjlog:
			return "WOBJLOG";
		case ppuRegisters::tm:
			return "TM";
		case ppuRegisters::ts:
			return "TS";
		case ppuRegisters::tmw:
			return "TMW";
		case ppuRegisters::tsw:
			return "TSW";
		case ppuRegisters::cgwsel:
			return "CGWSEL";
		case ppuRegisters::cgadsub:
			return "CGADDSUB";
		case ppuRegisters::coldata:
			return "COLDATA";
		case ppuRegisters::setini:
			return "SETINI";
		case ppuRegisters::mpyl:
			return "MPYL";
		case ppuRegisters::mpym:
			return "MPYM";
		case ppuRegisters::mpyh:
			return "MPYH";
		case ppuRegisters::slhv:
			return "SLHV";
		case ppuRegisters::oamdataread:
			return "OAMDATAREAD";
		case ppuRegisters::vmdatalread:
			return "VMDATALREAD";
		case ppuRegisters::vmdatahread:
			return "VMDATAHREAD";
		case ppuRegisters::cgdataread:
			return "CGDATAREAD";
		case ppuRegisters::ophct:
			return "OPHCT";
		case ppuRegisters::opvct:
			return "OPVCT";
		case ppuRegisters::stat77:
			return "STAT77";
		case ppuRegisters::stat78:
			return "STAT78";
		default:
			return "???";
		}
	}

	Component PPU::getComponent()
	{
		return Ppu;
	}

	bool PPU::isDebugger()
	{
		return false;
	}

	uint16_t PPU::cgramRead(uint16_t addr)
	{
		return this->cgram->read_internal(addr);
	}
}