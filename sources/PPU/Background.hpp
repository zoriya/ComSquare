//
// Created by cbihan on 5/14/20.
//

#pragma once

#include <array>
#include <vector>
#include <iostream>
#include "Models/Vector2.hpp"
#include "PPU/TileRenderer.hpp"
#include "Ram/Ram.hpp"
#include "PPU/PPU.hpp"
#include "PPU/PPUUtils.hpp"

namespace ComSquare::PPU
{
	class PPU;

	class Background
	{
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
		PPU &_ppu;
		//! @brief The tilemap configuration nb of tileMap vertically and horizontally
		//! @note members are set to true if the tilemap is expended in their direction
		Vector2<bool> _tileMapMirroring;
		//! @brief The number of pixels of a character (x: width, y: height)
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
		//! @brief The bg number (used to get the corresponding scroll)
		int _bgNumber;
		//! @brief Buffer if we have tiles that are more than  8x8
		std::array<std::array<uint32_t, 16>, 16> _tileBuffer;
		//! @brief the access to vram
		Ram::Ram &_vram;
		//! @brief The access to cgram
		Ram::Ram &_cgram;
		//! @brief Class that actually render a tile
		TileRenderer _tileRenderer;
		//! @brief Draw a tile on the screen at x y pos
		//! @param data The VRAM value to be interpreted as a Utils::TileData
		//! @param indexOffset The index offset of the Tile (ranging from 0 to 63)
		void _drawTile(uint16_t data, Vector2<int> indexOffset);
		//! @brief Draw the tile to the tile Buffer
		//! @param tileData The tile data to use to render the tile
		void _drawTileFromMemoryToTileBuffer(const union Utils::TileData &tileData);
		//! @brief draw a tileMap 32x32 starting at baseAddress
		//! @param baseAddress The starting address of the tileMap
		//! @param offset The offset of the tile map (ranging from 0 to 1)
		void _drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset);
	public:
		//! @brief The size of the background (x, y)
		Vector2<unsigned> backgroundSize;
		//! @brief The output buffer (pixels are written on it)
		std::array<std::array<uint32_t, 1024>, 1024> buffer;
		//! @brief The buffer of tile priority level
		std::array<std::array<bool, 64>, 64> tilesPriority;

		//! @brief Tells if a pixel has high priority
		[[nodiscard]] bool isPriorityPixel(int x, int y) const;
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
		void setTileMapMirroring(Vector2<bool> tileMaps);

		//! @brief Get the BackGround Number
		//! @return the current Background number
		[[nodiscard]] int getBgNumber() const;


		//! @brief Add a bg buffer to another buffer
		//! @tparam levelLow The priority of a low priority pixel (working like z-index CSS property)
		//! @tparam levelHigh The priority of a high priority pixel (working like z-index CSS property)
		//! @tparam DEST_SIZE_X The Horizontal array size
		//! @tparam DEST_SIZE_Y The Vertical array size
		//! @param bufferDest The destination buffer (buffer that will be written on)
		//! @param pixelDestinationLevelMap The destination buffer level map to use as reference and will be updated if a pixel has an higher level than the actual one
		//! @param backgroundSrc The Background to use as a source
		template <int levelLow, int levelHigh, std::size_t DEST_SIZE_X, std::size_t DEST_SIZE_Y>
		static void mergeBackgroundBuffer(std::array<std::array<uint32_t, DEST_SIZE_Y>, DEST_SIZE_X> &bufferDest,
		                                  std::array<std::array<unsigned char, DEST_SIZE_Y>, DEST_SIZE_X> &pixelDestinationLevelMap,
		                                  const Background &backgroundSrc
		                                  )
		{
			int i = 0;
			int j = 0;
			int pixelLevel;

			for (const auto &sourceRow : backgroundSrc.buffer) {
				for (const auto &pixel : sourceRow) {
					if (pixel <= 0xFF) {
						j++;
						continue;
					}
					pixelLevel = backgroundSrc.isPriorityPixel(i, j) ? levelHigh : levelLow;
					auto &pixelInitialLevel = pixelDestinationLevelMap[i][j];

					if (pixelLevel >= pixelInitialLevel) {
						bufferDest[i][j] = pixel;
						pixelInitialLevel = pixelLevel;
					}
					j++;
				};
				j = 0;
				i++;
			};

		}

		//! @brief ctor
		Background(PPU &_ppu, int backgroundNumber);
		//! @brief Default copy ctor
		Background(const Background &) = default;
		//! @brief Default destructor
		~Background() = default;
		//! @brief Delete assignment operator
		Background &operator=(const Background &) = delete;
	};
}