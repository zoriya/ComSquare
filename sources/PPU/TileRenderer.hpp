//
// Created by cbihan on 24/05/2021.
//

#pragma once

#include <array>
#include "Ram/Ram.hpp"

namespace ComSquare::PPU
{
	class TileRenderer {
	private:
		//! @brief ram to render
		std::shared_ptr<Ram::Ram> _ram;
		//! @brief cgram to access the colors
		std::shared_ptr<Ram::Ram> _cgram;
		//! @brief The bpp to use while rendering
		int _bpp;
		//! @brief The palette number to use while rendering
		int _paletteIndex;
	public:
		// todo background or ppu should have constexpr to explain it (16)
		//! @brief internal buffer max rendered tiles are 16x16
		std::array<std::array<uint32_t, 8>, 8> buffer;
		//! @brief Set the palette to use for render (index of palette)
		void setPaletteIndex(int paletteIndex);
		//! @brief Set the ram to look for color references
		void setCgram(std::shared_ptr<Ram::Ram> ram);
		//! @brief Set the bpp to render graphics
		void setBpp(int bpp);
		//! @brief The ram to render
		void setRam(std::shared_ptr<Ram::Ram> ram);
		//! @brief Get the current bpp
		int getBpp() const;
		//! @brief Get the index of the current palette used
		int getPaletteIndex() const;
		//! @brief Gives the reference of pixel using the tileAddress and the pixelIndex
		//! @note This function is wrapper of getPixelReferenceFromTileRow
		uint8_t getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex);
		//! @brief get the pixel color reference to use in the selected palette
		uint8_t getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex);
		//! @brief Gives the actual selected palette with all of it's colors
		//! @warning Values are CGRAM colors use PPU::getRealColor function to get the actual real color
		std::vector<uint16_t> getPalette(int nbPalette);
		//! @brief read the 2bpp value for a pixel (used multple times for 4bpp and 8bpp)
		uint8_t read2BPPValue(uint16_t tileRowAddress, uint8_t pixelIndex);
		//! @brief render the tile (8x8) at the tileAddress
		//! @param tileAddress The address of the tile to render
		void render(uint16_t tileAddress);
		TileRenderer();
		TileRenderer(const TileRenderer &) = default;
		~TileRenderer() = default;
		TileRenderer &operator=(const TileRenderer &) = default;
	};
}