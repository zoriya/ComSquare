//
// Created by cbihan on 5/7/21.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include "../PPU/PPU.hpp"
#include "../../ui/ui_tileView.h"
#include <QtCore/QSortFilterProxyModel>
#include <QEvent>
#include <QMouseEvent>
#include <QTableView>
#include "ClosableWindow.hpp"

/*
//! @brief The qt model that bind the logs to the view.
class CGramModel : public QAbstractTableModel
{
Q_OBJECT
private:
	//! @brief The ppu to log the cgram.
	ComSquare::PPU::PPU &_ppu;
public:
	//! @brief The number of columns
	const int column = 16;
	//! @brief The number of rows
	const int rows = 16;
	int x;
	int y;
	explicit CGramModel(ComSquare::PPU::PPU &ppu);
	CGramModel(const CGramModel &) = delete;
	const CGramModel &operator=(const CGramModel &) = delete;
	~CGramModel() override = default;

	//! @brief The number of row the table has.
	int rowCount(const QModelIndex &parent) const override;
	//! @brief The number of column the table has.
	int columnCount(const QModelIndex &parent) const override;
	//! @brief Return a data representing the table cell.
	QVariant data(const QModelIndex &index, int role) const override;
};
 */

namespace ComSquare::Debugger
{
	//! @brief window that allow the user to view all data going through the memory bus.
	class TileViewer : public QObject {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<TileViewer> *_window;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief A widget that contain the whole UI.
		Ui::TileViewer _ui;
		//! @brief A reference to the ppu
		ComSquare::PPU::PPU &_ppu;
	public:
		//! @brief Called when the window is closed. Turn off the debugger.
		void disableViewer();
	public:
		explicit TileViewer(SNES &snes, ComSquare::PPU::PPU &ppu);
		TileViewer(const TileViewer &) = delete;
		TileViewer &operator=(const TileViewer &) = delete;
		~TileViewer() = default;

		//! @brief Read data at the CGRAM address send it to the debugger.
		//! @param addr The address to read from.
		//! @return The color value in BGR, looks like this xbbbbbgggggrrrrr.
		uint16_t read(uint8_t addr);
		//! @brief Focus the debugger's window.
		void focus();
		//! @brief Return true if the Bus is overloaded with debugging features.
		bool isDebugger();
		//! @brief Update the text fields with corresponding tile info
		void updateInfoTile(int row, int column);
		//! @brief Update call updateInfoTile with the correct address
		void tileClicked(const QModelIndex &index);
	};
}
