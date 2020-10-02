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
		this->backgroundSize.x = this->_tileMaps.x * this->_characterSize.x * NB_CHARACTER_WIDTH;
		this->backgroundSize.y = this->_tileMaps.y * this->_characterSize.y * NB_CHARACTER_HEIGHT;

		for (int i = 0; i < 4; i++) {
			if (!(i == 1 && this->_tileMaps.x == 1) && !(i > 1 && this->_tileMaps.y == 1)) {
				drawBasicTileMap(vramAddress, offset);
			}
			vramAddress += 0x800;
			offset.x += NB_CHARACTER_WIDTH * this->_characterSize.x;
			if (i == 2) {
				offset.x = 0;
				offset.y += NB_CHARACTER_HEIGHT * this->_characterSize.y;
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
		// TODO explain X and Y and rethink the formula
		// X horizontal
		// Y vertical
		graphicAddress = this->_tileSetAddress + (tileData.posY * 16 * this->_bpp * 8) + (tileData.posX * this->_bpp * 8);
		for (int i = 0; i < this->_characterSize.y; i++) {
			for (int j = 0; j < this->_characterSize.x; j++) {
				/*if (index == 14) {
					printf("cc");
				}*/
				reference = getPixelReferenceFromTile(graphicAddress, index);
				color = getRealColor(palette[reference]);
				if (tileData.tilePriority == this->priority) // reference 0 is considered as transparency
					this->buffer[pos.x][pos.y] = (reference) ? color : 0;
			/*	if (j == 7) {
					index = -1;
					// to go to the tile to the right
					graphicAddress += 0x8 * this->_bpp;
				}
				if (index == 7 || this->_bpp == 8) {
					index = -1;
					graphicAddress += 2;
				}*/
				index++;
				pos.x++;
			}
			//graphicAddress -= (0x8 * this->_bpp);
			//index = 0;
			pos.x -= this->_characterSize.x;
			pos.y++;
		//	if (i == 7)
		//		graphicAddress += 0x100 - 8 * this->_bpp;
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

	uint8_t Background::getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex)
	{
		this->getCorrespondingBasicTileRowAndPixelIndex(&tileAddress, &pixelIndex);
		return this->getPixelReferenceFromTileRow(tileAddress, pixelIndex);
	}

	uint8_t Background::getPixelReferenceFromTileRow(uint16_t tileAddress, uint8_t pixelIndex)
	{
		//line by line for each pixel of the line
		uint8_t highByte;
		uint8_t lowByte;
		uint8_t secondHighByte;
		uint8_t secondLowByte;
		uint8_t result = 0;
		uint8_t shift;
		uint8_t rowNumber = pixelIndex / TILE_PIXEL_WIDTH;
		// C000
		//gérer le character size mauvais pour le retour de ligne

		//pixelIndex -= rowNumber * TILE_PIXEL_WIDTH; //get the index relative to the row
		//tileAddress += rowNumber * TILE_PIXEL_WIDTH * this->_bpp; // get row address

		highByte = this->_vram->read_internal(tileAddress % VRAMSIZE);
		lowByte = this->_vram->read_internal((tileAddress + 1) % VRAMSIZE);

		shift = (TILE_PIXEL_WIDTH - 1U - pixelIndex);

		switch (this->_bpp) {
		case 8:
			return highByte;
		case 4:
			secondHighByte =  this->_vram->read_internal((tileAddress + 32) % VRAMSIZE);
			secondLowByte = this->_vram->read_internal((tileAddress + 33) % VRAMSIZE);
			result = ((secondHighByte & (1U << shift)) | ((secondLowByte & (1U << shift)) << 1U)) >> (shift - 2U);
		case 2:
			result += ((highByte & (1U << shift)) | ((lowByte & (1U << shift)) << 1U)) >> shift;
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

		while (vramAddress < baseAddress + 0x800) {
			// TODO function to read 2 bytes (LSB order or bits reversed)
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

	void Background::getCorrespondingBasicTileRowAndPixelIndex(uint16_t *tileAddress, uint8_t *pixelIndex)
	{
		uint16_t tmpTileAddress = *tileAddress;
		uint16_t tmpPixelIndex = *pixelIndex;

		uint8_t row = tmpPixelIndex / this->_characterSize.x;
		uint8_t column = tmpPixelIndex / this->_characterSize.y;

		if (row >= TILE_PIXEL_HEIGHT) {
			tmpTileAddress += 0x80 * this->_bpp;
			row -= TILE_PIXEL_HEIGHT;
		}
		if (column >= TILE_PIXEL_WIDTH) {
			tmpTileAddress += 0x8 * this->_bpp;
			column -= TILE_PIXEL_WIDTH;
		}
		// might not work with 8 bpp must check
		tmpTileAddress += this->_bpp * row;

		*tileAddress = tmpTileAddress;
		*pixelIndex = tmpPixelIndex;
	}
}