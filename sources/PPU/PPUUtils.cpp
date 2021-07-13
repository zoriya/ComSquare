//
// Created by cbihan on 5/26/20.
//

#include "PPUUtils.hpp"

namespace ComSquare::PPU::Utils
{

	uint32_t CGRAMColorToRGBA(uint16_t CGRAMColor)
	{
		uint8_t b = to8Bit(CGRAMColor >> 10);
		uint8_t g = to8Bit((CGRAMColor >> 5) & 0x1F);
		uint8_t r = to8Bit(CGRAMColor & 0x1F);

		return (0x000000FF | (r << 24) | (g << 16) | (b << 8));
	}
}