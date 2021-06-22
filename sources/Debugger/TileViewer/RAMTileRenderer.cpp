//
// Created by cbihan on 24/05/2021.
//

#include <complex>
#include <cmath>
#include "RAMTileRenderer.hpp"
#include "PPU/PPU.hpp"
#include "PPU/Tile.hpp"
#include <iostream>

namespace ComSquare::Debugger
{
	RAMTileRenderer::RAMTileRenderer()
		: _ram(nullptr),
		  _renderSize(0x5000),
		  _nbColumns(16),
		  _ramOffset(0),
		  _bpp(2),
		  buffer({{{0}}})
	{
	}

	void RAMTileRenderer::setRam(std::shared_ptr<Ram::Ram> ram)
	{
		this->_ram = ram;
		this->_tileRenderer.setRam(ram);
	}

	void RAMTileRenderer::render()
	{
		int bufX = 0;
		int bufY = 0;
		int nbTilesDrawn = 0;
		int resetX = bufX;
		for (auto &i : buffer)
			i.fill(0);
		uint24_t limit = fmin(this->_ram->getSize(), this->_renderSize) + this->_ramOffset;

		for (uint24_t i = this->_ramOffset; i < limit; i += PPU::Tile::BaseByteSize * this->_bpp, nbTilesDrawn++) {
			if (bufX > 1024 || bufY > 1024)
				break;

			this->_tileRenderer.render(i);
			if (nbTilesDrawn) {
				resetX += PPU::Tile::NbPixelsWidth;
				bufX = resetX;
				bufY -= PPU::Tile::NbPixelsHeight;
			}
			if (nbTilesDrawn && nbTilesDrawn % this->_nbColumns == 0) {
				nbTilesDrawn = 0;
				resetX = 0;
				bufX = resetX;
				bufY += PPU::Tile::NbPixelsHeight;
			}

			for (const auto &raw : this->_tileRenderer.buffer) {
				for (const auto &pixel : raw) {
					buffer[bufX++][bufY] = pixel;
				}
				bufY++;
				bufX = resetX;
			}
		}
	}

	void RAMTileRenderer::setPaletteIndex(int paletteIndex)
	{
		this->_tileRenderer.setPaletteIndex(paletteIndex);
	}

	void RAMTileRenderer::setBpp(int bpp)
	{
		this->_bpp = bpp;
		this->_tileRenderer.setBpp(bpp);
	}

	void RAMTileRenderer::setCgram(std::shared_ptr<Ram::Ram> ram)
	{
		this->_tileRenderer.setCgram(ram);
	}

	void RAMTileRenderer::setRenderSize(int size)
	{
		this->_renderSize = size;
	}

	void RAMTileRenderer::setNbColumns(int nbColumns)
	{
		this->_nbColumns = nbColumns;
	}

	int RAMTileRenderer::getBpp() const
	{
		return this->_tileRenderer.getBpp();
	}

	int RAMTileRenderer::getPaletteIndex() const
	{
		return this->_tileRenderer.getPaletteIndex();
	}

	int RAMTileRenderer::getNbColumns() const
	{
		return this->_nbColumns;
	}

	void RAMTileRenderer::setRamOffset(int offset)
	{
		this->_ramOffset = offset;
	}
}