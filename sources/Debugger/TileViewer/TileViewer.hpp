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
#include "Renderer/QtRenderer/QtSfmlTileRenderer.hpp"
#include "../../../ui/ui_tileView.h"
#include "Ram/Ram.hpp"
#include "TileRenderer.hpp"

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
		std::unique_ptr<Renderer::QtSFMLTileRenderer> _sfWidget;
		//! @brief The tile renderer
		TileRenderer _tileRenderer;
		//! @brief Change the bpp from the index given by the ui (QT combo box)
		void _bppChangeUIHandler(int index);
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
		//! @brief Set the palette to use for render (index of palette)
		void setPaletteIndex(int paletteIndex);
		//! @brief Set the bpp to render graphics
		void setBpp(int bpp);
		//! @brief Set the number of maximum columns
		void setNbColumns(int nbColumns);
		//! @brief Set the size of ram to render
		void setRenderSize(int size);
		//! @brief Set the ram offset
		void setRamOffset(int offset);
		//! @brief Get the current bpp
		int getBpp() const;
		//! @brief Get the index of the current palette used
		int getPaletteIndex() const;
		//! @brief Get the numbr of maximum tile columns to render
		int getNbColumns() const;
		//! @brief Update the tile renderer
		void internalUpdate();

	};
}
