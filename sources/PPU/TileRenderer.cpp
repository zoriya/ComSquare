//
// Created by cbihan on 24/05/2021.
//

#include <complex>
#include <cmath>
#include "TileRenderer.hpp"
#include "PPU/PPU.hpp"
#include "PPU/Tile.hpp"
#include <iostream>

namespace ComSquare::PPU
{
	TileRenderer::TileRenderer(Ram::Ram &vram, Ram::Ram &cgram)
		: _ram(vram),
		  _cgram(cgram),
		  _bpp(2),
		  _paletteIndex(0),
		  buffer({{{0}}})
	{
	}

	void TileRenderer::render(uint16_t tileAddress)
	{
		std::vector<uint16_t> palette = this->getPalette(this->_paletteIndex);
		int it = 0;
		this->buffer = {{{0}}};

		for (auto &row : this->buffer) {
			for (auto &pixel : row) {
				uint8_t pixelReference = this->getPixelReferenceFromTile(tileAddress, it++);
				pixel = pixelReference ? Utils::CGRAMColorToRGBA(palette[pixelReference]) : 0;
			}
		}
	}

	uint8_t TileRenderer::getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex)
	{
		uint8_t row = pixelIndex / Tile::NbPixelsWidth;
		uint8_t column = pixelIndex % Tile::NbPixelsHeight;

		if (row >= Tile::NbPixelsHeight) {
			tileAddress += 0x80 * this->_bpp;
			row -= Tile::NbPixelsHeight;
		}
		if (column >= Tile::NbPixelsWidth) {
			tileAddress += 0x8 * this->_bpp;
			column -= Tile::NbPixelsWidth;
		}
		// TODO might not work with 8 bpp must check
		tileAddress += 2 * row;

		return this->getPixelReferenceFromTileRow(tileAddress, column);
	}

	void TileRenderer::setPaletteIndex(int paletteIndex)
	{
		this->_paletteIndex = paletteIndex;
	}

	void TileRenderer::setBpp(int bpp)
	{
		this->_bpp = bpp;
	}

	uint8_t TileRenderer::read2BPPValue(uint16_t tileRowAddress, uint8_t pixelIndex)
	{
		size_t size = this->_ram.getSize();
		uint8_t highByte = this->_ram.read(tileRowAddress % size);
		uint8_t lowByte = this->_ram.read((tileRowAddress + 1) % size);
		uint8_t shift = 8 - 1U - pixelIndex;

		return ((highByte & (1U << shift)) | ((lowByte & (1U << shift)) << 1U)) >> shift;
	}

	uint8_t TileRenderer::getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex)
	{
		// TODO unit test this
		uint16_t result = 0;

		switch (this->_bpp) {
		case 8:
			result += this->read2BPPValue(tileRowAddress + TileByteSizeRow * 3, pixelIndex) << 6;
			result += this->read2BPPValue(tileRowAddress + TileByteSizeRow * 2, pixelIndex) << 4;
			FALLTHROUGH
		case 4:
			result += this->read2BPPValue(tileRowAddress + TileByteSizeRow, pixelIndex) << 2;
			FALLTHROUGH
		case 2:
			result += this->read2BPPValue(tileRowAddress, pixelIndex);
		default:
			break;
		}
		return result;
	}

	std::vector<uint16_t> TileRenderer::getPalette(int nbPalette)
	{
		// todo if needed the tile renderer could cache the palette to avoid recompute this every render
		uint16_t nbColors = std::pow(2, this->_bpp);
		uint16_t addr = nbPalette * this->_bpp * this->_bpp * 2; // 2 because it's 2 addr for 1 color
		std::vector<uint16_t> palette(nbColors);

		for (int i = 0; i < nbColors; i++) {
			palette[i] = this->_cgram.read(addr);
			palette[i] += this->_cgram.read(addr + 1) << 8U;
			addr += 2;
		}
		return palette;
	}

	int TileRenderer::getBpp() const
	{
		return this->_bpp;
	}

	int TileRenderer::getPaletteIndex() const
	{
		return this->_paletteIndex;
	}
}