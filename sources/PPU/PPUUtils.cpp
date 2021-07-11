//
// Created by cbihan on 5/26/20.
//

#include "PPUUtils.hpp"

namespace ComSquare::PPU::Utils
{

	uint8_t To8Bit(int color)
	{
		return (uint)((color << 3) + (color >> 2));
	}

	uint32_t getRealColor(uint16_t cgramColor)
	{
		uint b = To8Bit(cgramColor >> 10);
		uint g = To8Bit((cgramColor >> 5) & 0x1F);
		uint r = To8Bit(cgramColor & 0x1F);

		return (0x000000FF | (r << 24) | (g << 16) | (b << 8));
	}
}