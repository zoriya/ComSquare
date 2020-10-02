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

//! @brief Transform SNES color code BGR to uint32_t RGB
uint32_t getRealColor(uint16_t color);

namespace ComSquare::PPU
{
	class PPU;
	class Background {
	#define NB_CHARACTER_WIDTH 32
	#define NB_CHARACTER_HEIGHT 32
	#define TILE_PIXEL_WIDTH 8U
	#define TILE_PIXEL_HEIGHT 8U
	private:
		Vector2<int> _tileMaps;
		Vector2<int> _characterSize;
		int _bpp;
		bool _directColor;
		bool _highRes;
		uint16_t _TileMapStartAddress;
		uint16_t _tileSetAddress;

		std::shared_ptr<Ram::Ram> _vram;
		std::shared_ptr<Ram::Ram> _cgram;
		//! @brief Draw a tile on the screen at x y pos
		void drawBgTile(uint16_t data, Vector2<int> pos);
		//! @brief Get a palette from the number of the palette (0 - 7)
		std::vector<uint16_t> getPalette(int nbPalette);
		//! @brief Get the color reference of a index pixel tile
		//! @param tileAddress The address of the line of pixel
		//TODO support addr as the address of the start of the tile and index goes from 0 to 63 regardless of the bpp
		uint8_t getPixelReferenceFromTileRow(uint16_t tileAddress, uint8_t pixelIndex);
		uint8_t getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex);

		//! @brief draw a tilemap 32x32 starting at baseAddress
		void drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset);
	public:
		// TODO getter setter for priority and bgNumber
		bool priority;
		int bgNumber;
		Vector2<int> backgroundSize;
		std::array<std::array<uint32_t, 1024>, 1024> buffer;
		Background(ComSquare::PPU::PPU &_ppu, int backGroundNumber, bool hasPriority);
		//! @brief Render a background on the screen
		void renderBackground(void);
		//! @brief Set the tilemap start address
		void setTileMapStartAddress(uint16_t address);
		//! @brief Set the character Size
		void setCharacterSize(Vector2<int> size);
	};
}


#endif //COMSQUARE_BACKGROUND_HPP