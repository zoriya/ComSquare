//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_CARTRIDGE_HPP
#define COMSQUARE_CARTRIDGE_HPP

#include <string>
#include "../Memory/IMemory.hpp"
#include "../Models/Ints.hpp"

namespace ComSquare::Cartridge
{
	//! @brief Contains the rom's memory/instructions.
	class Cartridge : IMemory {
	private:
		//! @brief The rom data (contains all the instructions).
		uint8_t *_data;
		//! @brief The size of the rom data.
		size_t _size;
		//! @brief Get the size of a rom from it's path.
		//! @param romPath The path of the rom to get info from.
		//! @return The size of the rom.
		static size_t getRomSize(const std::string &romPath);
	public:
		//! @brief Load a rom from it's path.
		explicit Cartridge(const std::string &romPath);
		//! @brief Destructor that free the cartridge data.
		~Cartridge();
		//! @brief Read from the rom.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the rom's memory.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the rom's memory.
		//! @return Return the data at the address.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the rom.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of the rom's memory.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the rom's memory.
		void write(uint24_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_CARTRIDGE_HPP
