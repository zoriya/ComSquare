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
		IMemory &_initial;
		//! @brief The number of banks to add to the memory before accessing it from the initial data.
		int _bankOffset = 0;
	public:
		//! @brief Create a shadow for the memory given as parameter.
		//! @param startBank The starting bank of the memory to shadow.
		//! @param endBank The ending bank of the memory to shadow
		//! @param startPage The starting page of the memory to shadow
		//! @param endPage The ending page of the memory to shadow
		RectangleShadow(IMemory &initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);
		//! @brief A rectangle shadow is copy constructable.
		RectangleShadow(const RectangleShadow &) = default;
		//! @brrief A rectangle shadow is not assignable
		RectangleShadow &operator=(const RectangleShadow &) = delete;
		//! @brief A default destructor.
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
		[[nodiscard]] uint24_t getRelativeAddress(uint24_t addr) const override;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const override;

		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const override;

		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const override;

		//! @brief Return the memory accessor this accessor mirror if any
		//! @return nullptr if isMirror is false, the source otherwise.
		[[nodiscard]] IMemory &getMirrored() const;

		//! @brief Set the number of bank this component do not shadow. Referring to the first byte of this component will refer to the first byte of the bank at (bankOffset + start of initial memory).
		void setBankOffset(int bankOffset);
	};
}