//
// Created by cbihan on 1/27/20.
//

#include <iostream>
#include <bitset>
#include "PPU.hpp"
#include "Exceptions/NotImplementedException.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include "Ram/Ram.hpp"
#include "Models/Vector2.hpp"

namespace ComSquare::PPU
{
	PPU::PPU(Renderer::IRenderer &renderer):
		vram(new Ram::Ram(VramSize, ComSquare::VRam, "VRAM")),
		oamram(new Ram::Ram(OAMRamSize, ComSquare::OAMRam, "OAMRAM")),
		cgram(new Ram::Ram(CGRamSize, ComSquare::CGRam, "CGRAM")),
		_renderer(renderer),
		_backgrounds{
			Background(*this, 1, false),
			Background(*this, 1, true),
			Background(*this, 2, false),
			Background(*this, 2, true),
			Background(*this, 3, false),
			Background(*this, 3, true),
			Background(*this, 4, false),
			Background(*this, 4, true)
		},
		_mainScreen({{{0}}}),
		_subScreen({{{0}}})
	{
		this->_registers._isLowByte = true;

		//colors for the cgram
		this->cgram->write(2, 0xE0);
		this->cgram->write(3, 0x7F);
		this->cgram->write(4, 0x1F); // 0x1F
		this->cgram->write(6, 0xFF);
		this->cgram->write(7, 0x03);
		this->cgram->write(66, 0xE0);
		this->cgram->write(67, 0x7F);

		//tiles
		int vram_test[] = {
			00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xff,0xff,0xff,0xff,
0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xff,0xff,0xff,0xff,
00,0xc0,0x00,0xe0,0x00,0x70,0x00,0x38,0x00,0x1c,0x00,0x0e,0x00,0x07,0x00,0x03,
00,0x03,0x00,0x07,0x00,0x0e,0x00,0x1c,0x00,0x38,0x00,0x70,0x00,0xe0,0x00,0xc0,
00,0x07,0x00,0x0f,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0xc0,0x00,0xc0,0x00,0xc0,
00,0xe0,0x00,0xf0,0x00,0x18,0x00,0x0c,0x00,0x06,0x00,0x03,0x00,0x03,0x00,0x03,
0xfc,0x00,0xf8,0x00,0xf0,0x00,0xe0,0x00,0xc0,0x00,0x80,0x00,0x00,0x00,0x00,0x00,
0x3f,0x00,0x1f,0x00,0x0f,0x00,0x07,0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
00,0x03,0x00,0x07,0x00,0x0e,0x00,0x1c,0x00,0x38,0x00,0x70,0x00,0xe0,0x00,0xc0,
00,0xc0,0x00,0xe0,0x00,0x70,0x00,0x38,0x00,0x1c,0x00,0x0e,0x00,0x07,0x00,0x03,
00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0x60,0x00,0x30,0x00,0x18,0x00,0x0f,0x00,0x07,
00,0x03,0x00,0x03,0x00,0x03,0x00,0x06,0x00,0x0c,0x00,0x18,0x00,0xf0,0x00,0xe0,
00,0x00,0x00,0x00,0x80,0x00,0xc0,0x00,0xe0,0x00,0xf0,0x00,0xf8,0x00,0xfc,0x00,
00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x0f,00,0x1f,00,0x3f,00, -1
		};
	/*	int *cgram_test = Utils::get_dump_cgram();
		for (int i = 0; cgram_test[i] != -1; i++) {
			this->cgram->write(i, cgram_test[i]);
		} */

		//int *vram_test = Utils::get_dump_vram();
		for (int i = 0; vram_test[i] != -1; i++) {
			this->vram->write(i, vram_test[i]);
		}
		int vram_test_2[] = {8, 00, 02, 00, 0x0A, 00, 02, 00, 0x0A, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_2[i] != -1; i++) {
			this->vram->write(i + 0x8000, vram_test_2[i]);
		}
		int vram_test_3[] = {8, 00, 02, 00, 0x8, 00, 02, 00, 0x8, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_3[i] != -1; i++) {
			this->vram->write(i + 0x8080, vram_test_3[i]);
		}
		int vram_test_4[] = {8, 00, 02, 00, 0x0A, 00, 02, 00, 0x0A, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_4[i] != -1; i++) {
			this->vram->write(i + 0x8100, vram_test_4[i]);
		}
		this->vram->write(0x8040, 04);
		this->vram->write(0x8042, 06);
		this->vram->write(0x8044, 04);
		this->vram->write(0x8046, 06);
		this->vram->write(0x8048, 04);

		this->vram->write(0x80C0, 04);
		this->vram->write(0x80C2, 06);
		this->vram->write(0x80C4, 04);
		this->vram->write(0x80C6, 06);
		this->vram->write(0x80C8, 04);

		this->vram->write(0xC000, 0x0C);

		//registers tic tac toe
		this->_registers._bgmode.bgMode = 0;
		this->_backgrounds[0].setBpp(this->getBPP(1));
		this->_backgrounds[1].setBpp(this->getBPP(1));
		this->_backgrounds[2].setBpp(this->getBPP(2));
		this->_backgrounds[3].setBpp(this->getBPP(2));
		this->_backgrounds[4].setBpp(this->getBPP(3));
		this->_backgrounds[5].setBpp(this->getBPP(3));
		this->_backgrounds[6].setBpp(this->getBPP(4));
		this->_backgrounds[7].setBpp(this->getBPP(4));

		this->_registers._bgmode.characterSizeBg1 = true;
		this->_registers._bgmode.characterSizeBg2 = true;
		this->_backgrounds[0].setCharacterSize(this->getCharacterSize(1));
		this->_backgrounds[1].setCharacterSize(this->getCharacterSize(1));
		this->_backgrounds[2].setCharacterSize(this->getCharacterSize(2));
		this->_backgrounds[3].setCharacterSize(this->getCharacterSize(2));
		this->_backgrounds[4].setCharacterSize(this->getCharacterSize(3));
		this->_backgrounds[5].setCharacterSize(this->getCharacterSize(3));
		this->_backgrounds[6].setCharacterSize(this->getCharacterSize(4));
		this->_backgrounds[7].setCharacterSize(this->getCharacterSize(4));

		this->_registers._bgsc[0].tilemapAddress = 0x4000 >> 10U;
		this->_registers._bgsc[1].tilemapAddress = 0x6000 >> 10U;
		this->_backgrounds[0].setTileMapStartAddress(this->getTileMapStartAddress(1));
		this->_backgrounds[1].setTileMapStartAddress(this->getTileMapStartAddress(1));
		this->_backgrounds[2].setTileMapStartAddress(this->getTileMapStartAddress(2));
		this->_backgrounds[3].setTileMapStartAddress(this->getTileMapStartAddress(2));

		//this->_registers._bgofs[2].raw = 0x03E0;
		//this->_registers._bgofs[3].raw = 0x03DF;
		this->_registers._t[0].enableWindowDisplayBg1 = true;
		this->_registers._t[0].enableWindowDisplayBg2 = true;
/*

		//registers aladin

		this->_registers._bgmode.bgMode = 1;
		this->_backgrounds[0].setBpp(this->getBPP(1));
		this->_backgrounds[1].setBpp(this->getBPP(1));
		this->_backgrounds[2].setBpp(this->getBPP(2));
		this->_backgrounds[3].setBpp(this->getBPP(2));
		this->_backgrounds[4].setBpp(this->getBPP(3));
		this->_backgrounds[5].setBpp(this->getBPP(3));
		//this->_registers._bgmode.characterSizeBg1 = false;
		//this->_registers._bgmode.characterSizeBg2 = false;
		this->_registers._bgmode.mode1Bg3PriorityBit = true;
		this->_backgrounds[0].setCharacterSize(this->getCharacterSize(1));
		this->_backgrounds[1].setCharacterSize(this->getCharacterSize(1));
		this->_backgrounds[2].setCharacterSize(this->getCharacterSize(2));
		this->_backgrounds[3].setCharacterSize(this->getCharacterSize(2));

		this->_registers._bgsc[0].tilemapAddress = 0x4800U >> 10U; // 0x4800
		this->_registers._bgsc[0].tilemapHorizontalMirroring = 1;
		this->_registers._bgsc[1].tilemapAddress = 0x4000U >> 10U; // 0x4000
		this->_registers._bgsc[1].tilemapHorizontalMirroring = 1;
		this->_registers._bgsc[2].tilemapAddress = 0x5C00U >> 10U;
		this->_backgrounds[0].setTileMapStartAddress(this->getTileMapStartAddress(1));
		this->_backgrounds[0].setTilemaps(this->getBackgroundSize(1));
		this->_backgrounds[1].setTileMapStartAddress(this->getTileMapStartAddress(1));
		this->_backgrounds[1].setTilemaps(this->getBackgroundSize(1));
		this->_backgrounds[2].setTileMapStartAddress(this->getTileMapStartAddress(2));
		this->_backgrounds[2].setTilemaps(this->getBackgroundSize(2));
		this->_backgrounds[3].setTileMapStartAddress(this->getTileMapStartAddress(2));
		this->_backgrounds[3].setTilemaps(this->getBackgroundSize(2));
		this->_backgrounds[4].setTileMapStartAddress(this->getTileMapStartAddress(3));
		this->_backgrounds[5].setTileMapStartAddress(this->getTileMapStartAddress(3));

		//registres bgnba
		//this->_registers._bgnba[0].baseAddressBg1a3 = 0x5;
		//this->_registers._bgnba[0].baseAddressBg2a4 = 0x5;
		this->_registers._bgnba[1].baseAddressBg1a3 = 0x5;

		//this->_backgrounds[0].setTilesetAddress(this->getTilesetAddress(1));
		//this->_backgrounds[1].setTilesetAddress(this->getTilesetAddress(1));
		//this->_backgrounds[2].setTilesetAddress(this->getTilesetAddress(2));
		//this->_backgrounds[3].setTilesetAddress(this->getTilesetAddress(2));
		this->_backgrounds[4].setTilesetAddress(this->getTilesetAddress(3));
		this->_backgrounds[5].setTilesetAddress(this->getTilesetAddress(3));

		this->_registers._vmain.incrementMode = true;
		this->_registers._vmain.incrementAmount = 1;

		this->_registers._vmdata.vmdata = 0x1AF0;

		this->_registers._t[0].enableWindowDisplayBg1 = true;
		this->_registers._t[0].enableWindowDisplayBg2 = true;
		this->_registers._t[0].enableWindowDisplayBg3 = true;
*/

	}

