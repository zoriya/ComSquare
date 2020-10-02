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
		//! @brief Get a palette from the number of the palette
		//! @param nbPalette The palette number (0 - 7)
		//! @return The array of color of the palette
		std::vector<uint16_t> getPalettes(int nbPalette);
		//! @brief Get the color reference of a pixel from the address of the row
		//! @param tileAddress The address of the line of pixel
		//! @param pixelIndex The index of the pixel (0 - 7)
		//! @return The color Reference
		uint8_t getPixelReferenceFromTileRow(uint16_t tileAddress, uint8_t pixelIndex);
		//! @brief Get the color pixel reference from the tile address
		//! @param tileAddress The starting address of the tile
		//! @param pixelIndex The index of the pixel (0 - 255)
		//! @return The color reference
		uint8_t getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex);
		//! @brief draw a tilemap 32x32 starting at baseAddress
		//! @param baseAddress The starting address of the tileMap
		//! @param offset The rendering offeset in pixels
		void drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset);
	public:
		// TODO getter setter for priority and bgNumber
		bool priority;
		int bgNumber;
		Vector2<int> backgroundSize;
		std::array<std::array<uint32_t, 1024>, 1024> buffer;
		Background(ComSquare::PPU::PPU &_ppu, int backGroundNumber, bool hasPriority);
		//! @brief Render a background on his internal buffer
		void renderBackground(void);
		//! @brief Set the tilemap start address
		//! @param address TileMap start address
		void setTileMapStartAddress(uint16_t address);
		//! @brief Set the character Size
		//! @param size The character size (8x8, 16x16, 16x8, 8x16)
		void setCharacterSize(Vector2<int> size);
	};
}


#endif //COMSQUARE_BACKGROUND_HPP