//
// Created by cbihan on 3/27/20.
//

#include "CGramDebug.hpp"
#include "../SNES.hpp"
#include <QColor>
#include <iostream>
#include <bitset>
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
		this->_ui.cgram_view->setModel(&this->_model);

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
	u_int16_t addressValue;
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::BackgroundRole)
		return QVariant();
	addressValue = this->_ppu.cgramRead(index.column() * 16 + index.row());

	blue = (addressValue & 0x7D00U) >> 10U;
	green = (addressValue & 0x03E0U) >> 5U;
	red = (addressValue & 0x001FU);

	red = red * 255U / 31U;
	green = green * 255U / 31U;
	blue = blue * 255U / 31U;
	return QColor(red, green, blue);
}