	uint8_t PPU::read(uint24_t addr)
	{
		//return 0;
		switch (addr) {
		case PpuRegisters::mpyl:
			return  this->_registers._mpy.mpyl;
		case PpuRegisters::mpym:
			return this->_registers._mpy.mpym;
		case PpuRegisters::mpyh:
			return this->_registers._mpy.mpyh;
		case PpuRegisters::slhv:
			return this->_registers._slhv;
		case PpuRegisters::oamdataread:
			return 0;
		case PpuRegisters::vmdatalread: {
			auto returnValue = static_cast<uint8_t>(this->_vramReadBuffer);
			if (!this->_registers._vmain.incrementMode) {
				this->updateVramReadBuffer();
				// & 0x7FFF;
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			}
			return returnValue;
		}
		case PpuRegisters::vmdatahread: {
			auto returnValue = static_cast<uint8_t>(this->_vramReadBuffer >> 8);
			if (this->_registers._vmain.incrementMode) {
				this->updateVramReadBuffer();
				// & 0x7FFF;
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			}
			return returnValue;
		}
		case PpuRegisters::cgdataread: {
			return this->cgram->read(this->_registers._cgadd++);
		}
		case PpuRegisters::ophct:
		case PpuRegisters::opvct:
		case PpuRegisters::stat77:
		case PpuRegisters::stat78:
			return 0;
		default:
			throw InvalidAddress("PPU Internal Registers read ", addr + this->_start);
 		}
	}

