//
// Created by cbihan on 5/7/21.
//

namespace ComSquare::Renderer
{
	class QtFullSFML;
}

#include "../Renderer/QtRenderer/QtSFML.hpp"
#include "TileViewer.hpp"
#include "../SNES.hpp"
#include <QColor>
#include <string>
#include <iostream>
#include <QtWidgets/QTableWidget>
#include "../Utility/Utility.hpp"

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
		this->_sfWidget(this->_ui.tab);
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

}