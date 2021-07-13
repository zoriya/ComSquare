//
// Created by cbihan on 1/27/20.
//

#pragma once

#include <cstdint>
#include "Memory/AMemory.hpp"
#include "Memory/MemoryBus.hpp"
#include "Renderer/IRenderer.hpp"
#include "Ram/Ram.hpp"
#include "Models/Vector2.hpp"
#include <algorithm>
#include "Background.hpp"
#include "PPU/PPUUtils.hpp"
#include "PPU/PPURegisters.hpp"

#ifdef DEBUGGER_ENABLED
#include "Debugger/TileViewer/RAMTileRenderer.hpp"
#endif

#define FALLTHROUGH __attribute__((fallthrough));

namespace ComSquare::PPU::Utils
{
	struct PpuState;
}

namespace ComSquare::PPU
{
	static constexpr uint32_t VramSize = 65536;
	static constexpr uint32_t CGRamSize = 512;
	static constexpr uint32_t OAMRamSize = 544;


	class Background;
	//! @brief Enum to access more easily the ppu background array
	enum BgName {
		Background1 = 0,
		Background2,
		Background3,
		Background4
	};

	//! @brief The class containing all the registers of the PPU
	class PPU : public Memory::AMemory {
	public:
		//! @brief Rams
		Ram::Ram vram;
		Ram::Ram oamram;
		Ram::Ram cgram;
	private:
		//! @brief Init ppuRegisters
		Registers _registers{};
		Renderer::IRenderer &_renderer;
		//! @brief Backgrounds buffers
		Background _backgrounds[4];
		//! @brief Main Screen buffer
		std::array<std::array<uint32_t, 1024>, 1024> _mainScreen;
		std::array<std::array<uint8_t, 1024>, 1024> _mainScreenLevelMap;
		//! @brief Sub Screen buffer
		std::array<std::array<uint8_t, 1024>, 1024> _subScreenLevelMap;
		std::array<std::array<uint32_t, 1024>, 1024> _subScreen;
		//! @brief Final Screen buffer
		std::array<std::array<uint32_t, 1024>, 1024> _screen;
		//! @brief Used for vram read registers (0x2139 - 0x213A)
		uint16_t _vramReadBuffer = 0;
		//! @brief Struct that contain all necessary vars for the use of the registers
		struct Utils::PpuState _ppuState;

	public:

		explicit PPU(Renderer::IRenderer &renderer);
		PPU(const PPU &) = delete;
		~PPU() override = default;
		PPU &operator=(const PPU &) = delete;

		//! @brief Read data from the component.
		//! @param addr The local address to read from (0x0 should refer to the first byte of this component).
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		//! @return Return the data at the address given as parameter.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to this component.
		//! @param addr The local address to write data (0x0 should refer to the first byte of this component).
		//! @param data The new data to write.
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		void write(uint24_t addr, uint8_t data) override;
		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const override;
		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const override;
		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const override;

		//! @brief Update the PPU of n cycles.
		//! @param The number of cycles to update.
		virtual void update(unsigned cycles);
		//! @brief Give the Vram Address with the right Address remapping
		[[nodiscard]] uint16_t getVramAddress() const;
		//! @brief Give the name of the Address register (used for debug)
		[[nodiscard]] std::string getValueName(uint24_t addr) const override;
		//! @brief Allow others components to read the CGRAM
		uint16_t cgramRead(uint16_t addr);
		//! @brief get the bpp depending of the bgNumber and the Bgmode
		[[nodiscard]] int getBPP(int bgNumber) const;
		//! @brief Give the correct character size depending of the bgMode
		[[nodiscard]] Vector2<int> getCharacterSize(int bgNumber) const;
		//! @brief Give the address where the tilemap starts
		[[nodiscard]] uint16_t getTileMapStartAddress(int bgNumber) const;
		//! @brief Give the address to find the correct tileset for a given x and y
		[[nodiscard]] uint16_t getTilesetAddress(int bgNumber) const;
		//! @brief Tells if the tilemap is expanded for the x and y directions
		[[nodiscard]] Vector2<bool> getBackgroundMirroring(int bgNumber) const;
		//! @brief Render the Main and sub screen correctly
		void renderMainAndSubScreen();
		//! @brief Add a bg to the sub and/or main screen
		template<int levelLow, int levelHigh>
		void addToMainSubScreen(Background &bg)
		{
			if (this->_registers._t[0].raw & (1U << (bg.getBgNumber() - 1U))) {
				Background::mergeBackgroundBuffer<levelLow, levelHigh>(this->_mainScreen, this->_mainScreenLevelMap, bg);
			}
			if (this->_registers._t[1].raw & (1U << (bg.getBgNumber() - 1U))) {
				Background::mergeBackgroundBuffer<levelLow, levelHigh>(this->_subScreen, this->_subScreenLevelMap, bg);
			}
		}
		//! @brief Get the current background Mode
		[[nodiscard]] int getBgMode() const;
		//! @brief update the Vram buffer
		void updateVramReadBuffer();
		//! @brief update the Vram buffer
		[[nodiscard]] Vector2<int> getBgScroll(int bgNumber) const;
		//! @brief Allow to look the value of each write register (used by Register debugger)
		[[nodiscard]] const Registers &getWriteRegisters() const;
	};
}
