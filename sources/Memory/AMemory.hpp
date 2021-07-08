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
#include "IMemory.hpp"

namespace ComSquare::Memory
{
	//! @brief Abstract class representing a continuous block of memory.
	class AMemory : public IMemory {
	protected:
		//! @brief The starting address mapped to this component.
		uint24_t _start = 0;
		//! @brief The last continuous address mapped to this components. For shadows, see the MemoryShadow class.
		uint24_t _end = 0;
	public:
		//! @brief Translate an absolute address to a relative address
		//! @param addr The absolute address (in the 24 bit bus)
		//! @return The local address (0 refers to the first byte of this component).
		//! @throw InvalidAddress is thrown if the address is not mapped by this component.
		[[nodiscard]] uint24_t getRelativeAddress(uint24_t addr) const override;

		//! @brief Change starting and ending points of this mapped memory.
		//! @param start The first address mapped to this component.
		//! @param end The last address mapped to this component.
		//! @warning The start/end address should be a continuous range. You can't map address 0x0 and 0x2 but not 0x1. To do that, use two AMemory.
		void setMemoryRegion(uint24_t start, uint24_t end);

		//! @brief Return true if this component has mapped the address.
		//! @param addr The address to check.
		//! @return True if this address is mapped to the component. False otherwise.
		[[nodiscard]] bool hasMemoryAt(uint24_t addr) const override;

		//! @brief A default destructor
		~AMemory() override = default;
	};
}