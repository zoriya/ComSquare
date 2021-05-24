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
	};

}