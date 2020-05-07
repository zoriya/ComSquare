//
// Created by anonymus-raccoon on 1/28/20.
//

#ifndef COMSQUARE_RAM_HPP
#define COMSQUARE_RAM_HPP

#include "../Memory/ARectangleMemory.hpp"
#include <string>

namespace ComSquare::Ram
{
	class Ram : public Memory::ARectangleMemory {
	protected:
		//! @brief The ram. (Can be used for WRam, SRam, VRam etc)
		uint8_t *_data;
		//! @brief The size of the ram (iny bytes).
		size_t _size;
		//! @brief An id identifying the type of memory this is (for the debugger)
		Component _ramType;
		//! @brief The name of this ram.
		std::string _ramName;
	public:
		//! @brief Create a ram of a given size in bytes.
		explicit Ram(size_t size, Component, std::string ramName);
		//! @brief The ram can't be copied.
		Ram(const Ram &) = delete;
		//! @brief The ram can't be assigned.
		Ram &operator=(Ram &) = delete;
		//! @brief Destructor that free the ram.
		~Ram() override;
		//! @brief Read from the ram.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of this ram.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		//! @return Return the data at the address.
		uint8_t read_internal(uint24_t addr) override;
		//! @brief Write data to the ram.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of this ram.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		void write_internal(uint24_t addr, uint8_t data) override;

		//! @brief replace values between two addresses with a value
		//! @param start start address to replace
		//! @param end end address to replace
		//! @param value replace value
		void memset(uint24_t start, uint24_t end, uint8_t value);

		//! @brief Get the name of this accessor (used for debug purpose)
		std::string getName() override;

		//! @brief Get the component of this accessor (used for debug purpose)
		Component getComponent() override;

		//! @brief Get the size of the ram in bytes.
		size_t getSize();
	};
}

#endif //COMSQUARE_RAM_HPP
