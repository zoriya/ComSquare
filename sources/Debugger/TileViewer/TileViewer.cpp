//
// Created by cbihan on 5/7/21.
//

namespace ComSquare::Renderer
{
	class QtFullSFML;
}

#include "Renderer/QtRenderer/QtSFML.hpp"
#include "TileViewer.hpp"
#include "SNES.hpp"
#include <QColor>
#include <string>
#include <iostream>
#include <QtWidgets/QTableWidget>
#include <utility>
#include <complex>
#include "Utility/Utility.hpp"
#include "PPU/PPU.hpp"

namespace ComSquare::Debugger
{
	TileViewer::TileViewer(SNES &snes, ComSquare::PPU::PPU &ppu)
		: _window(new ClosableWindow<TileViewer>(*this, &TileViewer::disableViewer)),
		  _snes(snes),
		  _ui(),
		  _ppu(ppu)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_sfWidget = std::make_unique<Renderer::QtSFML>(this->_ui.tab);
		this->_window->show();
		QEvent::registerEventType();
	}

	void TileViewer::disableViewer()
	{
		this->_snes.disableTileViewerDebugging();
	}

	void TileViewer::focus()
	{
		this->_window->activateWindow();
	}

	bool TileViewer::isDebugger()
	{
		return true;
	}

	uint16_t TileViewer::read(uint8_t addr)
	{
		return this->_ppu.cgramRead(addr);
	}

	TileRenderer::TileRenderer()
		: _ram(nullptr),
		  _cgram(nullptr),
		  _bpp(2),
		  _palette(0),
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
		std::vector<uint16_t> palette = this->getPalette(this->_palette);
		int bufX = 0;
		int bufY = 0;

		for (uint24_t i = 0; i < this->_ram->getSize(); i += this->_bpp) {
			for (int j = 0; j < 8; j ++) {
				colorReference = this->getPixelReferenceFromTileRow(i, j);
				color =  PPU::getRealColor(palette[colorReference]);
				buffer[bufY++][bufX++] = color;
			}
		}
	}

	void TileRenderer::setPalette(int palette)
	{
		this->_palette = palette;
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
			secondHighByte =  this->_ram->read((tileRowAddress + 16) % size);
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

}