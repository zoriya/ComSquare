//
// Created by cbihan on 24/05/2021.
//

#include <complex>
#include <cmath>
#include "TileRenderer.hpp"
#include "PPU/PPU.hpp"
#include "PPU/Tile.hpp"
#include <iostream>

namespace ComSquare::Debugger
{
	TileRenderer::TileRenderer()
		: _ram(nullptr),
		  _cgram(nullptr),
		  _bpp(2),
		  _paletteIndex(0),
		  _renderSize(0x5000),
		  _nbColumns(16),
		  buffer({{{0}}})
	{
	}

	void TileRenderer::setRam(std::shared_ptr<Ram::Ram> ram)
	{
		this->_ram = std::move(ram);
	}

	void TileRenderer::render()
	{
		uint8_t colorReference;
		uint24_t color;
		std::vector<uint16_t> palette = this->getPalette(this->_paletteIndex);
		int bufX = 0;
		int bufY = 0;
		int nbTilesDrawn = 0;
		int resetX = bufX;
		int it = 0;
		for (auto &i : buffer)
			i.fill(0);

		for (uint24_t i = this->_ramOffset; i + this->_ramOffset < fmin(this->_ram->getSize(), this->_renderSize); i += 2, it++) {
			if (bufX > 1024 || bufY > 1024)
				break;
			if (it && it % 8 == 0) {
				resetX += PPU::Tile::NbPixelsWidth;
				bufX = resetX;
				bufY -= PPU::Tile::NbPixelsHeight;
				i += (this->_bpp - 2) * 0x8;
				nbTilesDrawn++;
			}
			if (nbTilesDrawn && nbTilesDrawn % this->_nbColumns == 0) {
				nbTilesDrawn = 0;
				resetX = 0;
				bufX = resetX;
				bufY += PPU::Tile::NbPixelsHeight;
			}
			for (int j = 0; j < 8; j++) {
				colorReference = this->getPixelReferenceFromTileRow(i, j);
				color = PPU::getRealColor(palette[colorReference]);
				buffer[bufX++][bufY] = color;
			}
			bufY++;
			bufX = resetX;
		}
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
		// TODO unit test this
		size_t size = this->_ram->getSize();
		uint8_t highByte = this->_ram->read(tileRowAddress % size);
		uint8_t lowByte = this->_ram->read((tileRowAddress + 1) % size);
		uint8_t shift = 8 - 1U - pixelIndex;

		return ((highByte & (1U << shift)) | ((lowByte & (1U << shift)) << 1U)) >> shift;
	}

	uint8_t TileRenderer::getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex)
	{
		// TODO unit test this
		uint16_t result = 0;
		// TODO do a constexpr
		const int TileByteSizeRow = 16;

		switch (this->_bpp) {
		case 8:
			result += this->read2BPPValue(tileRowAddress + TileByteSizeRow * 2, pixelIndex) << 4;
			result += this->read2BPPValue(tileRowAddress + TileByteSizeRow * 3, pixelIndex) << 6;
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
		uint16_t nbColors = std::pow(2, this->_bpp);
		uint16_t addr = nbPalette * this->_bpp * this->_bpp * 2; // 2 because it's 2 addr for 1 color
		std::vector<uint16_t> palette(nbColors);

		for (int i = 0; i < nbColors; i++) {
			palette[i] = this->_cgram->read(addr);
			palette[i] += this->_cgram->read(addr + 1) << 8U;
			addr += 2;
		}
		return palette;
	}

	void TileRenderer::setCgram(std::shared_ptr<Ram::Ram> ram)
	{
		this->_cgram = std::move(ram);
	}

	void TileRenderer::setRenderSize(int size)
	{
		this->_renderSize = size;
	}

	void TileRenderer::setNbColumns(int nbColumns)
	{
		this->_nbColumns = nbColumns;
	}

	int TileRenderer::getBpp() const
	{
		return this->_bpp;
	}

	int TileRenderer::getPaletteIndex() const
	{
		return this->_paletteIndex;
	}

	int TileRenderer::getNbColumns() const
	{
		return this->_nbColumns;
	}

	void TileRenderer::setRamOffset(int offset)
	{
		this->_ramOffset = offset;
	}
}