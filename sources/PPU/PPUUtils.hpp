//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_UTILS_HPP
#define COMSQUARE_PPU_UTILS_HPP

#include <stdint-gcc.h>

namespace ComSquare::PPU
{
	//! @brief Used to parse easily VRAM Tile information
	union TileMapData {
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

}
#endif //COMSQUARE_PPU_UTILS_HPP