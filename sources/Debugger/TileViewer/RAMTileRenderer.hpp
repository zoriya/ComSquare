//
// Created by cbihan on 24/05/2021.
//

#pragma once

#include <vector>
#include "PPU/TileRenderer.hpp"
#include "Ram/Ram.hpp"

namespace ComSquare::Debugger
{
	class RAMTileRenderer
	{
	private:
		//! @brief ram to render
		Ram::Ram &_ram;
		//! @brief The size to render in the ram
		int _renderSize;
		//! @brief The number of tile columns to display
		int _nbColumns;
		//! @brief Bytes to skip from the start of the ram
		int _ramOffset;
		//! @brief The actual bpp to render
		int _bpp;
		//! @brief The class tha actually render the tile
		PPU::TileRenderer _tileRenderer;
	public:
		//! @brief internal buffer
		std::vector<std::vector<uint32_t>> buffer;
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
		//! @brief render the selected ram
		void render();

		//! @brief ctor
		RAMTileRenderer(Ram::Ram &ram, Ram::Ram &cgram);
		//! @brief copy ctor
		RAMTileRenderer(const RAMTileRenderer &) = default;
		//! @brief dtor
		~RAMTileRenderer() = default;
		//! @brief A RAMTileRender is not assignable.
		RAMTileRenderer &operator=(const RAMTileRenderer &) = delete;
	};
}

