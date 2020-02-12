//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_MEMORYBUS_HPP
#define COMSQUARE_MEMORYBUS_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include "IMemory.hpp"

namespace ComSquare
{
	struct SNES;

	namespace Memory
	{
		//! @brief The memory bus is the component responsible of mapping addresses to components address and transmitting the data.
		class MemoryBus {
		private:
			//! @brief The list of components registered inside the bus. Every components that can read/write to a public address should be in this vector.
			std::vector<std::shared_ptr<IMemory>> _memoryAccessors;

			//! @brief Helper function to get the components that is responsible of read/write at an address.
			//! @param addr The address you want to look for.
			//! @return The components responsible for the address param or nullptr if none was found.
			std::shared_ptr<IMemory> getAccessor(uint24_t addr);

			//! @brief The last value read via the memory bus.
			uint8_t _openBus = 0;

			//! @brief WRam, CPU, PPU & APU registers are mirrored to all banks of Q1 & Q3. This function is used for the mirroring.
			//! @param console All the components.
			//! @param i Base address for the mirrors.
			inline void _mirrorComponents(SNES &console, unsigned i);

		public:
			//! @brief Read data at a global address.
			//! @param addr The address to read from.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			uint8_t read(uint24_t addr);

			//! @brief Write a data to a global address.
			//! @param addr The address to write to.
			//! @param data The data to write.
			void write(uint24_t addr, uint8_t data);

			//! @brief Map components to the address space using the currently loaded cartridge to set the right mapping mode.
			//! @param console All the components.
			void mapComponents(SNES &console);
		};
	}
}


#endif //COMSQUARE_MEMORYBUS_HPP
