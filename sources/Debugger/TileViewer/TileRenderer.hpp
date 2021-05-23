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
		int _palette;
	public:
		//! @brief internal buffer
		std::array<std::array<uint32_t, 1024>, 1024> buffer;
		void setPalette(int palette);
		void setCgram(std::shared_ptr<Ram::Ram> ram);
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
}

