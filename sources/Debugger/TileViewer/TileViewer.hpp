//
// Created by cbihan on 5/7/21.
//

#pragma once

namespace ComSquare::PPU
{
	class PPU;
}

#include <QtCore/QSortFilterProxyModel>
#include <QEvent>
#include <QMouseEvent>
#include <array>
#include "PPU/PPU.hpp"
#include "Debugger/ClosableWindow.hpp"
#include "Renderer/QtRenderer/QtSFML.hpp"
#include "../../../ui/ui_tileView.h"
#include "Ram/Ram.hpp"


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
		//! @brief the window
		std::unique_ptr<Renderer::QtSFML> _sfWidget;
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
	};
}
