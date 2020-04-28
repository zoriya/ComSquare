//
// Created by anonymus-raccoon on 1/29/20.
//

#ifndef COMSQUARE_ARECTANGLEMEMORY_HPP
#define COMSQUARE_ARECTANGLEMEMORY_HPP


#include "AMemory.hpp"

namespace ComSquare::Memory
{
	//! @brief Superset of the AMemory to map non continuous rectangle to the memory. (A rectangle that spam across more than one bank but that does not start at 0000 or end at FFFF).
	class ARectangleMemory : public AMemory {
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
		//! @brief Read data from the component using the same method as the basic AMemory.
		//! @param addr The global 24 bits address. This method is responsible of mapping to the component's read.
		//! @throw InvalidAddress if the address is not mapped to the component.
		//! @return Return the data at the address given as parameter.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to this component using the same method as the basic AMemory.
		//! @param addr The global 24 bits address. This method is responsible of mapping to the component's write.
		//! @param data The new data to write.
		//! @throw InvalidAddress if the address is not mapped to the component.
		void write(uint24_t addr, uint8_t data) override;
		//! @brief Internal component read. Implement this as you would implement a basic AMemory's read.
		//! @param addr The local address to read from. 0x0 refer to the first byte of your data and the address is in the component's space. That means that you can consider this address as continuous
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		//! @return Return the data at the address given as parameter.
		virtual uint8_t read_internal(uint24_t addr) = 0;
		//! @brief Internal component write. Implement this as you would implement a basic AMemory's write.
		//! @param addr The local address to write to. 0x0 refer to the first byte of your data and the address is in the component's space. That means that you can consider this address as continuous
		//! @param data The new data to write.
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		virtual void write_internal(uint24_t addr, uint8_t data) = 0;
		//! @brief Return true if this component has mapped the address.
		//! @param addr The address to check.
		//! @return True if this address is mapped to the component. False otherwise.
		bool hasMemoryAt(uint24_t addr) override;
		//! @brief Get the first address mapped to this component.
		//! @return the _start value.
		uint24_t getStart() override;
		//! @brief Change starting and ending points of this mapped memory.
		//! @param startBank The first bank mapped to this component.
		//! @param endBank The last bank mapped to this component.
		//! @param startPage The first page mapped to this component (every mapped banks will have this page mapped)
		//! @param endPage The end page mapped to this component (every mapped banks will have this pages lower than this mapped)
		//! @warning The start/end address should be a rectangle. To mirror memory, use the MemoryShadow class and not this one.
		void setMemoryRegion(uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);
	};
}

#endif //COMSQUARE_ARECTANGLEMEMORY_HPP
