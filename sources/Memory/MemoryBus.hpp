//
// Created by anonymus-raccoon on 1/23/20.
//

#pragma once

#include "AMemory.hpp"
#include "RectangleShadow.hpp"
#include "MemoryShadow.hpp"
#include "IMemoryBus.hpp"
#include <cstdint>
#include <memory>
#include <vector>

namespace ComSquare
{
	class SNES;

	namespace Memory
	{
		//! @brief The memory bus is the component responsible of mapping addresses to components address and transmitting the data.
		class MemoryBus : public IMemoryBus
		{
		private:
			//! @brief The list of components registered inside the bus. Every components that can read/write to a public address should be in this vector.
			std::vector<std::reference_wrapper<IMemory>> _memoryAccessors;

			//! @brief The list of simple memory shadows that are used to map duplicated zones of memory.
			std::vector<MemoryShadow> _shadows = {};
			//! @brief The list of rectangle memory shadows that are used to map duplicated zones of memory.
			std::vector<RectangleShadow> _rectangleShadows = {};

			//! @brief WRam, CPU, PPU & APU registers are mirrored to all banks of Q1 & Q3. This function is used for the mirroring.
			//! @param console All the components.
			//! @param i Base address for the mirrors.
			void _mirrorComponents(SNES &console, unsigned i);
		protected:
			//! @brief The last value read via the memory bus.
			uint8_t _openBus = 0;
		public:
			//! @brief Create a new default memory bus.
			MemoryBus() = default;
			//! @brief A memory bus is copyable.
			MemoryBus(const MemoryBus &) = default;
			//! @brief A memory bus is assignable.
			MemoryBus &operator=(const MemoryBus &) = default;
			//! @brief A default destructor
			~MemoryBus() override = default;

			//! @brief Read data at a global address. This form allow read to be silenced.
			//! @param addr The address to read from.
			//! @throws InvalidAddress If the address is not mapped to the bus, this exception is thrown.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			uint8_t read(uint24_t addr) override;

			//! @brief This as the same purpose as a read but it does not change the open bus and won't throw an exception.
			//! @param addr The address to read from.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			std::optional<uint8_t> peek(uint24_t addr) override;

			//! @brief This as the same purpose as a read but it does not change the open bus and won't throw an exception.
			//! @param addr The address to read from.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			//! @note If the value address is not mapped, 0 is returned instead of nullopt.
			uint8_t peek_v(uint24_t addr) override;

			//! @brief Write a data to a global address.
			//! @param addr The address to write to.
			//! @param data The data to write.
			void write(uint24_t addr, uint8_t data) override;

			//! @brief Map components to the address space using the currently loaded cartridge to set the right mapping mode.
			//! @param console All the components.
			void mapComponents(SNES &console);

			//! @brief Helper function to get the components that is responsible of read/write at an address.
			//! @param addr The address you want to look for.
			//! @return The components responsible for the address param or nullptr if none was found.
			IMemory *getAccessor(uint24_t addr) override;
		};
	}
}
