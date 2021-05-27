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
#include "Utility/Utility.hpp"
#include "TileRenderer.hpp"
#include "PPU/PPU.hpp"

namespace ComSquare::Debugger
{
	TileViewer::TileViewer(SNES &snes, ComSquare::PPU::PPU &ppu)
		: _window(new ClosableWindow<TileViewer>(*this, &TileViewer::disableViewer)),
		  _snes(snes),
		  _ui(),
		  _ppu(ppu),
		  _tileRenderer()
	{
		this->_tileRenderer.setRam(ppu.vram);
		this->_tileRenderer.setCgram(ppu.cgram);
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		//this->_sfWidget = std::make_unique<Renderer::QtSFML>(this->_ui.tab);
		QMainWindow::connect(this->_ui.NbColumns, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int nb) -> void { this->setNbColumns(nb); });
		QMainWindow::connect(this->_ui.ByteSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int nb) -> void { this->setRenderSize(nb); });
		QMainWindow::connect(this->_ui.Address, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int nb) -> void { this->setRamOffset(nb); });
		QMainWindow::connect(this->_ui.PaletteIndex, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int nb) -> void { this->setPaletteIndex(nb); });
		QMainWindow::connect(this->_ui.BppFormat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) -> void { this->_bppChangeUIHandler(index); });

		// used to setup ui restrictions
		this->setBpp(this->getBpp());
		this->_window->show();
		this->internalUpdate();
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

	void TileViewer::setPaletteIndex(int paletteIndex)
	{
		this->_tileRenderer.setPaletteIndex(paletteIndex);
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
		this->_tileRenderer.setBpp(bpp);
		this->internalUpdate();
	}

	void TileViewer::setNbColumns(int nbColumns)
	{
		this->_tileRenderer.setNbColumns(nbColumns);
		this->internalUpdate();
	}

	void TileViewer::setRenderSize(int size)
	{
		this->_tileRenderer.setRenderSize(size);
		this->internalUpdate();
	}

	int TileViewer::getBpp() const
	{
		return this->_tileRenderer.getBpp();
	}

	int TileViewer::getPaletteIndex() const
	{
		return this->_tileRenderer.getPaletteIndex();
	}

	int TileViewer::getNbColumns() const
	{
		return this->_tileRenderer.getNbColumns();
	}

	void TileViewer::internalUpdate()
	{
		this->_tileRenderer.render();
		this->_ppu.add_buffer(this->_tileRenderer.buffer, {200, 200});
	}

	void TileViewer::setRamOffset(int offset)
	{
		this->_tileRenderer.setRamOffset(offset);
		this->internalUpdate();
	}

	void TileViewer::_bppChangeUIHandler(int index)
	{
		switch (index) {
		case 0: return this->setBpp(2);
		case 1: return this->setBpp(4);
		case 2: return this->setBpp(8);
		default:
			break;
		}
		// TODO error handling
	}
}