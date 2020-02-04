//
// Created by anonymus-raccoon on 2/4/20.
//

#ifndef COMSQUARE_RECTANGLESHADOW_HPP
#define COMSQUARE_RECTANGLESHADOW_HPP

#include <memory>
#include "IRectangleMemory.hpp"

namespace ComSquare::Memory
{
	class RectangleShadow : public IRectangleMemory {
	private:
		//! @brief Memory to shadow from.
		std::shared_ptr<IRectangleMemory> _initial;
	public:
		//! @brief Create a shadow for the memory given as parameter.
		explicit RectangleShadow(std::shared_ptr<IRectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);

		static std::shared_ptr<IMemory> createShadow(std::shared_ptr<IRectangleMemory> initial, uint8_t startBank, uint8_t endBank, uint16_t startPage, uint16_t endPage);
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
	};
}

#endif //COMSQUARE_RECTANGLESHADOW_HPP
