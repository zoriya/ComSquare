//
// Created by cbihan on 3/27/20.
//

#include "CGramDebug.hpp"
#include "../SNES.hpp"
#include <QColor>
#include "../Utility/Utility.hpp"
#include "../Exceptions/InvalidAction.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Debugger
{
	CGramDebug::CGramDebug(SNES &snes, ComSquare::PPU::PPU &ppu)
		: _window(new ClosableWindow<CGramDebug>(*this, &CGramDebug::disableViewer)),
		  _snes(snes),
		  _ui(),
		  _model(ppu),
		  _ppu(ppu)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);

		this->_window->show();
	}

	void CGramDebug::disableViewer()
	{
		this->_snes.disableCgramDebugging();
	}

	void CGramDebug::focus()
	{
		this->_window->activateWindow();
	}

	bool CGramDebug::isDebugger()
	{
		return true;
	}

	uint16_t CGramDebug::read(uint8_t addr)
	{
		return this->_ppu.cgramRead(addr);
	}
}

CGramModel::CGramModel(ComSquare::PPU::PPU &ppu) : _ppu(ppu) {}

int CGramModel::rowCount(const QModelIndex &) const
{
	return this->rows;
}

int CGramModel::columnCount(const QModelIndex &) const
{
	return this->column;
}

QVariant CGramModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role == Qt::BackgroundRole)
		return 1;
	this->_ppu.cgramRead(0);
	if (role != Qt::DisplayRole)
		return QVariant();
}