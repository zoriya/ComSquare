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
		  _renderSize(5000),
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
		int bufX = this->_offsetX;
		int bufY = this->_offsetY;
		int nbTilesDrawn = 0;
		int resetX = bufX;
		int it = 0;
		for (auto &i : buffer)
			i.fill(0);

		for (uint24_t i = 0; i < fmin(this->_ram->getSize(), this->_renderSize); i += 2, it++) {
			if (bufX > 128 || bufY > 128)
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
				resetX = this->_offsetX;
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

	uint8_t TileRenderer::getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex)
	{
		size_t size = this->_ram->getSize();
		uint8_t highByte = this->_ram->read(tileRowAddress % size);
		uint8_t lowByte = this->_ram->read((tileRowAddress + 1) % size);
		uint8_t secondHighByte;
		uint8_t secondLowByte;
		uint16_t result = 0;
		uint8_t shift = 8 - 1U - pixelIndex;

		switch (this->_bpp) {
		case 8:
			return highByte;
		case 4:
			secondHighByte = this->_ram->read((tileRowAddress + 16) % size);
			secondLowByte = this->_ram->read((tileRowAddress + 17) % size);
			result = ((secondHighByte & (1U << shift)) | ((secondLowByte & (1U << shift)) << 1U));
			result = (shift - 2 >= 0) ? result >> (shift - 2) : result << ((shift - 2) * -1);
			FALLTHROUGH
		case 2:
			result += ((highByte & (1U << shift)) | ((lowByte & (1U << shift)) << 1U)) >> shift;
		default:
			break;
		}
		return result;
	}

	std::vector<uint16_t> TileRenderer::getPalette(int nbPalette)
	{
		uint8_t nbColors = std::pow(2, this->_bpp);
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
}