//
// Created by cbihan on 5/14/20.
//

#include "PPUUtils.hpp"
#include "PPU.hpp"
#include "Background.hpp"
#include "Tile.hpp"
#include "Models/Vector2.hpp"

namespace ComSquare::PPU
{
	Background::Background(ComSquare::PPU::PPU &ppu, int backgroundNumber)
		: _ppu(ppu),
		  _tileMapMirroring(ppu.getBackgroundMirroring(backgroundNumber)),
		  _characterNbPixels(ppu.getCharacterSize(backgroundNumber)),
		  _bpp(ppu.getBPP(backgroundNumber)),
		  _directColor(false),
		  _highRes(false),
		  _tileMapStartAddress(ppu.getTileMapStartAddress(backgroundNumber)),
		  _tilesetAddress(ppu.getTilesetAddress(backgroundNumber)),
		  _bgNumber(backgroundNumber),
		  _tileBuffer({{{0}}}),
		  _vram(ppu.vram),
		  _cgram(ppu.cgram),
		  _tileRenderer(this->_vram, this->_cgram),
		  buffer({{{0}}}),
		  tilesPriority({{{false}}})
	{}

	void Background::renderBackground()
	{
		uint16_t vramAddress = this->_tileMapStartAddress;
		//Vector2<int> offset = this->_ppu.getBgScroll(this->_bgNumber);
		Vector2i offset = {0, 0};
		this->backgroundSize.x =
			(static_cast<int>(this->_tileMapMirroring.x) + 1) * this->_characterNbPixels.x * NbCharacterWidth;
		this->backgroundSize.y =
			(static_cast<int>(this->_tileMapMirroring.y) + 1) * this->_characterNbPixels.y * NbCharacterHeight;

		this->_drawBasicTileMap(vramAddress, offset);
		for (int i = 1; i < 4; i++) {
			vramAddress += TileMapByteSize;
			offset.x++;
			if (i == 2) {
				offset.x = 0;
				offset.y++;
			}
			if (i > 1 && !this->_tileMapMirroring.y)
				break;
			if ((i == 1 || i == 3) && !this->_tileMapMirroring.x)
				continue;
			this->_drawBasicTileMap(vramAddress, offset);
		}
	}

	void Background::_drawTileFromMemoryToTileBuffer(const Utils::TileData &tileData)
	{
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
				tileOffset.x++;
			}
			tileOffset.x = 0;
			tileOffset.y++;
		}
	}

	void Background::_drawTile(uint16_t data, Vector2<int> indexOffset)
	{
		union Utils::TileData tileData;

		tileData.raw = data;

		this->tilesPriority[indexOffset.y][indexOffset.x] = tileData.tilePriority;
		this->_drawTileFromMemoryToTileBuffer(tileData);

		// todo check why i need to invert vertical and horizontal flips
		if (tileData.verticalFlip)
			Utils::HFlipArray(this->_tileBuffer, {this->_characterNbPixels.x, this->_characterNbPixels.y});
		if (tileData.horizontalFlip)
			Utils::VFlipArray(this->_tileBuffer, {this->_characterNbPixels.x, this->_characterNbPixels.y});

		Vector2<int> pixelPosition{indexOffset.x * this->_characterNbPixels.x, indexOffset.y * this->_characterNbPixels.y};
		std::for_each(this->_tileBuffer.begin(), this->_tileBuffer.begin() + this->_characterNbPixels.y,
		              [this, &pixelPosition](const auto &row) {
			              std::move(row.begin(), row.begin() + this->_characterNbPixels.x,
			                        this->buffer[pixelPosition.y++].begin() + pixelPosition.x);
		              });
	}

	void Background::_drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset)
	{
		Vector2<int> pos(0, 0);
		uint16_t vramAddress = baseAddress;

		while (vramAddress < baseAddress + TileMapByteSize) {
			// TODO function to read 2 bytes (LSB order or bits reversed)
			uint16_t tileMapValue = this->_vram.read(vramAddress);
			tileMapValue += this->_vram.read(vramAddress + 1) << 8U;
			this->_drawTile(tileMapValue, {(offset.x * NbCharacterWidth) + pos.x,
			                               (offset.y * NbCharacterHeight) + pos.y});
			vramAddress += 2;
			if (pos.x % 31 == 0 && pos.x) {
				pos.y++;
				pos.x = 0;
			}
			else {
				pos.x++;
			}
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

	void Background::setTileMapMirroring(Vector2<bool> tileMaps)
	{
		this->_tileMapMirroring = tileMaps;
	}

	int Background::getBgNumber() const
	{
		return this->_bgNumber;
	}

	bool Background::isPriorityPixel(int y, int x) const
	{
		return this->tilesPriority[y / this->_characterNbPixels.y][x / this->_characterNbPixels.x];
	}
}