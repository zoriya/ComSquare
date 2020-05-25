//
// Created by cbihan on 5/14/20.
//

#ifndef COMSQUARE_BACKGROUND_HPP
#define COMSQUARE_BACKGROUND_HPP

#include <stdint-gcc.h>
#include <array>
#include <vector>
#include "../Models/Vector2.hpp"
#include "../Ram/Ram.hpp"
#include "PPU.hpp"

namespace ComSquare::PPU
{
	class PPU;
	class Background {
	private:
		Vector2<int> _tileMaps;
		Vector2<int> _characterSize;
		int _bpp;
		bool _directColor;
		bool _highRes;
		bool _priority;
		uint16_t _TileMapStartAddress;
		uint16_t _tileSetAddress;

		std::shared_ptr<Ram::Ram> _vram;
		std::shared_ptr<Ram::Ram> _cgram;
		//! @brief Draw a tile on the screen at x y pos
		void drawBgTile(uint16_t data, Vector2<int> pos);
		//! @brief Get a palette from the number of the palette (0 - 7)
		std::vector<uint16_t> getPalette(int nbPalette);
		//! @brief Transform SNES color code BGR to uint32_t RGB
		uint32_t getRealColor(uint16_t color);
		//! @brief Get the color reference of a nb pixel tile
		uint8_t getTilePixelReference(uint16_t addr, int nb);
		//! @brief draw a tilemap 32x32 starting at baseAddress
		void drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset);
	public:
		Vector2<int> _backgroundSize;
		std::array<std::array<uint32_t, 1024>, 1024> _buffer;
		Background(ComSquare::PPU::PPU &_ppu, int bgNumber, bool priority);
		//! @brief Render a background on the screen
		void renderBackground(void);
	};
}


#endif //COMSQUARE_BACKGROUND_HPP