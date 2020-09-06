//
// Created by cbihan on 1/27/20.
//

#ifndef COMSQUARE_PPU_UTILS_HPP
#define COMSQUARE_PPU_UTILS_HPP

#include <stdint-gcc.h>

namespace ComSquare::PPU
{
	union TileMapData {
		struct {
			uint8_t posX: 4;
			uint8_t posY: 6;
			uint8_t palette: 3;
			bool tilePriority: 1;
			bool horizontalFlip: 1;
			bool verticalFlip: 1;
		};
		uint16_t raw;
	};
}
#endif //COMSQUARE_PPU_UTILS_HPP