	void PPU::write(uint24_t addr, uint8_t data)
	{
		//return;
		switch (addr) {
		case PpuRegisters::inidisp:
			this->_registers._inidisp.raw = data;
			break;
		case PpuRegisters::obsel:
			this->_registers._obsel.raw = data;
			break;
		case PpuRegisters::oamaddl:
			this->_registers._oamadd.oamaddl = data;
			break;
		case PpuRegisters::oamaddh:
			this->_registers._oamadd.oamaddh = data;
			break;
		case PpuRegisters::oamdata:
			this->_registers._oamdata = data;
			//throw InvalidAddress("oamdata", addr);
			//std::cout << "oamdata" << std::endl;
			// the oamAddress have to be calculated if fblank or not (not implemented)
			oamram->write(this->_registers._oamadd.oamAddress, this->_registers._oamdata);
			this->_registers._oamadd.oamAddress++;
			break;
		case PpuRegisters::bgmode:
			this->_registers._bgmode.raw = data;
			// update backgrounds
			for (int i = 0; i < 8; i++) {
				this->_backgrounds[i].setBpp(this->getBPP((i / 2) + 1));
				this->_backgrounds[i].setCharacterSize(this->getCharacterSize((i / 2) + 1));
			}
			break;
		case PpuRegisters::mosaic:
			this->_registers._mosaic.raw = data;
			break;
		case PpuRegisters::bg1sc:
		case PpuRegisters::bg2sc:
		case PpuRegisters::bg3sc:
		case PpuRegisters::bg4sc:
			this->_registers._bgsc[addr - 0x07].raw = data;
			// update background tilemap address
			this->_backgrounds[addr - 0x07].setTileMapStartAddress(this->getTileMapStartAddress(addr - 0x07 + 1));
			this->_backgrounds[addr - 0x07 + 1].setTileMapStartAddress(this->getTileMapStartAddress(addr - 0x07 + 1));
			this->_backgrounds[addr - 0x07].setTilemaps({this->_registers._bgsc[addr - 0x07].tilemapHorizontalMirroring, this->_registers._bgsc[addr - 0x07].tilemapVerticalMirroring});
			this->_backgrounds[addr - 0x07 + 1].setTilemaps({this->_registers._bgsc[addr - 0x07].tilemapHorizontalMirroring, this->_registers._bgsc[addr - 0x07].tilemapVerticalMirroring});
			break;
		case PpuRegisters::bg12nba:
		case PpuRegisters::bg34nba:
			this->_registers._bgnba[addr - PpuRegisters::bg12nba].raw = data;
			break;
		case PpuRegisters::bg1hofs:
			// TODO need of special var for prev value for Mode 7
			this->_registers._m7ofs[addr - PpuRegisters::bg1hofs].raw = data;
			FALLTHROUGH
		case PpuRegisters::bg2hofs:
		case PpuRegisters::bg3hofs:
		case PpuRegisters::bg4hofs:
			this->_registers._bgofs[addr - PpuRegisters::bg1hofs].raw = ((data << 8) | (this->_ppuState.hvSharedScrollPrevValue & ~7) | (this->_ppuState.hScrollPrevValue & 7)) & 0x3FF;
			this->_ppuState.hScrollPrevValue = data;
			this->_ppuState.hvSharedScrollPrevValue = data;
			break;
		case PpuRegisters::bg1vofs:
			// TODO need of special var for prev value for Mode 7
			this->_registers._bgnba[addr - PpuRegisters::bg12nba].raw = data;
			FALLTHROUGH
		case PpuRegisters::bg2vofs:
		case PpuRegisters::bg3vofs:
		case PpuRegisters::bg4vofs:
			this->_registers._bgofs[addr - PpuRegisters::bg1hofs].raw = ((data << 8) | this->_ppuState.hvSharedScrollPrevValue) & 0x3FF;
			this->_ppuState.hvSharedScrollPrevValue = data;
			break;
		case PpuRegisters::vmain:
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
		case PpuRegisters::vmaddl:
			this->_registers._vmadd.vmaddl = data;
			this->updateVramReadBuffer();
			break;
		case PpuRegisters::vmaddh:
			this->_registers._vmadd.vmaddh = data;
			this->updateVramReadBuffer();
			break;
		case PpuRegisters::vmdatal:
			//throw InvalidAddress("vmdata", addr);
			//std::cout << "vmdatal" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatal = data;
				this->vram->write(this->getVramAddress(), data);
			}
			if (!this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case PpuRegisters::vmdatah:
			//std::cout << "vmdatah" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatah = data;
				this->vram->write(this->getVramAddress() + 1, data);
			}
			if (this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case PpuRegisters::m7sel:
			this->_registers._m7sel.raw = data;
			break;
		case PpuRegisters::m7a:
		case PpuRegisters::m7b:
		case PpuRegisters::m7c:
		case PpuRegisters::m7d:
			this->_registers._m7[addr - PpuRegisters::m7a].m7 = (this->_registers._m7[addr - PpuRegisters::m7a].m7 << 8U) | data;
			break;
		case PpuRegisters::m7x:
		case PpuRegisters::m7y:
			// TODO these registers
			break;
		case PpuRegisters::cgadd:
			this->_registers._cgadd = data;
			this->_registers._isLowByte = true;
			break;
		case PpuRegisters::cgdata:
			if (this->_registers._isLowByte) {
				this->_registers._cgdata.cgdatal = data;
			}
			else {
				this->_registers._cgdata.cgdatah = data;
				this->cgram->write(this->_registers._cgadd, this->_registers._cgdata.cgdatal);
				this->_registers._cgadd++;
				this->cgram->write(this->_registers._cgadd, this->_registers._cgdata.cgdatah);
				this->_registers._cgadd++;
			}
			this->_registers._isLowByte = !this->_registers._isLowByte;
			break;
		case PpuRegisters::w12sel:
		case PpuRegisters::w34sel:
		case PpuRegisters::wobjsel:
			this->_registers._wsel[addr - PpuRegisters::w12sel].raw = data;
			break;
		case PpuRegisters::wh0:
			this->_registers._wh[0] = data;
			break;
		case PpuRegisters::wh1:
			this->_registers._wh[1] = data;
			break;
		case PpuRegisters::wh2:
			this->_registers._wh[2] = data;
			break;
		case PpuRegisters::wh3:
			this->_registers._wh[3] = data;
			break;
		case PpuRegisters::wbjlog:
			this->_registers._wbglog.raw = data;
			break;
		case PpuRegisters::wobjlog:
			this->_registers._wobjlog.raw = data;
			break;
		case PpuRegisters::tm:
		case PpuRegisters::ts:
			this->_registers._t[addr - PpuRegisters::tm].raw = data;
			break;
		case PpuRegisters::tmw:
		case PpuRegisters::tsw:
			this->_registers._tw[addr - PpuRegisters::tmw].raw = data;
			break;
		case PpuRegisters::cgwsel:
			this->_registers._cgwsel.raw = data;
			break;
		case PpuRegisters::cgadsub:
			this->_registers._cgadsub.raw = data;
			break;
		case PpuRegisters::coldata:
			this->_registers._coldata.raw = data;
			break;
		case PpuRegisters::setini:
			this->_registers._setini.raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		case PpuRegisters::stat77: // some roms write here but it is useless
			break;
		default:
			throw InvalidAddress("PPU Internal Registers write", addr + this->_start);
		}
	}

