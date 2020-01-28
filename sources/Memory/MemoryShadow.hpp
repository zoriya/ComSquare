//
// Created by anonymus-raccoon on 1/28/20.
//

#ifndef COMSQUARE_MEMORYSHADOW_HPP
#define COMSQUARE_MEMORYSHADOW_HPP

#include <memory>
#include "IMemory.hpp"

namespace ComSquare::Memory
{
	class MemoryShadow : public IMemory {
	private:
		//! @brief Memory to shadow from.
		std::shared_ptr<IMemory> _initial;
	public:
		//! @brief Create a shadow for the memory given as parameter.
		explicit MemoryShadow(std::shared_ptr<IMemory> initial, uint24_t start, uint24_t end);

		static std::shared_ptr<IMemory> createShadow(std::shared_ptr<IMemory> initial, uint24_t start, uint24_t end);
		//! @brief Read from the initial IMemory given.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the initial IMemory.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the initial IMemory.
		//! @return Return the data at the address.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the ram.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of the initial IMemory.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the initial IMemory.
		void write(uint24_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_MEMORYSHADOW_HPP
