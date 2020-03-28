//
// Created by cbihan on 3/27/20.
//

#include "CGramDebug.hpp"
#include "../SNES.hpp"
#include <QColor>
#include <iostream>
#include <bitset>
#include <string>
#include "../Utility/Utility.hpp"

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
		updateInfoTile(0);
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

	void CGramDebug::updateInfoTile(uint8_t addr)
	{
		uint16_t cgramValue = this->_ppu.cgramRead(addr);
		uint8_t blue = (cgramValue & 0x7D00U) >> 10U;
		uint8_t green = (cgramValue & 0x03E0U) >> 5U;
		uint8_t red = (cgramValue & 0x001FU);
		uint24_t hexColorValue = 0;

		this->_ui.indexLineEdit->setText(std::to_string(addr).c_str());
		this->_ui.valueLineEdit->setText(std::to_string(cgramValue).c_str());
		this->_ui.rLineEdit->setText(std::to_string(red).c_str());
		this->_ui.gLineEdit->setText(std::to_string(green).c_str());
		this->_ui.bLineEdit->setText(std::to_string(blue).c_str());
		hexColorValue += (red * 255U / 31U) << 16U;
		hexColorValue += (green * 255U / 31U) << 8U;
		hexColorValue += (blue * 255U / 31U);
		this->_ui.hexLineEdit->setText(Utility::to_hex(hexColorValue).c_str());
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