//
// Created by cbihan on 5/14/20.
//

#include "Background.hpp"

namespace PPU
{
	/*void PPU::renderBackground(int bgNumber, Vector2<int> characterSize, int bpp, bool priority)
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
	}*/
}