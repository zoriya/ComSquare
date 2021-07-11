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
		//! @brief The byte size offset for 1 row in VRAM
		static constexpr int TileByteSizeRow = 16;

		//! @brief ram to render
		Ram::Ram &_ram;
		//! @brief cgram to access the colors
		Ram::Ram &_cgram;
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
		//! @brief Set the bpp to render graphics
		void setBpp(int bpp);
		//! @brief Get the current bpp
		[[nodiscard]] int getBpp() const;
		//! @brief Get the index of the current palette used
		[[nodiscard]] int getPaletteIndex() const;
		//! @brief Get the color pixel reference from the tile address and pixelIndex
		//! @param tileAddress The starting address of the tile
		//! @param pixelIndex The index of the pixel (0 - 255)
		//! @return The color reference
		//! @note This function is wrapper of getPixelReferenceFromTileRow
		uint8_t getPixelReferenceFromTile(uint16_t tileAddress, uint8_t pixelIndex);
		//! @brief Get the color reference of a pixel from the address of the row
		//! @param tileRowAddress The address of the line of pixel
		//! @param pixelIndex The index of the pixel (0 - 7)
		//! @return The color Reference
		uint8_t getPixelReferenceFromTileRow(uint16_t tileRowAddress, uint8_t pixelIndex);
		//! @brief Gives the actual selected palette with all of it's colors
		//! @param nbPalette The index of the palette wanted
		//! @return The array of color of the palette
		//! @note The return and argument depends on the current bpp
		//! @warning Values are CGRAM colors use PPU::CGRAMColorToRGBA function to get the actual real color
		std::vector<uint16_t> getPalette(int nbPalette);
		//! @brief read the 2bpp value for a pixel (used multple times for 4bpp and 8bpp)
		//! @param tileRowAddress Address where the read is done. Usage: Address of the tile row to render
		//! @param pixelIndex The offset form tileRowAddress, Usage: the pixel to read
		uint8_t read2BPPValue(uint16_t tileRowAddress, uint8_t pixelIndex);
		//! @brief render the tile (8x8) at the tileAddress
		//! @param tileAddress The address of the tile to render
		void render(uint16_t tileAddress);

		TileRenderer(Ram::Ram &vram, Ram::Ram &cgram);
		//! @brief A tile renderer is copy constructable.
		TileRenderer(const TileRenderer &) = default;
		//! @brief A default destructor
		~TileRenderer() = default;
		//! @brief A tile render is not assignable.
		TileRenderer &operator=(const TileRenderer &) = delete;
	};
}