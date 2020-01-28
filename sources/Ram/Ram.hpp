//
// Created by anonymus-raccoon on 1/28/20.
//

#ifndef COMSQUARE_RAM_HPP
#define COMSQUARE_RAM_HPP

#include "../Memory/IMemory.hpp"

namespace ComSquare::Ram
{
	class Ram : public IMemory {
	private:
		//! @brief The ram. (Can be used for WRam, SRam, VRam etc)
		uint8_t *_data;
		//! @brief The size of the ram.
		size_t _size;
	public:
		//! @brief Load a rom from it's path.
		explicit Ram(size_t size);
		//! @brief Destructor that free the ram.
		~Ram();
		//! @brief Read from the ram.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of this ram.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		//! @return Return the data at the address.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the ram.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of this ram.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		void write(uint24_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_RAM_HPP
