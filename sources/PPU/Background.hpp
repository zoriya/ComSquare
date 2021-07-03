//
// Created by cbihan on 5/14/20.
//

#pragma once

#include <stdint-gcc.h>
#include <array>
#include <vector>
#include "../Models/Vector2.hpp"
#include "TileRenderer.hpp"
#include "../Ram/Ram.hpp"
#include "PPU.hpp"

namespace ComSquare::PPU
{
	class PPU;
	class Background {
	private:
		//! @brief The number of character a TileMap has in width
		static constexpr int NbCharacterWidth = 32;
		//! @brief The number of character a TileMap has in height
		static constexpr int NbCharacterHeight = 32;
		//! @brief The number of rows in one line of VRAM
		//! @note If you're lost by this description, open a tile viewer in an emulator, and set the number of tiles in width to 16 graphics
		static constexpr unsigned NbTilePerRow = 16;
		//! @brief The size of a TileMap in memory
		static constexpr unsigned short TileMapByteSize = 0x800;


		//! @brief the ppu used to get registers values (ex: bg scroll)
		ComSquare::PPU::PPU &_ppu;
		//! @brief The tilemap configuration nb of tileMap vertically and horizontally
		//! @note members are set to 1 if the tilemap is expended in their direction
		Vector2<int> _tileMapsConfig;
		//! @brief The number of pixels of a character (x: width, y:height)
		Vector2<int> _characterNbPixels;
		//! @brief The number of bits per pixels to currently look for each pixel
		int _bpp;
		// TODO make better doc for direct color & high res
		//! @brief PPU official direct color mode
		bool _directColor;
		//! @brief PPU offical highRes mode
		bool _highRes;
		//! @brief The first address of the tilemap data
		uint16_t _tileMapStartAddress;
		//! @brief The first address for tileset data
		uint16_t _tilesetAddress;
		//! @brief If pixel from this background should be treated as primarily
		bool _priority;
		//! @brief The bg number (used to get the corresponding scroll)
		int _bgNumber;
		//! @brief Class that actually render a tile
		TileRenderer _tileRenderer;
		//! @brief Buffer if we have tiles that are more than  8x8
		std::array<std::array<uint32_t, 16>, 16> _tileBuffer;
		//! @brief the access to vram
		std::shared_ptr<Ram::Ram> _vram;
		//! @brief The access to cgram
		std::shared_ptr<Ram::Ram> _cgram;
		//! @brief Draw a tile on the screen at x y pos
		void _drawBgTile(uint16_t data, Vector2<int> pos);
		//! @brief draw a tileMap 32x32 starting at baseAddress
		//! @param baseAddress The starting address of the tileMap
		//! @param offset The rendering offeset in pixels
		void _drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset);
	public:
		//! @brief The size of the background (x, y)
		Vector2<unsigned> backgroundSize;
		//! @brief The output buffer (pixels are written on it)
		std::array<std::array<uint32_t, 1024>, 1024> buffer;

		//! @brief Render a background on his internal buffer
		void renderBackground();
		//! @brief Set the tileMap start address
		//! @param address TileMap start address
		void setTileMapStartAddress(uint16_t address);
		//! @brief Set the character Size
		//! @param size The character size (8x8, 16x16, 16x8, 8x16)
		void setCharacterSize(Vector2<int> size);
		//! @brief Set the tileset address of the background
		void setTilesetAddress(uint16_t address);
		//! @brief Set the bpp (bits per pixels) of the Background
		//! @info The bpp can be 2, 4 or 8 (7 can be possible when BgMode is 7)
		void setBpp(int bpp);
		//! @brief setter for private variable _tileMaps
		//! @param tileMaps The tileMaps to set
		void setTilemaps(Vector2<int> tileMaps);

		//! @brief Get the BackGround Number
		//! @return the current Background number
		int getBgNumber() const;
		//! @brief set the Background priority
		//! @param bgNumber the new Background priority
		void setPriority(bool priority);
		//! @brief Get the Background priority
		//! @return the current Background priority
		bool getPriority() const;

		//! @brief ctor
		Background(ComSquare::PPU::PPU &_ppu, int backGroundNumber, bool hasPriority);
		//! @brief Default copy ctor
		Background(const Background &) = default;
		//! @brief Default destructor
		~Background() = default;
		//! @brief Delete assignment operator
		Background &operator=(const Background &) = delete;
	};
}