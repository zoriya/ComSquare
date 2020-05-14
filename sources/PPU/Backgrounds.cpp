//
// Created by cbihan on 5/14/20.
//

#include <iostream>
#include <bitset>
#include "PPU.hpp"
#include "PPUUtils.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Ram/Ram.hpp"
#include "../Models/Vector2.hpp"

namespace ComSquare::PPU
{
	void PPU::renderBackground(int bgNumber, Vector2<int> characterSize, int bpp, bool priority)
	{
		int nbBgHeight = (this->_registers._bgsc[bgNumber - 1].tilemapVerticalMirroring) ? 2 : 1;
		int nbBgWidth = (this->_registers._bgsc[bgNumber - 1].tilemapHorizontalMirroring) ? 2 : 1;
		uint16_t vramAddress = this->_registers._bgsc[bgNumber - 1].tilemapAddress << 1U;
		Vector2<int> offset(0, 0);

		for (int i = 0; i < 4; i++) {
			if (!(i == 1 && nbBgWidth == 1) && !(i > 1 && nbBgHeight == 1)) {
				drawBasicTileMap(vramAddress, bgNumber, bpp, characterSize, offset);
			}
			vramAddress+= 0x800;
			offset.x += 32 * characterSize.x;
			if (i == 2) {
				offset.x = 0;
				offset.y += 32 * characterSize.y;
			}
		}
	}

	uint16_t PPU::getGraphicVramAddress(int x, int y, int bg, int bpp)
	{
		uint16_t baseAddress = this->_registers._bgnba[bg > 2].raw;
		int step = bpp * 8;

		baseAddress = (bg % 2) ? baseAddress & 0xFU : (baseAddress & 0xFU) >> 4U;
		baseAddress = baseAddress << 12U;
		return baseAddress + (x * 16 * step) + (y * step);
	}

	void PPU::drawBgTile(uint16_t data, Vector2<int> pos, int bg, int bpp, Vector2<int> characterSize)
	{
		uint16_t graphicAddress;
		union TileMapData tileData;
		std::vector<uint16_t> palette;
		int index = 0;
		uint8_t reference = 0;
		uint32_t color = 0;

		tileData.raw = data;
		graphicAddress = this->getGraphicVramAddress(tileData.posX, tileData.posY, bg, bpp);
		for (int i = 0; i < characterSize.y; i++) {
			for (int j = 0; j < characterSize.x; j++) {
				palette = getPalette(tileData.palette);
				reference = getTilePixelReference(graphicAddress, bpp, index);
				color = getRealColor(palette[reference]);
				this->_renderer.putPixel(pos.x, pos.y, color);
				index++;
				pos.x++;
				if (index == (8 / bpp) - 1) {
					index = 0;
					graphicAddress++;
				}
			}
			index = 0;
			pos.x -= characterSize.x;
			pos.y++;
		}
	}

	std::vector<uint16_t> PPU::getPalette(int nbPalette)
	{
		std::vector<uint16_t> palette(0xF);

		uint16_t addr = nbPalette * 0x10;
		for (int i = 0; i < 0xF; i++) {
			palette[i] = this->cgramRead(addr);
			palette[i] += this->cgramRead(addr + 1) << 8U;
		}
		return palette;
	}

	uint32_t PPU::getRealColor(uint16_t color)
	{
		uint8_t blue;
		uint8_t red;
		uint8_t green;
		uint32_t pixelTmp;

		blue = (color & 0x7D00U) >> 10U;
		green = (color & 0x03E0U) >> 5U;
		red = (color & 0x001FU);

		pixelTmp = this->_registers._inidisp.brightness * 255U / 15U;
		pixelTmp += (red * 255U / 31U) << 24U;
		pixelTmp += (green * 255U / 31U) << 16U;
		pixelTmp += (blue * 255U / 31U) << 8U;
		return pixelTmp;
	}

	uint8_t PPU::getTilePixelReference(uint16_t addr, int bpp, int nb)
	{
		uint8_t reference = this->vram->read_internal(addr);

		switch (bpp) {
		case 8:
			return reference;
		case 4:
			return (reference & (0xFU << ((1 - nb) * 4U))) >> (1 - nb) * 4U;
		case 2:
			return (reference & (0x3U << ((3 - nb) * 2U))) >> (3 - nb) * 2U;
		default:
			break;
		}
		return 0;
	}

	void PPU::drawBasicTileMap(uint16_t baseAddress, int bgNumber, int bpp, Vector2<int> characterSize, Vector2<int> offset)
	{
		uint16_t tileMapValue = 0;
		Vector2<int> pos(0,0);
		uint16_t vramAddress = baseAddress;

		while (vramAddress < 0x800 + baseAddress) {
			tileMapValue = this->vram->read_internal(vramAddress);
			tileMapValue += this->vram->read_internal(vramAddress + 1) << 8U;
			vramAddress += 2;
			drawBgTile(tileMapValue, {(pos.x * characterSize.x) + offset.x, (pos.y * characterSize.y) + offset.y}, bgNumber, bpp, characterSize);
			if (pos.x % 31 == 0 && pos.x) {
				pos.y++;
				pos.x = 0;
			}
			else
				pos.x++;
		}
	}
}