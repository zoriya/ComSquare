//
// Created by cbihan on 5/26/20.
//

#include <stdint-gcc.h>

uint32_t getRealColor(uint16_t color)
{
	uint8_t blue;
	uint8_t red;
	uint8_t green;
	uint32_t pixelTmp;

	blue = (color & 0x7D00U) >> 10U;
	green = (color & 0x03E0U) >> 5U;
	red = (color & 0x001FU);

	pixelTmp = 0xFF;
	pixelTmp += (red * 255U / 31U) << 24U;
	pixelTmp += (green * 255U / 31U) << 16U;
	pixelTmp += (blue * 255U / 31U) << 8U;
	return pixelTmp;
}