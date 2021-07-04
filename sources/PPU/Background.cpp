//
// Created by cbihan on 5/14/20.
//

#include "PPUUtils.hpp"
#include "PPU.hpp"
#include "Background.hpp"
#include <cmath>
#include "Tile.hpp"
#include "PPUUtils.hpp"
#include "Models/Vector2.hpp"

namespace ComSquare::PPU
{
	Background::Background(ComSquare::PPU::PPU &ppu, int backGroundNumber, bool hasPriority):
		_ppu(ppu),
		_tileMapsConfig(ppu.getBackgroundMirroring(backGroundNumber)),
		_characterNbPixels(ppu.getCharacterSize(backGroundNumber)),
		_bpp(ppu.getBPP(backGroundNumber)),
		_directColor(false),
		_highRes(false),
		_tileMapStartAddress(ppu.getTileMapStartAddress(backGroundNumber)),
		_tilesetAddress(ppu.getTilesetAddress(backGroundNumber)),
		_priority(hasPriority),
		_bgNumber(backGroundNumber),
		_tileBuffer({{{0}}}),
		_vram(ppu.vram),
		_cgram(ppu.cgram),
		buffer({{{0}}})
	{
		this->_tileRenderer.setRam(this->_vram);
		this->_tileRenderer.setCgram(this->_cgram);
	}

	void Background::renderBackground()
	{
		uint16_t vramAddress = this->_tileMapStartAddress;
		Vector2<int> offset = this->_ppu.getBgScroll(this->_bgNumber);
		this->backgroundSize.x =
			(static_cast<int>(this->_tileMapsConfig.x) + 1) * this->_characterNbPixels.x * NbCharacterWidth;
		this->backgroundSize.y =
			(static_cast<int>(this->_tileMapsConfig.y) + 1) * this->_characterNbPixels.y * NbCharacterHeight;

		this->_drawBasicTileMap(vramAddress, offset);
		for (int i = 1; i < 4; i++) {
			vramAddress += TileMapByteSize;
			offset.x += NbCharacterWidth * this->_characterNbPixels.x;
			if (i == 2) {
				offset.x = 0;
				offset.y += NbCharacterHeight * this->_characterNbPixels.y;
			}
			if (i > 1 && !this->_tileMapsConfig.y)
				break;
			if ((i == 1 || i == 3) && !this->_tileMapsConfig.x)
				continue;
			this->_drawBasicTileMap(vramAddress, offset);
		}
	}

	void Background::_drawBgTile(uint16_t data, Vector2<int> pos)
	{
		union Utils::TileMapData tileData;

		tileData.raw = data;

		if (tileData.tilePriority != this->_priority)
			return;

		uint16_t graphicAddress;
		Vector2i tileOffset = {0, 0};
		// X horizontal
		// Y vertical

		this->_tileRenderer.setPaletteIndex(tileData.palette);
		for (int i = 0; i < this->_characterNbPixels.y; i += Tile::NbPixelsHeight) {
			for (int j = 0; j < this->_characterNbPixels.x; j += Tile::NbPixelsWidth) {
				graphicAddress = this->_tilesetAddress +
				                 ((tileData.posY + tileOffset.y) * NbTilePerRow * this->_bpp * Tile::BaseByteSize) +
				                 ((tileData.posX + tileOffset.x) * this->_bpp * Tile::BaseByteSize);
				this->_tileRenderer.render(graphicAddress);
				Utils::merge2DArray(this->_tileBuffer, this->_tileRenderer.buffer, {j, i});
				tileOffset.x += 1;
			}
			tileOffset.x = 0;
			tileOffset.y += 1;
		}

		// todo check why i need to invert vertical and horizontal flips
		if (tileData.verticalFlip)
			Utils::HFlipArray(this->_tileBuffer, {this->_characterNbPixels.x, this->_characterNbPixels.y});
		if (tileData.horizontalFlip)
			Utils::VFlipArray(this->_tileBuffer, {this->_characterNbPixels.x, this->_characterNbPixels.y});
		for (int i = 0; i < this->_characterNbPixels.y; i++) {
			for (int j = 0; j < this->_characterNbPixels.x; j++) {
				//std::copy(this->_tileBuffer.begin(), this->_tileBuffer.begin() + this->_characterNbPixels.x, this->buffer[pos.x].begin() )
				this->buffer[pos.y + i][pos.x + j] = this->_tileBuffer[i][j];
				//pos.x++;
			}
			//pos.x -= this->_characterNbPixels.x;
			//pos.y++;
		}
	}

	void Background::_drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset)
	{
		uint16_t tileMapValue = 0;
		Vector2<int> pos(0, 0);
		uint16_t vramAddress = baseAddress;

		while (vramAddress < baseAddress + TileMapByteSize) {
			// TODO function to read 2 bytes (LSB order or bits reversed)
			tileMapValue = this->_vram->read(vramAddress);
			tileMapValue += this->_vram->read(vramAddress + 1) << 8U;
			_drawBgTile(tileMapValue, {(pos.x * this->_characterNbPixels.x) + offset.x,
			                           (pos.y * this->_characterNbPixels.y) + offset.y});
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
		this->_characterNbPixels = size;
	}

	void Background::setBpp(int bpp)
	{
		if (bpp == 2 || bpp == 4 || bpp == 8 || bpp == 7)
			this->_bpp = bpp;
		else
			this->_bpp = 2;
		this->_tileRenderer.setBpp(this->_bpp);
	}

	void Background::setTilemaps(Vector2<bool> tileMaps)
	{
		this->_tileMapsConfig = tileMaps;
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