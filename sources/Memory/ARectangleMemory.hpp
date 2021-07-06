//
// Created by anonymus-raccoon on 1/29/20.
//

#pragma once

#include "IMemory.hpp"
#include "Models/Ints.hpp"

namespace ComSquare::Memory
{
	//! @brief Base memory class to map non continuous rectangle to the memory. (A rectangle that spam across more than one bank but that does not start at $0000 or end at $FFFF).
	class ARectangleMemory : public IMemory
	{
	protected:
		//! @brief The first bank to map to.
		uint8_t _startBank = 0;
		//! @brief The last bank to map to.
		uint8_t _endBank = 0;
		//! @brief The first address of each bank to map.
		uint16_t _startPage = 0;
		//! @brief The last address of each bank to map.
		uint16_t _endPage = 0;

	public:
		//! @brief Translate an absolute address to a relative address
		//! @param addr The absolute address (in the 24 bit bus)
		//! @return The local address (0 refers to the first byte of this component).
		//! @throw InvalidAddress is thrown if the address is not mapped by this component.
		[[nodiscard]] uint24_t getRelativeAddress(uint24_t addr) const override;

		//! @brief Return true if this component has mapped the address.
		//! @param addr The address to check.
		//! @return True if this address is mapped to the component. False otherwise.
		[[nodiscard]] bool hasMemoryAt(uint24_t addr) const override;

		//! @brief Change starting and ending points of this mapped memory.
		//! @param startBank The first bank mapped to this component.
		//! @param endBank The last bank mapped to this component.
		//! @param startPage The first page mapped to this component (every mapped banks will have this page mapped)
		//! @param endPage The end page mapped to this component (every mapped banks will have this pages lower than this mapped)
		//! @warning The start/end address should be a rectangle. To mirror memory, use the MemoryShadow class and not this one.
		void setMemoryRegion(uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);

		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		[[nodiscard]] std::string getValueName(uint24_t addr) const override;

		//! @brief A default destructor.
		~ARectangleMemory() override = default;
	};
}// namespace ComSquare::Memory