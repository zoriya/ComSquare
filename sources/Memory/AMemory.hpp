//
// Created by anonymus-raccoon on 1/23/20.
//

#ifndef COMSQUARE_AMEMORY_HPP
#define COMSQUARE_AMEMORY_HPP


#include <cstdint>
#include <vector>
#include <memory>
#include "../Models/Int24.hpp"

namespace ComSquare::Memory
{
	//! @brief Common interface implemented by all components mapping memory.
	class AMemory {
	private:
		//! @brief The starting address mapped to this component.
		uint24_t _start = 0;
		//! @brief The last continuous address mapped to this components. For shadows, see the MemoryShadow class.
		uint24_t _end = 0;
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
		//! @brief Change starting and ending points of this mapped memory.
		//! @param start The first address mapped to this component.
		//! @param end The last address mapped to this component.
		//! @warning The start/end address should be a continuous range. You can't map address 0x0 and 0x2 but not 0x1. To do that, use two AMemory.
		void setMemoryRegion(uint24_t start, uint24_t end);
		//! @brief Return true if this component has mapped the address.
		//! @param addr The address to check.
		//! @return True if this address is mapped to the component. False otherwise.
		virtual bool hasMemoryAt(uint24_t addr);
		//! @brief Get the first address mapped to this component.
		//! @return the _start value.
		virtual uint24_t getStart();
		//! @brief Check if this memory is a mirror or not.
		//! @return True if this memory is a mirror. False otherwise.
		virtual bool isMirror();
		//! @brief Get the name of this accessor (used for debug purpose)
		virtual std::string getName() = 0;
		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		virtual std::string getValueName(uint24_t addr);
		//! @brief Return the memory accessor this accessor mirror if any
		//! @return nullptr if isMirror is false, the source otherwise.
		virtual std::shared_ptr<AMemory> getMirrored();
		virtual ~AMemory() = default;
	};
};


#endif //COMSQUARE_AMEMORY_HPP
