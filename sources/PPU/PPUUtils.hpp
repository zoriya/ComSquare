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
			uint16_t posX: 4;
			uint16_t posY: 6;
			uint16_t palette: 3;
			uint16_t tilePriority: 1;
			uint16_t horizontalFlip: 1;
			uint16_t verticalFlip: 1;
		};
		uint16_t raw = 0;
	};

}
#endif //COMSQUARE_PPU_UTILS_HPP