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
#include <random>

namespace ComSquare::PPU
{
	PPU::PPU(Renderer::IRenderer &renderer):
		vram(new Ram::Ram(VRAMSIZE, ComSquare::VRam, "VRAM")),
		oamram(new Ram::Ram(OAMRAMSIZE, ComSquare::OAMRam, "OAMRAM")),
		cgram(new Ram::Ram(CGRAMSIZE, ComSquare::CGRam, "CGRAM")),
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
		}
	{
		this->_registers._isLowByte = true;
		/*for (int i = 0; i < 512; i++) {
			this->cgram->write_internal(i, random() % 255);
		}*/

		//colors for the cgram
	/*	this->cgram->write_internal(2, 0xE0);
		this->cgram->write_internal(3, 0x7F);
		this->cgram->write_internal(4, 0x1F); // 0x1F
		this->cgram->write_internal(6, 0xFF);
		this->cgram->write_internal(7, 0x03);
		this->cgram->write_internal(66, 0xE0);
		this->cgram->write_internal(67, 0x7F);*/

		//tiles
		/*int vram_test[] = {
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
		};*/
		int *cgram_test = get_dump_cgram();
		for (int i = 0; cgram_test[i] != -1; i++) {
			this->cgram->write_internal(i, cgram_test[i]);
		}

		int *vram_test = get_dump_vram();
		for (int i = 0; vram_test[i] != -1; i++) {
			this->vram->write_internal(i, vram_test[i]);
		}
		/*int vram_test_2[] = {8, 00, 02, 00, 0x0A, 00, 02, 00, 0x0A, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_2[i] != -1; i++) {
			this->vram->write_internal(i + 0x8000, vram_test_2[i]);
		}
		int vram_test_3[] = {8, 00, 02, 00, 0x8, 00, 02, 00, 0x8, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_3[i] != -1; i++) {
			this->vram->write_internal(i + 0x8080, vram_test_3[i]);
		}
		int vram_test_4[] = {8, 00, 02, 00, 0x0A, 00, 02, 00, 0x0A, 00, 00, 00, 00, 00, 00, -1};
		for (int i = 0; vram_test_4[i] != -1; i++) {
			this->vram->write_internal(i + 0x8100, vram_test_4[i]);
		}
		this->vram->write_internal(0x8040, 04);
		this->vram->write_internal(0x8042, 06);
		this->vram->write_internal(0x8044, 04);
		this->vram->write_internal(0x8046, 06);
		this->vram->write_internal(0x8048, 04);

		this->vram->write_internal(0x80C0, 04);
		this->vram->write_internal(0x80C2, 06);
		this->vram->write_internal(0x80C4, 04);
		this->vram->write_internal(0x80C6, 06);
		this->vram->write_internal(0x80C8, 04);

		this->vram->write_internal(0xC000, 0x0C);*/

		//registers

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


	}

	uint8_t PPU::read(uint24_t addr)
	{
		return 0;
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
			return 0;
		case ppuRegisters::cgdataread: {
			return this->cgram->read_internal(this->_registers._cgadd++);
		}
		case ppuRegisters::ophct:
		case ppuRegisters::opvct:
		case ppuRegisters::stat77:
		case ppuRegisters::stat78:
			return 0;
		default:
			throw InvalidAddress("PPU Internal Registers read ", addr);
 		}
	}

	void PPU::write(uint24_t addr, uint8_t data)
	{
		return;
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
			// update backgrounds
			for (int i = 0; i < 8; i++) {
				this->_backgrounds[i].setBpp(this->getBPP((i / 2) + 1));
				this->_backgrounds[i].setCharacterSize(this->getCharacterSize((i / 2) + 1));
			}
			break;
		case ppuRegisters::mosaic:
			this->_registers._mosaic.raw = data;
			break;
		case ppuRegisters::bg1sc:
		case ppuRegisters::bg2sc:
		case ppuRegisters::bg3sc:
		case ppuRegisters::bg4sc:
			this->_registers._bgsc[addr - 0x07].raw = data;
			// update background tilemap address
			this->_backgrounds[addr - 0x07].setTileMapStartAddress(this->getTileMapStartAddress(addr - 0x07 + 1));
			this->_backgrounds[addr - 0x07 + 1].setTileMapStartAddress(this->getTileMapStartAddress(addr - 0x07 + 1));
			this->_backgrounds[addr - 0x07].setTilemaps({this->_registers._bgsc[addr - 0x07].tilemapHorizontalMirroring, this->_registers._bgsc[addr - 0x07].tilemapVerticalMirroring});
			this->_backgrounds[addr - 0x07 + 1].setTilemaps({this->_registers._bgsc[addr - 0x07].tilemapHorizontalMirroring, this->_registers._bgsc[addr - 0x07].tilemapVerticalMirroring});
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
				this->vram->write_internal(getVramAddress(), this->_registers._vmdata.vmdatal);
			}
			if (!this->_registers._vmain.incrementMode)
				this->_registers._vmadd.vmadd += this->_registers._incrementAmount;
			break;
		case ppuRegisters::vmdatah:
			//std::cout << "vmdatah" << std::endl;
			if (!this->_registers._inidisp.fblank) {
				this->_registers._vmdata.vmdatah = data;
				this->vram->write_internal(getVramAddress(), this->_registers._vmdata.vmdatah);
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
			this->_registers._m7[addr - ppuRegisters::m7a].m7 = (this->_registers._m7[addr - ppuRegisters::m7a].m7 << 8U) | data;
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
		case ppuRegisters::stat77: // some roms write here but it is useless
			break;
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
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

	int PPU::getBPP(int bgNumber)
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
			return -1;
		}
	}

	Vector2<int> PPU::getCharacterSize(int bgNumber)
	{
		Vector2<int> characterSize(8, 8);

		//this wont work for modes 5 and 6 and will be reworked
		if (this->_registers._bgmode.raw & (1U << (3 + bgNumber)))
			characterSize = {16, 16};
		return characterSize;
	}

	uint16_t PPU::getTileMapStartAddress(int bgNumber)
	{
		return this->_registers._bgsc[bgNumber - 1].tilemapAddress << 11U;
	}

	uint16_t PPU::getTilesetAddress(int bgNumber)
	{
		uint16_t baseAddress = this->_registers._bgnba[bgNumber > 2].raw;

		baseAddress = (bgNumber % 2) ? baseAddress & 0xFU : (baseAddress & 0xFU) >> 4U;
		baseAddress = baseAddress << 13U;
		return baseAddress;
	}

	Vector2<int> PPU::getBackgroundSize(int bgNumber)
	{
		Vector2<int> backgroundSize(0,0);

		backgroundSize.y = (this->_registers._bgsc[bgNumber - 1].tilemapVerticalMirroring) ? 2 : 1;
		backgroundSize.x = (this->_registers._bgsc[bgNumber - 1].tilemapHorizontalMirroring) ? 2 : 1;
		return backgroundSize;
	}

	void PPU::renderMainAndSubScreen(void)
	{
		uint16_t colorPalette;
		// should only render backgrounds needed (depending of th bgMode)
		for (auto & _background : this->_backgrounds)
			_background.renderBackground();
		// TODO make a function getDefaultBgColor
		colorPalette = this->cgram->read_internal(0);
		colorPalette += this->cgram->read_internal(1) << 8U;

		for (unsigned long i = 0; i < this->_subScreen.size(); i++)
			for (unsigned long j = 0; j < this->_subScreen[i].size(); j++)
				this->_subScreen[i][j] = getRealColor(colorPalette);
		// the buffer is overwrite if necessary by a new bg so the background priority is from back to front
		// the starting palette index isn't implemented
		switch (this->_registers._bgmode.bgMode) {
		case 0:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg4NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg3NoPriority]);
			//sprites  priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg4Priority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg3Priority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			break;
		case 1:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg3NoPriority]);
			//sprites priority 0
			if (!this->_registers._bgmode.mode1Bg3PriorityBit)
				this->addToMainSubScreen(this->_backgrounds[bgName::bg3Priority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			if (this->_registers._bgmode.mode1Bg3PriorityBit)
				this->addToMainSubScreen(this->_backgrounds[bgName::bg3Priority]);
			break;
		case 2:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			break;
		case 3:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			break;
		case 4:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			break;
		case 5:
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2NoPriority]);
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 1
			this->addToMainSubScreen(this->_backgrounds[bgName::bg2Priority]);
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
			break;
		case 6:
			//sprites priority 0
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1NoPriority]);
			//sprites priority 1
			//sprites priority 2
			this->addToMainSubScreen(this->_backgrounds[bgName::bg1Priority]);
			//sprites priority 3
            break;
		case 7:
			// Not implemented
		default:
			break;
		}
	}

	template <std::size_t DEST_SIZE, std::size_t SRC_SIZE>
	void PPU::add_buffer(std::array<std::array<uint32_t, DEST_SIZE>, DEST_SIZE> &bufferDest, std::array<std::array<uint32_t, SRC_SIZE>, SRC_SIZE> &bufferSrc)
	{
		for (unsigned long i = 0; i < bufferSrc.size(); i++) {
			for (unsigned long j = 0; j < bufferSrc[i].size(); j++) {
				if (bufferSrc[i][j] > 0xFF) // 0xFF correspond to a black pixel with full brightness
					bufferDest[i][j] = bufferSrc[i][j];
			}
		}
	}

	void PPU::addToMainSubScreen(Background &bg)
	{
		if (this->_registers._t[0].raw & (1U << (bg.bgNumber - 1U)))
			this->add_buffer(this->_mainScreen, bg.buffer);
		if (this->_registers._t[1].raw & (1U << (bg.bgNumber - 1U)))
			this->add_buffer(this->_subScreen, bg.buffer);
	}
}