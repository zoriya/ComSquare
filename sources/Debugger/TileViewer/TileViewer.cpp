//
// Created by cbihan on 5/7/21.
//


#include "Renderer/QtRenderer/QtSFML.hpp"
#include "TileViewer.hpp"
#include "SNES.hpp"
#include <QColor>
#include <string>
#include <iostream>
#include "RAMTileRenderer.hpp"
#include "PPU/PPU.hpp"
#include "PPU/Tile.hpp"

namespace ComSquare::Debugger
{
	TileViewer::TileViewer(SNES &snes, ComSquare::PPU::PPU &ppu)
		:_window(new ClosableWindow([&snes] { snes.disableTileViewer(); })),
		  _snes(snes),
		  _ui(),
		  _ppu(ppu),
		  _ramTileRenderer(ppu.vram, ppu.cgram)
	{
		this->_ui.setupUi(this->_window);
		//this->_qtSfmlRenderer(this->_ui.widget_sfml, 30);
		//this->_renderer = std::make_unique<Renderer::QtSFMLTileRenderer>(this->_ui.widget_sfml, 30);;
		this->_renderer = std::make_unique<Renderer::QtSFMLTileRenderer>(this->_ui.widget_sfml, 30);
		//this->_renderer = nullptr;
	//	this->_sfWidget = std::make_unique<Renderer::QtSFMLTileRenderer>(this->_ui.widget_sfml);
		QMainWindow::connect(this->_ui.NbColumns, QOverload<int>::of(&QSpinBox::valueChanged), this,
		                     [this](int nb) -> void { this->setNbColumns(nb); });
		QMainWindow::connect(this->_ui.ByteSize, QOverload<int>::of(&QSpinBox::valueChanged), this,
		                     [this](int nb) -> void { this->setRenderSize(nb); });
		QMainWindow::connect(this->_ui.Address, QOverload<int>::of(&QSpinBox::valueChanged), this,
		                     [this](int nb) -> void { this->setRamOffset(nb); });
		QMainWindow::connect(this->_ui.PaletteIndex, QOverload<int>::of(&QSpinBox::valueChanged), this,
		                     [this](int nb) -> void { this->setPaletteIndex(nb); });
		QMainWindow::connect(this->_ui.BppFormat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
		                     [this](int index) -> void { this->_bppChangeUIHandler(index); });

		// used to setup ui restrictions
		this->setBpp(this->getBpp());
		this->_window->show();
		this->internalUpdate();
	}

	void TileViewer::focus()
	{
		this->_window->activateWindow();
	}

	uint16_t TileViewer::read(uint8_t addr)
	{
		return this->_ppu.cgramRead(addr);
	}

	void TileViewer::setPaletteIndex(int paletteIndex)
	{
		this->_ramTileRenderer.setPaletteIndex(paletteIndex);
		this->internalUpdate();
	}

	void TileViewer::setBpp(int bpp)
	{
		this->_ui.PaletteIndex->setDisabled(bpp > 4);
		switch (bpp) {
		case 8:
			this->_ui.PaletteIndex->setValue(0);
			break;
		case 4:
			this->_ui.PaletteIndex->setMaximum(15);
			if (this->_ui.PaletteIndex->value() > 15) {
				this->_ui.PaletteIndex->setValue(15);
			}
			break;
		case 2:
		default:
			bpp = 2;
			this->_ui.PaletteIndex->setMaximum(63);
			break;
		}
		this->_ramTileRenderer.setBpp(bpp);
		this->internalUpdate();
	}

	void TileViewer::setNbColumns(int nbColumns)
	{
		this->_ramTileRenderer.setNbColumns(nbColumns);
		this->internalUpdate();
	}

	void TileViewer::setRenderSize(int size)
	{
		this->_ramTileRenderer.setRenderSize(size);
		this->internalUpdate();
	}

	int TileViewer::getBpp() const
	{
		return this->_ramTileRenderer.getBpp();
	}

	int TileViewer::getPaletteIndex() const
	{
		return this->_ramTileRenderer.getPaletteIndex();
	}

	int TileViewer::getNbColumns() const
	{
		return this->_ramTileRenderer.getNbColumns();
	}

	void TileViewer::internalUpdate()
	{
		this->_ramTileRenderer.render();
		int i = 0;
		int j = 0;
		for (const auto &row : this->_ramTileRenderer.buffer) {
			for (const auto &pixel : row) {
				this->_renderer->putPixel(i, j++, pixel);
			}
			j = 0;
			i++;
		}
	}

	void TileViewer::setRamOffset(int offset)
	{
		this->_ramTileRenderer.setRamOffset(offset);
		this->internalUpdate();
	}

	void TileViewer::_bppChangeUIHandler(int index)
	{
		switch (index) {
		case 0:
			return this->setBpp(2);
		case 1:
			return this->setBpp(4);
		case 2:
			return this->setBpp(8);
		default:
			throw std::runtime_error("Invalid Index");
		}
	}
}