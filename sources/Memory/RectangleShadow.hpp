//
// Created by anonymus-raccoon on 2/4/20.
//

#pragma once

#include <memory>
#include "ARectangleMemory.hpp"
#include "MemoryShadow.hpp"

namespace ComSquare::Memory
{
	class RectangleShadow : public ARectangleMemory {
	private:
		//! @brief Memory to shadow from.
		std::shared_ptr<IMemory> _initial;
		//! @brief The number of banks to add to the memory before accessing it from the initial data.
		uint8_t _bankOffset = 0;
	public:
		//! @brief Create a shadow for the memory given as parameter.
		explicit RectangleShadow(std::shared_ptr<IMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);
		RectangleShadow(const RectangleShadow &) = default;
		RectangleShadow &operator=(const RectangleShadow &) = default;
		~RectangleShadow() override = default;

		//! @brief Read from the initial AMemory given.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the initial AMemory.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the initial AMemory.
		//! @return Return the data at the address.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the ram.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of the initial AMemory.
		//! @param data The data to write.
		//! @throw InvalidAddress will be thrown if the address is more than the size of the initial AMemory.
		void write(uint24_t addr, uint8_t data) override;
		//! @brief Translate an absolute address to a relative address
		//! @param addr The absolute address (in the 24 bit bus)
		//! @return The local address (0 refers to the first byte of this component).
		//! @throw InvalidAddress is thrown if the address is not mapped by this component.
		uint24_t getRelativeAddress(uint24_t addr) override;
		//! @brief Check if this memory is a mirror or not.
		//! @return True if this memory is a mirror. False otherwise.
		bool isMirror() override;
		//! @brief Get the name of this accessor (used for debug purpose)
		std::string getName() override;
		//! @brief Get the component of this accessor (used for debug purpose)
		Component getComponent() override;
		//! @brief Return the memory accessor this accessor mirror if any
		//! @return nullptr if isMirror is false, the source otherwise.
		std::shared_ptr<IMemory> getMirrored() override;

		RectangleShadow *setBankOffset(uint8_t bankOffset);
	};
}