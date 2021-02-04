//
// Created by cbihan on 5/14/20.
//

#include "PPUUtils.hpp"
#include "PPU.hpp"
#include "Background.hpp"
#include <cmath>
#include "../Models/Vector2.hpp"

namespace ComSquare::PPU
{
	Background::Background(ComSquare::PPU::PPU &ppu, int backGroundNumber, bool hasPriority):
		_priority(hasPriority),
		_bgNumber(backGroundNumber),
		_ppu(ppu)
	{
		_cgram = ppu.cgram;
		_vram = ppu.vram;
		_bpp = ppu.getBPP(backGroundNumber);
		_characterSize = ppu.getCharacterSize(backGroundNumber);
		_tileMapStartAddress = ppu.getTileMapStartAddress(backGroundNumber);
		_tilesetAddress = ppu.getTilesetAddress(backGroundNumber);
		_tileMaps = ppu.getBackgroundSize(backGroundNumber);
		_directColor = false;
		_highRes = false;
	}


	void Background::renderBackground()
	{
		uint16_t vramAddress = this->_tileMapStartAddress;
		Vector2<int> offset = this->_ppu.getBgScroll(this->_bgNumber);
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
		// X horizontal
		// Y vertical
		graphicAddress = this->_tilesetAddress + (tileData.posY * NB_TILE_PER_ROW * this->_bpp * TILE_SIZE) + (tileData.posX * this->_bpp * TILE_SIZE);
		for (int i = 0; i < this->_characterSize.y; i++) {
			index = i * this->_characterSize.x;
			if (tileData.verticalFlip)
				index = (this->_characterSize.y - 1 - i) * this->_characterSize.x;
			if (tileData.horizontalFlip)
				index += this->_characterSize.x - 1;
			for (int j = 0; j < this->_characterSize.x; j++) {
				reference = getPixelReferenceFromTile(graphicAddress, index);
				color = getRealColor(palette[reference]);
				if (tileData.tilePriority == this->_priority) // reference 0 is considered as transparency
					this->buffer[pos.x][pos.y] = (reference) ? color : 0;
				index += (tileData.horizontalFlip) ? -1 : 1;
				pos.x++;
			}
			pos.x -= this->_characterSize.x;
			pos.y++;
		}
	}

	std::vector<uint16_t> Background::getPalette(int nbPalette)
	{
		uint8_t nbColors = std::pow(2, this->_bpp);
		uint16_t addr = nbPalette * this->_bpp * this->_bpp * 2; // 2 because it's 2 addr for 1 color
		std::vector<uint16_t> palette(nbColors);

		switch (this->_ppu.getBgMode()) {
		case 0:
			addr += (this->_bgNumber - 1) * (4 * 8) * 2;
			break;
		default:
			break;
		}

		for (int i = 0; i < nbColors; i++) {
			palette[i] = this->_cgram->read(addr);
			palette[i] += this->_cgram->read(addr + 1) << 8U;
			addr += 2;
		}
		return palette;
	}

	uint8_t Background::getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex)
	{
		uint8_t row = pixelIndex / this->_characterSize.x;
		uint8_t column = pixelIndex % this->_characterSize.y;

		if (row >= TILE_PIXEL_HEIGHT) {
			tileAddress += 0x80 * this->_bpp;
			row -= TILE_PIXEL_HEIGHT;
		}
		if (column >= TILE_PIXEL_WIDTH) {
			tileAddress += 0x8 * this->_bpp;
			column -= TILE_PIXEL_WIDTH;
		}
		// might not work with 8 bpp must check
		tileAddress += 2 * row;

		return this->getPixelReferenceFromTileRow(tileAddress, column);
	}

	uint8_t Background::getPixelReferenceFromTileRow(uint16_t tileAddress, uint8_t pixelIndex)
	{
		uint8_t highByte = this->_vram->read(tileAddress % VRAMSIZE);
		uint8_t lowByte = this->_vram->read((tileAddress + 1) % VRAMSIZE);
		uint8_t secondHighByte;
		uint8_t secondLowByte;
		uint16_t result = 0;
		uint8_t shift = (TILE_PIXEL_WIDTH - 1U - pixelIndex);

		switch (this->_bpp) {
		case 8:
			return highByte;
		case 4:
			secondHighByte =  this->_vram->read((tileAddress + 16) % VRAMSIZE);
			secondLowByte = this->_vram->read((tileAddress + 17) % VRAMSIZE);
			result = ((secondHighByte & (1U << shift)) | ((secondLowByte & (1U << shift)) << 1U));
			result = (shift - 2 >= 0) ? result >> (shift - 2) : result << ((shift - 2) * -1);
			__attribute__((fallthrough));
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
			tileMapValue = this->_vram->read(vramAddress);
			tileMapValue += this->_vram->read(vramAddress + 1) << 8U;
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
		this->_tileMapStartAddress = address;
	}

	void Background::setTilesetAddress(uint16_t address)
	{
		this->_tilesetAddress = address;
	}

	void Background::setCharacterSize(Vector2<int> size)
	{
		this->_characterSize = size;
	}

	void Background::setBpp(int bpp)
	{
		if (bpp == 2 || bpp == 4 || bpp == 8 || bpp == 7)
			this->_bpp = bpp;
		else
			this->_bpp = 2;
	}

	void Background::setTilemaps(Vector2<int> tileMaps)
	{
		this->_tileMaps = tileMaps;
	}

	void Background::setBgNumber(int bgNumber)
	{
		this->_bgNumber = bgNumber;
	}

	int Background::getBgNumber() const
	{
		return this->_bgNumber;
	}

	void Background::setPriority(bool priority)
	{
		this->_priority = priority;
	}

	bool Background::getPriority() const
	{
		return this->_priority;
	}
}