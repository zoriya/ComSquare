//
// Created by cbihan on 5/7/21.
//

#pragma once

#include <QtCore/QSortFilterProxyModel>
#include <QEvent>
#include <QMouseEvent>
#include <array>
#include "PPU/PPU.hpp"
#include "ClosableWindow.hpp"
#include "Renderer/QtRenderer/QtSFML.hpp"
#include "ui/ui_tileView.h"
#include "Ram/Ram.hpp"


namespace ComSquare::Debugger
{
	class TileRenderer {
	private:
		//! @brief ram to render
		std::shared_ptr<Ram::Ram> _ram;
		//! @brief cgram to access the colors
		std::shared_ptr<Ram::Ram> _cgram;
		//! @brief The bpp to use while rendering
		int _bpp,
		//! @brief The palette number to use while rendering
		int _palette;
	public:
		//! @brief internal buffer
		std::array<std::array<uint32_t, 500>, 1024> buffer;
		void setPalette(int palette);
		void setBpp(int bpp);
		void setRam(std::shared_ptr<Ram::Ram> ram);
		uint8_t getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex);
		std::vector<uint16_t> getPalette(int nbPalette);
		//! @brief render the selected ram
		void render();
		TileRenderer();
		TileRenderer(const TileRenderer &) = default;
		~TileRenderer() = default;
		TileRenderer &operator=(const TileRenderer &) = default;
	};

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
