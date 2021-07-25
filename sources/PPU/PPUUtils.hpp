//
// Created by cbihan on 1/27/20.
//

#pragma once

#include <stdint-gcc.h>
#include <cstddef>
#include <memory>
#include <vector>
#include <array>
#include "Models/Vector2.hpp"

namespace ComSquare::PPU
{
	class PPU;
}

namespace ComSquare::PPU::Utils
{
	//! @brief Transform CGRAM color data to 8bits
	//! @note Used with b, g and r values
	//! @return The 8 bit value of the color
	inline uint8_t to8Bit(int color)
	{
		return static_cast<uint8_t>((color << 3) + (color >> 2));
	}
	//! @brief Transform SNES color code BGR to uint32_t RGBA
	//! @param CGRAMColor The color of the CGRAM
	//! @return The CGRAM color to RGBA format
	uint32_t CGRAMColorToRGBA(uint16_t CGRAMColor);
	//! @brief Used to parse easily VRAM Tile information
	union TileData {
		struct {
			//! @brief Tile X offset
			uint16_t posX: 4;
			//! @brief Tile Y offset
			uint16_t posY: 6;
			//! @brief Palette number used by the Tile
			uint16_t palette: 3;
			//! @brief True if the Tile has priority
			uint16_t tilePriority: 1;
			//! @brief True if the Tile need to be horizontally flipped
			uint16_t horizontalFlip: 1;
			//! @brief True if the Tile need to be vertically flipped
			uint16_t verticalFlip: 1;
		};
		uint16_t raw = 0;
	};

	//! @brief Struct to save all specific variables needed for the registers (prev values for example)
	struct PpuState {
		//! @brief Used by by all eight BGnxOFS registers (0x210D - 0x2114)
		uint8_t hvSharedScrollPrevValue;
		//! @brief Shared by the four BGnHOFS registers
		uint8_t hScrollPrevValue;
	};

	//! @brief Takes the bufferSrc and set the values in bufferDest starting at offset
	//! @param bufferDest The destination buffer (the one who will be written on)
	//! @param bufferSrc The source buffer (the one that will be copied to the bufferSrc)
	//! @param offset The starting position to copy bufferSrc to bufferDest
	//! @tparam DEST_SIZE_X Horizontal size of bufferDest
	//! @tparam DEST_SIZE_Y Vertical size of bufferDest
	//! @tparam SRC_SIZE_X Horizontal size of bufferSrc
	//! @tparam SRC_SIZE_Y Vertical size of bufferSrc
	template <std::size_t DEST_SIZE_Y, std::size_t DEST_SIZE_X, std::size_t SRC_SIZE_Y, std::size_t SRC_SIZE_X>
	void merge2DArray(std::array<std::array<uint32_t, DEST_SIZE_X>, DEST_SIZE_Y> &bufferDest,
					  const std::array<std::array<uint32_t, SRC_SIZE_X>, SRC_SIZE_Y> &bufferSrc,
					  const Vector2<int> &offset)
	{
		int offsetY = offset.y;
		for (const auto &row : bufferSrc) {
			std::copy(row.begin(), row.end(), bufferDest[offsetY++].begin() + offset.x);
		}
	}

	//! @brief Flips Vertically an 2D buffer
	//! @param buffer The buffer to be flipped
	//! @param size The maximum size you want to be split
	//! @param offset The starting position of the flip
	//! @tparam HORIZONTAL_SIZE The horizontal size of the buffer
	//! @tparam VERTICAL_SIZE The vertical size of the buffer
	template <std::size_t VERTICAL_SIZE, std::size_t HORIZONTAL_SIZE>
	void vFlip2DBuffer(std::array<std::array<uint32_t, HORIZONTAL_SIZE>, VERTICAL_SIZE> &buffer,
	                   const Vector2<int> &size = {HORIZONTAL_SIZE, VERTICAL_SIZE},
	                   const Vector2<int> &offset = {0, 0})
	{
		for (int i = offset.y; i < offset.y + size.y; i++) {
			std::reverse(buffer[i].begin() + offset.x, buffer[i].begin() + offset.x + size.x);
		}
	}

	//! @brief Flips Horizontally an 2D buffer
	//! @param buffer The buffer to be flipped
	//! @param size The maximum size you want to be split
	//! @param offset The starting position of the flip
	//! @tparam HORIZONTAL_SIZE The horizontal size of the buffer
	//! @tparam VERTICAL_SIZE The vertical size of the buffer
	//! @warning This function might not behave like you think, it doesn't even look for the size.y but it's only meant to be use in the tile context
	template<std::size_t VERTICAL_SIZE, std::size_t HORIZONTAL_SIZE>
	void hFlip2DBuffer(std::array<std::array<uint32_t, HORIZONTAL_SIZE>, VERTICAL_SIZE> &buffer,
	                   const Vector2<int> &size = {HORIZONTAL_SIZE, VERTICAL_SIZE},
	                   const Vector2<int> &offset = {0, 0})
	{
		std::reverse(buffer.begin() + offset.x, buffer.begin() + offset.x + size.x);
	}

	//! @brief Add a bg buffer to another buffer
	template<std::size_t DEST_SIZE_X, std::size_t DEST_SIZE_Y, std::size_t SRC_SIZE_X, std::size_t SRC_SIZE_Y>
	static void addBuffer(std::array<std::array<uint32_t, DEST_SIZE_Y>, DEST_SIZE_X> &bufferDest,
	                      const std::array<std::array<uint32_t, SRC_SIZE_Y>, SRC_SIZE_X> &bufferSrc)
	{
		int i = 0;
		int j = 0;
		for (const auto &sourceRow : bufferSrc) {
			for (const auto &pixel : sourceRow) {
				if (pixel > 0xFF)
					bufferDest[i][j] = pixel;
				j++;
			};
			j = 0;
			i++;
		};
	}
}
