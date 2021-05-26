//
// Created by cbihan on 24/05/2021.
//

#pragma once

#include <array>
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
		int _bpp;
		//! @brief The palette number to use while rendering
		int _paletteIndex;
		//! @brief The size to render in the ram
		int _renderSize;
		//! @brief The number of tile columns to display
		int _nbColumns;
		//! @brief render offset in x
		int _offsetX = 0;
		//! @brief render offset in y
		int _offsetY = 0;
	public:
		//! @brief internal buffer
		std::array<std::array<uint32_t, 1024>, 1024> buffer;
		//! @brief Set the palette to use for render (index of palette)
		void setPaletteIndex(int paletteIndex);
		//! @brief Set the ram to look for color references
		void setCgram(std::shared_ptr<Ram::Ram> ram);
		//! @brief Set the bpp to render graphics
		void setBpp(int bpp);
		//! @brief Set the number of maximum columns
		void setNbColumns(int nbColumns);
		//! @brief Set the size of ram to render
		void setRenderSize(int size);
		//! @brief The ram to render
		void setRam(std::shared_ptr<Ram::Ram> ram);
		//! @brief Get the current bpp
		int getBpp() const;
		//! @brief Get the index of the current palette used
		int getPaletteIndex() const;
		//! @brief Get the numbr of maximum tile columns to render
		int getNbColumns() const;
		uint8_t getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex);
		std::vector<uint16_t> getPalette(int nbPalette);
		uint8_t read2BPPValue(uint16_t tileRowAddress, uint8_t pixelIndex);
		//! @brief render the selected ram
		void render();
		TileRenderer();
		TileRenderer(const TileRenderer &) = default;
		~TileRenderer() = default;
		TileRenderer &operator=(const TileRenderer &) = default;
	};
}

