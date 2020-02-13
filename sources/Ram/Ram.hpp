//
// Created by anonymus-raccoon on 1/28/20.
//

#ifndef COMSQUARE_RAM_HPP
#define COMSQUARE_RAM_HPP

#include <cstring>
#include "../Memory/IRectangleMemory.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	template<typename T>
	class Ram : public Memory::IRectangleMemory {
		private:
		//! @brief The ram. (Can be used for WRam, SRam, VRam etc)
		T *_data;
		//! @brief The size of the ram.
		size_t _size;
	public:
		//! @brief Create a ram of the given size (in bytes).
		explicit Ram(size_t size)
			: _size(size)
		{
			this->_data = new T[size];
			std::memset(this->_data, 0, size * sizeof(T));
		}
		//! @brief The ram can't be copied.
		Ram(const Ram &) = delete;
		//! @brief The ram can't be assigned.
		Ram &operator=(Ram &) = delete;
		//! @brief Destructor that free the ram.
		~Ram()
		{
			delete[] this->_data;
		}
		//! @brief Read from the ram.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of this ram.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		//! @return Return the data at the address.
		T read_internal(uint24_t addr) override
		{
			if (addr >= this->_size)
				throw InvalidAddress("Ram read", addr);
			return this->_data[addr];
		}
		//! @brief Write data to the ram.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of this ram.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the ram.
		void write_internal(uint24_t addr, T data) override
		{
			if (addr >= this->_size)
				throw InvalidAddress("Ram write", addr);
			this->_data[addr] = data;
		}
	};

	typedef Ram<uint8_t> BasicRam;
}

#endif //COMSQUARE_RAM_HPP
