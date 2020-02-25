//
// Created by anonymus-raccoon on 2/4/20.
//

#ifndef COMSQUARE_RECTANGLESHADOW_HPP
#define COMSQUARE_RECTANGLESHADOW_HPP

#include <memory>
#include "IRectangleMemory.hpp"
#include "MemoryShadow.hpp"

namespace ComSquare::Memory
{
	class RectangleShadow : public IRectangleMemory {
	private:
		//! @brief Memory to shadow from.
		std::shared_ptr<IRectangleMemory> _initial;
		//! @brief The number of banks to add to the memory before accessing it from the initial data.
		uint8_t _bankOffset = 0;
	public:
		//! @brief Create a shadow for the memory given as parameter.
		explicit RectangleShadow(std::shared_ptr<IRectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);
		RectangleShadow(const RectangleShadow &) = default;
		RectangleShadow &operator=(const RectangleShadow &) = default;
		~RectangleShadow() = default;

		//! @brief Internal component read. Implement this as you would implement a basic IMemory's read.
		//! @param addr The local address to read from. 0x0 refer to the first byte of your data and the address is in the component's space. That means that you can consider this address as continuous
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		//! @return Return the data at the address given as parameter.
		uint8_t read_internal(uint24_t addr) override;
		//! @brief Internal component write. Implement this as you would implement a basic IMemory's write.
		//! @param addr The local address to write to. 0x0 refer to the first byte of your data and the address is in the component's space. That means that you can consider this address as continuous
		//! @param data The new data to write.
		//! @throw This function should thrown an InvalidAddress for address that are not mapped to the component.
		void write_internal(uint24_t addr, uint8_t data) override;
		//! @brief Check if this memory is a mirror or not.
		//! @return True if this memory is a mirror. False otherwise.
		bool isMirror() override;
		//! @brief Return the memory accessor this accessor mirror if any
		//! @return nullptr if isMirror is false, the source otherwise.
		std::shared_ptr<IMemory> getMirrored() override;

		RectangleShadow *setBankOffset(uint8_t bankOffset);
	};
}

#endif //COMSQUARE_RECTANGLESHADOW_HPP