	uint24_t PPU::getSize() const
	{
		return 0x3F;
	}

	uint16_t PPU::getVramAddress() const
	{
		uint16_t vanillaAddress = this->_registers._vmadd.vmadd * 2;

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
		throw InvalidAddress("Invalid vram address", vanillaAddress);
	}

	void PPU::update(unsigned cycles)
	{
		(void)cycles;


		this->renderMainAndSubScreen();
		this->add_buffer(this->_screen, this->_subScreen);
		this->add_buffer(this->_screen, this->_mainScreen);
		//this->_backgrounds[2].renderBackground();
		//add_buffer(this->_screen, this->_backgrounds[2].buffer);
		for (unsigned long i = 0; i < this->_screen.size(); i++) {
			for (unsigned long j = 0; j < this->_screen[i].size(); j++) {
				this->_renderer.putPixel(j, i, this->_screen[i][j]);
			}
		}
		this->_renderer.drawScreen();
		for (auto &i : this->_mainScreen)
			i.fill(0XFF);
		for (auto &i : this->_subScreen)
			i.fill(0XFF);
	}

	std::string PPU::getName() const
	{
		return "PPU";
	}

	std::string PPU::getValueName(uint24_t addr) const
	{
		switch (addr) {
		case PpuRegisters::inidisp:
			return "INIDISP";
		case PpuRegisters::obsel:
			return "OBSEL";
		case PpuRegisters::oamaddl:
			return "OAMADDL";
		case PpuRegisters::oamaddh:
			return "OAMDDH";
		case PpuRegisters::oamdata:
			return "OAMDATA";
		case PpuRegisters::bgmode:
			return "BGMODE";
		case PpuRegisters::mosaic:
			return "MOSAIC";
		case PpuRegisters::bg1sc:
			return "BG1SC";
		case PpuRegisters::bg2sc:
			return "BG2SC";
		case PpuRegisters::bg3sc:
			return "BG3SC";
		case PpuRegisters::bg4sc:
			return "BG4SC";
		case PpuRegisters::bg12nba:
			return "BG12NBA";
		case PpuRegisters::bg34nba:
			return "BG34NBA";
		case PpuRegisters::bg1hofs:
			return "BG1HOFS";
		case PpuRegisters::bg1vofs:
			return "BG1VOFS";
		case PpuRegisters::bg2hofs:
			return "BG2HOFS";
		case PpuRegisters::bg2vofs:
			return "BG2VOFS";
		case PpuRegisters::bg3hofs:
			return "BG3HOFS";
		case PpuRegisters::bg3vofs:
			return "BG3VOFS";
		case PpuRegisters::bg4hofs:
			return "BG4HOFS";
		case PpuRegisters::bg4vofs:
			return "BG4VOFS";
		case PpuRegisters::vmain:
			return "VMAIN";
		case PpuRegisters::vmaddl:
			return "VMADDL";
		case PpuRegisters::vmaddh:
			return "VMADDH";
		case PpuRegisters::vmdatal:
			return "VMDATAL";
		case PpuRegisters::vmdatah:
			return "VMDATAH";
		case PpuRegisters::m7sel:
			return "M7SEL";
		case PpuRegisters ::m7a:
			return "M7A";
		case PpuRegisters ::m7b:
			return "M7B";
		case PpuRegisters ::m7c:
			return "M7C";
		case PpuRegisters ::m7d:
			return "M7D";
		case PpuRegisters ::m7x:
			return "M7X";
		case PpuRegisters ::m7y:
			return "M7Y";
		case PpuRegisters::cgadd:
			return "CGADD";
		case PpuRegisters::cgdata:
			return "CGDATA";
		case PpuRegisters::w12sel:
			return "W12SEL";
		case PpuRegisters::w34sel:
			return "W34SEL";
		case PpuRegisters::wobjsel:
			return "WOBJSEL";
		case PpuRegisters::wh0:
			return "WH0";
		case PpuRegisters::wh1:
			return "WH1";
		case PpuRegisters::wh2:
			return "WH2";
		case PpuRegisters::wh3:
			return "WH3";
		case PpuRegisters::wbjlog:
			return "WBJLOG";
		case PpuRegisters::wobjlog:
			return "WOBJLOG";
		case PpuRegisters::tm:
			return "TM";
		case PpuRegisters::ts:
			return "TS";
		case PpuRegisters::tmw:
			return "TMW";
		case PpuRegisters::tsw:
			return "TSW";
		case PpuRegisters::cgwsel:
			return "CGWSEL";
		case PpuRegisters::cgadsub:
			return "CGADDSUB";
		case PpuRegisters::coldata:
			return "COLDATA";
		case PpuRegisters::setini:
			return "SETINI";
		case PpuRegisters::mpyl:
			return "MPYL";
		case PpuRegisters::mpym:
			return "MPYM";
		case PpuRegisters::mpyh:
			return "MPYH";
		case PpuRegisters::slhv:
			return "SLHV";
		case PpuRegisters::oamdataread:
			return "OAMDATAREAD";
		case PpuRegisters::vmdatalread:
			return "VMDATALREAD";
		case PpuRegisters::vmdatahread:
			return "VMDATAHREAD";
		case PpuRegisters::cgdataread:
			return "CGDATAREAD";
		case PpuRegisters::ophct:
			return "OPHCT";
		case PpuRegisters::opvct:
			return "OPVCT";
		case PpuRegisters::stat77:
			return "STAT77";
		case PpuRegisters::stat78:
			return "STAT78";
		default:
			return "???";
		}
	}

