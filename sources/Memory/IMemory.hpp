//
// Created by anonymus-raccoon on 1/23/20.
//

#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include "Models/Ints.hpp"
#include "Models/Components.hpp"

namespace ComSquare::Memory
{
	//! @brief Common interface implemented by all components mapping memory.
	class IMemory
	{
	public:
		//! @brief Read data from the component.
		//! @param addr The local address to read from (0x0 should refer to the first byte of this component).
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		//! @return Return the data at the address given as parameter.
		virtual uint8_t read(uint24_t addr) = 0;

		//! @brief Write data to this component.
		//! @param addr The local address to write data (0x0 should refer to the first byte of this component).
		//! @param data The new data to write.
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		virtual void write(uint24_t addr, uint8_t data) = 0;

		//! @brief Return true if this component has mapped the address.
		//! @param addr The address to check.
		//! @return True if this address is mapped to the component. False otherwise.
		virtual bool hasMemoryAt(uint24_t addr) const = 0;

		//! @brief Translate an absolute address to a relative address
		//! @param addr The absolute address (in the 24 bit bus)
		//! @return The local address (0 refers to the first byte of this component).
		//! @throw InvalidAddress is thrown if the address is not mapped by this component.
		virtual uint24_t getRelativeAddress(uint24_t addr) const = 0;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		virtual uint24_t getSize() const = 0;

		//! @brief Get the name of this accessor (used for debug purpose)
		virtual std::string getName() const = 0;
		//! @brief Get the component of this accessor (used for debug purpose)
		virtual Component getComponent() const = 0;

		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		virtual std::string getValueName(uint24_t addr) const = 0;

		//! @brief A virtual default destructor.
		virtual ~IMemory() = default;
	};
};