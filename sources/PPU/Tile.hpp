//
// Created by cbihan on 25/05/2021.
//

#pragma once

namespace ComSquare::PPU
{
	//! @brief Info on tile struct
	struct Tile
	{
		//! @brief The number of pixel a base tile can have in width
		static constexpr int NbPixelsWidth = 8;
		//! @brief The number of pixel a base tile can have in height
		static constexpr int NbPixelsHeight = 8;
		//! @brief A base tile size in byte is 0x8
		//! @note Used like: bpp * BaseByteSize to get the byte size of 1 tile
		static constexpr int BaseByteSize = 0x8;
	};

}