	Component PPU::getComponent() const
	{
		return Ppu;
	}

	bool PPU::isDebugger() const
	{
		return false;
	}

	uint16_t PPU::cgramRead(uint16_t addr)
	{
		return this->cgram->read(addr);
	}

	int PPU::getBPP(int bgNumber) const
	{
		switch (this->_registers._bgmode.bgMode) {
		case 0:
			return 2;
		case 1:
			if (bgNumber < 3)
				return 4;
			return 2;
		case 2:
			return 4;
		case 3:
			if (bgNumber == 1)
				return 8;
			return 4;
		case 4:
			if (bgNumber == 1)
				return 8;
			return 2;
		case 5:
			if (bgNumber == 1)
				return 4;
			return 2;
		case 6:
			return 4;
		case 7:
			if (bgNumber == 1)
				return 8;
			return 7;
		default:
			throw std::runtime_error("Invalid Background number");
		}
	}

	Vector2<int> PPU::getCharacterSize(int bgNumber) const
	{
		Vector2<int> characterSize(8, 8);

		//TODO this wont work for modes 5 and 6 and will be reworked
		if (this->_registers._bgmode.raw & (1U << (3 + bgNumber)))
			characterSize = {16, 16};
		return characterSize;
	}

	uint16_t PPU::getTileMapStartAddress(int bgNumber) const
	{
		return this->_registers._bgsc[bgNumber - 1].tilemapAddress << 11U;
	}

