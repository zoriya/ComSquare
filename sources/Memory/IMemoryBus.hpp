//
// Created by Zoe Roux on 2021-07-04.
//


#pragma once

#include <cinttypes>
#include <optional>
#include "IMemory.hpp"

namespace ComSquare
{
	class SNES;

	namespace Memory
	{
		//! @brief The memory bus is the component responsible of mapping addresses to components address and transmitting the data.
		class IMemoryBus
		{
		public:
			//! @brief A virtual default destructor.
			virtual ~IMemoryBus() = default;

			//! @brief Read data at a global address. This form allow read to be silenced.
			//! @param addr The address to read from.
			//! @throws InvalidAddress If the address is not mapped to the bus, this exception is thrown.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			virtual uint8_t read(uint24_t addr) = 0;

			//! @brief This as the same purpose as a read but it does not change the open bus and won't throw an exception.
			//! @param addr The address to read from.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			virtual std::optional<uint8_t> peek(uint24_t addr) = 0;

			//! @brief This as the same purpose as a read but it does not change the open bus and won't throw an exception.
			//! @param addr The address to read from.
			//! @return The value that the component returned for this address. If the address was mapped to ram, it simply returned the value. If the address was mapped to a register the component returned the register.
			//! @note If the value address is not mapped, 0 is returned instead of nullopt.
			virtual uint8_t peek_v(uint24_t addr) = 0;

			//! @brief Write a data to a global address.
			//! @param addr The address to write to.
			//! @param data The data to write.
			virtual void write(uint24_t addr, uint8_t data) = 0;

			//! @brief Helper function to get the components that is responsible of read/write at an address.
			//! @param addr The address you want to look for.
			//! @return The components responsible for the address param or nullptr if none was found.
			virtual IMemory *getAccessor(uint24_t addr) = 0;
		};
	}
}