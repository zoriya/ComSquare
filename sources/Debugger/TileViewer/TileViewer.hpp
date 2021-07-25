//
// Created by cbihan on 5/7/21.
//

#pragma once

#include <QtCore/QSortFilterProxyModel>
#include <QEvent>
#include <QMouseEvent>
#include <array>
#include "PPU/PPU.hpp"
#include "Debugger/ClosableWindow.hpp"
#include "Renderer/QtRenderer/QtSfmlTileRenderer.hpp"
#include "ui/ui_tileView.h"
#include "Ram/Ram.hpp"
#include "RAMTileRenderer.hpp"

namespace ComSquare::Debugger
{

	//! @brief window that allow the user to view all data going through the memory bus.
	class TileViewer : public QObject
	{
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow *_window;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief A widget that contain the whole UI.
		Ui::TileViewer _ui;
		//! @brief A reference to the ppu
		ComSquare::PPU::PPU &_ppu;
		//! @brief the window
		//std::unique_ptr<Renderer::QtSFMLTileRenderer> _sfWidget;
		//! @brief The ram tile renderer
		RAMTileRenderer _ramTileRenderer;
		//! @brief The instantiation of the renderer (should be passed via argument)
		Renderer::QtWidgetSFML _qtSfmlRenderer;
		//! @brief Renderer used to display tiles
		Renderer::IRenderer &_renderer;
		//! @brief Change the bpp from the index given by the ui (QT combo box)
		void _bppChangeUIHandler(int index);

	public:
		//! @brief ctor
		explicit TileViewer(SNES &snes, ComSquare::PPU::PPU &ppu);
		//! @brief copy ctor
		TileViewer(const TileViewer &) = delete;
		//! @brief dtor
		~TileViewer() override = default;
		//! @brief assignment operator
		TileViewer &operator=(const TileViewer &) = delete;

		//! @brief Read data at the CGRAM address send it to the debugger.
		//! @param addr The address to read from.
		//! @return The color value in BGR, looks like this xbbbbbgggggrrrrr.
		uint16_t read(uint8_t addr);
		//! @brief Focus the debugger's window.
		void focus();
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
		[[nodiscard]] int getBpp() const;
		//! @brief Get the index of the current palette used
		[[nodiscard]] int getPaletteIndex() const;
		//! @brief Get the numbr of maximum tile columns to render
		[[nodiscard]] int getNbColumns() const;
		//! @brief Update the tile renderer
		void internalUpdate();
	};
}
