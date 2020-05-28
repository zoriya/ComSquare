//
// Created by cbihan on 5/14/20.
//

#include "PPUUtils.hpp"
#include "PPU.hpp"
#include "Background.hpp"
#include "../Models/Vector2.hpp"

namespace ComSquare::PPU
{
	Background::Background(ComSquare::PPU::PPU &_ppu, int backGroundNumber, bool hasPriority):
		priority(hasPriority),
		bgNumber(backGroundNumber)
	{
		_cgram = _ppu.cgram;
		_vram = _ppu.vram;
		_bpp = _ppu.getBPP(backGroundNumber);
		_characterSize = _ppu.getCharacterSize(backGroundNumber);
		_TileMapStartAddress = _ppu.getTileMapStartAddress(backGroundNumber);
		_tileSetAddress = _ppu.getTileSetAddress(backGroundNumber);
		_tileMaps = _ppu.getBackgroundSize(backGroundNumber);
		_directColor = false;
		_highRes = false;
	}


	void Background::renderBackground(void)
	{
		uint16_t vramAddress = this->_TileMapStartAddress;
		Vector2<int> offset(0, 0);
		this->backgroundSize.x = this->_tileMaps.x * this->_characterSize.x * 32;
		this->backgroundSize.y = this->_tileMaps.y * this->_characterSize.y * 32;

		for (int i = 0; i < 4; i++) {
			if (!(i == 1 && this->_tileMaps.x == 1) && !(i > 1 && this->_tileMaps.y == 1)) {
				drawBasicTileMap(vramAddress, offset);
			}
			vramAddress+= 0x800;
			offset.x += 32 * this->_characterSize.x;
			if (i == 2) {
				offset.x = 0;
				offset.y += 32 * this->_characterSize.y;
			}
		}
	}

	void Background::drawBgTile(uint16_t data, Vector2<int> pos)
	{
		uint16_t graphicAddress;
		union TileMapData tileData;
		std::vector<uint16_t> palette;
		int index = 0;
		uint8_t reference = 0;
		uint32_t color = 0;

		tileData.raw = data;
		palette = getPalette(tileData.palette);
		graphicAddress = this->_tileSetAddress + (tileData.posX * 16 * this->_bpp * 8) + (tileData.posY * this->_bpp * 8);
		for (int i = 0; i < this->_characterSize.y; i++) {
			for (int j = 0; j < this->_characterSize.x; j++) {
				reference = getTilePixelReference(graphicAddress, index);
				color = getRealColor(palette[reference]);
				if (tileData.tilePriority == this->priority && reference != 0) // reference 0 is considered as transparency
					this->buffer[pos.x][pos.y] = color;
				if (j == 7) {
					index = -1;
					// to go to the tile to the right
					graphicAddress += 0x8 * this->_bpp;
				}
				if (index == 7 || this->_bpp == 8) {
					index = -1;
					graphicAddress += 2;
				}
				index++;
				pos.x++;
			}
			graphicAddress -= (0x8 * this->_bpp);
			index = 0;
			pos.x -= this->_characterSize.x;
			pos.y++;
			if (i == 7)
				graphicAddress += 0x100 - 8 * this->_bpp;
		}
	}

	std::vector<uint16_t> Background::getPalette(int nbPalette)
	{
		std::vector<uint16_t> palette(0xF);
		uint16_t addr = nbPalette * 0x10;

		for (int i = 0; i < 0xF; i++) {
			palette[i] = this->_cgram->read_internal(addr);
			palette[i] += this->_cgram->read_internal(addr + 1) << 8U;
			addr += 2;
		}
		return palette;
	}

	uint8_t Background::getTilePixelReference(uint16_t addr, int nb)
	{
		uint8_t highByte = this->_vram->read_internal(addr % VRAMSIZE);
		uint8_t lowByte = this->_vram->read_internal((addr + 1) % VRAMSIZE);
		uint8_t secondHightByte;
		uint8_t secondLowByte;
		uint8_t result = 0;
		// C000

		switch (this->_bpp) {
		case 8:
			return highByte;
		case 4:
			secondHightByte =  this->_vram->read_internal((addr + 32) % VRAMSIZE);
			secondLowByte = this->_vram->read_internal((addr + 33) % VRAMSIZE);
			result = ((secondHightByte & (1U << (7U - nb))) | ((secondLowByte & (1U << (7U - nb))) << 1U)) >> (7U - nb - 2);
		case 2:
			result += ((highByte & (1U << (7U - nb))) | ((lowByte & (1U << (7U - nb))) << 1U)) >> (7U - nb);
		default:
			break;
		}
		return result;
	}

	void Background::drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset)
	{
		uint16_t tileMapValue = 0;
		Vector2<int> pos(0,0);
		uint16_t vramAddress = baseAddress;

		while (vramAddress < 0x800 + baseAddress) {
			tileMapValue = this->_vram->read_internal(vramAddress);
			tileMapValue += this->_vram->read_internal(vramAddress + 1) << 8U;
			drawBgTile(tileMapValue, {(pos.x * this->_characterSize.x) + offset.x, (pos.y * this->_characterSize.y) + offset.y});
			vramAddress += 2;
			if (pos.x % 31 == 0 && pos.x) {
				pos.y++;
				pos.x = 0;
			}
			else
				pos.x++;
		}
	}

	void Background::setTileMapStartAddress(uint16_t address)
	{
		this->_TileMapStartAddress = address;
	}

	void Background::setCharacterSize(Vector2<int> size)
	{
		this->_characterSize = size;
	}
}