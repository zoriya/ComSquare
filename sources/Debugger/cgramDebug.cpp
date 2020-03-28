//
// Created by cbihan on 3/27/20.
//

#include "cgramDebug.hpp"
#include "../SNES.hpp"
#include <QColor>
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidAction.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Debugger
{
	cgramDebug::cgramDebug(SNES &snes, ComSquare::PPU::PPU &ppu)
		: _window(new ClosableWindow<cgramDebug>(*this, &cgramDebug::disableViewer)),
		  _snes(snes),
		  _ui(),
		  _model(),
		  _ppu(ppu)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);

		this->_window->show();
	}

	void cgramDebug::disableViewer()
	{
		this->_snes.disableCgramDebugging();
	}

	void cgramDebug::focus()
	{
		this->_window->activateWindow();
	}

	bool cgramDebug::isDebugger()
	{
		return true;
	}

	uint16_t cgramDebug::read(uint8_t addr)
	{
		return this->_ppu.cgramRead(addr);
	}
}

int cgramModel::rowCount(const QModelIndex &) const
{
	return this->rows;
}

int cgramModel::columnCount(const QModelIndex &) const
{
	return this->column;
}

QVariant cgramModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role == Qt::BackgroundRole)
		return 1;
	this->_ppu.cgramRead(0);
	if (role != Qt::DisplayRole)
		return QVariant();
}

void cgramModel::ppu(const ComSquare::PPU::PPU &ppu)
{
	this->_ppu = ppu;
}