	uint16_t PPU::getTilesetAddress(int bgNumber) const
	{
		uint16_t baseAddress = this->_registers._bgnba[bgNumber > 2].raw;

		baseAddress = (bgNumber % 2) ? baseAddress & 0xFU : (baseAddress & 0xFU) >> 4U;
		baseAddress = baseAddress << 13U;
		return baseAddress;
	}

	Vector2<int> PPU::getBackgroundSize(int bgNumber) const
	{
		Vector2<int> backgroundSize(0,0);

		backgroundSize.y = (this->_registers._bgsc[bgNumber - 1].tilemapVerticalMirroring) ? 2 : 1;
		backgroundSize.x = (this->_registers._bgsc[bgNumber - 1].tilemapHorizontalMirroring) ? 2 : 1;
		return backgroundSize;
	}

	void PPU::renderMainAndSubScreen()
	{
		uint16_t colorPalette;
		// should only render backgrounds needed (depending of th bgMode)
		//int i = 0;
		for (auto &_background : this->_backgrounds) {
			//i++;
			_background.renderBackground();
		}
		// TODO make a function getDefaultBgColor
		colorPalette = this->cgram->read(0);
		colorPalette += this->cgram->read(1) << 8U;

		uint32_t color = Utils::getRealColor(colorPalette);
		for (auto &row : this->_subScreen)
			row.fill(color);
		// the buffer is overwrite if necessary by a new bg so the background priority is from back to front
		// the starting palette index isn't implemented
		switch (this->_registers._bgmode.bgMode) {
		case 0:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg4NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg3NoPriority]);
			//sprites  priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg4Priority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg3Priority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			break;
		case 1:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg3NoPriority]);
			//sprites priority 0
			if (!this->_registers._bgmode.mode1Bg3PriorityBit)
				this->addToMainSubScreen(this->_backgrounds[BgName::bg3Priority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			if (this->_registers._bgmode.mode1Bg3PriorityBit)
				this->addToMainSubScreen(this->_backgrounds[BgName::bg3Priority]);
			break;
		case 2:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			break;
		case 3:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			break;
		case 4:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			break;
		case 5:
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[BgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority 3
			break;
		case 6:
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1NoPriority]);
			//sprites priority 1
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[BgName::bg1Priority]);
			//sprites priority
			break;
		case 7:
			// Not implemented
			throw std::runtime_error("not implemented");
		default:
			break;
		}
	}

	void PPU::addToMainSubScreen(Background &bg)
	{
		if (this->_registers._t[0].raw & (1U << (bg.getBgNumber() - 1U)))
			this->add_buffer(this->_mainScreen, bg.buffer);
		if (this->_registers._t[1].raw & (1U << (bg.getBgNumber() - 1U)))
			this->add_buffer(this->_subScreen, bg.buffer);
	}

	int PPU::getBgMode() const
	{
		return this->_registers._bgmode.bgMode;
	}

	void PPU::updateVramReadBuffer()
	{
		this->_vramReadBuffer = this->vram->read(this->getVramAddress());
		this->_vramReadBuffer += this->vram->read(this->getVramAddress() + 1) << 8;
	}

	Vector2<int> PPU::getBgScroll(int bgNumber) const
	{
		return Vector2<int>(this->_registers._bgofs[(bgNumber - 1) * 2].offsetBg, this->_registers._bgofs[(bgNumber - 1) * 2 + 1].offsetBg);
	}

	const Registers &PPU::getWriteRegisters() const
	{
		return this->_registers;
	}
}