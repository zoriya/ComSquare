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
		//! @brief The size of the ram (in bytes).
		uint24_t _size;
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
		std::string getName() const override;

		//! @brief Get the component of this accessor (used for debug purpose)
		Component getComponent() const override;

		//! @brief Get the size of the ram in bytes.
		uint24_t getSize() const override;
	};
}

#endif //COMSQUARE_RAM_HPP
