//
// Created by Melefo on 27/02/2020.
//

#pragma once

#include "Memory/AMemory.hpp"

namespace ComSquare::APU::IPL
{
	class IPL : public Memory::AMemory {
	protected:
		//! @brief The Rom.
		std::array<uint8_t, 64> _data = {
			0xCD, 0xEF, 0xBD, 0xE8, 0x00, 0xC6, 0x1D, 0xD0,
			0xFC, 0x8F, 0xAA, 0xF4, 0x8F, 0xBB, 0xF5, 0x78,
			0xCC, 0xF4, 0xD0, 0xFB, 0x2F, 0x19, 0xEB, 0xF4,
			0xD0, 0xFC, 0x7E, 0xF4, 0xD0, 0x0B, 0xE4, 0xF5,
			0xCB, 0xF4, 0xD7, 0x00, 0xFC, 0xD0, 0xF3, 0xAB,
			0x01, 0x10, 0xEF, 0x7E, 0xF4, 0x10, 0xEB, 0xBA,
			0xF6, 0xDA, 0x00, 0xBA, 0xF4, 0xC4, 0xF4, 0xDD,
			0x5D, 0xD0, 0xDB, 0x1F, 0x00, 0x00, 0xC0, 0xFF
		};
		//! @brief The size of the IPL rom (in bytes).
		size_t _size = 64;
		//! @brief An id identifying the type of memory this is (for the debugger)
		Component _iplType;
		//! @brief The name of this ram.
		std::string _iplName;
	public:
		//! @brief Create the rom with its value.
		explicit IPL(Component, std::string iplName);
		//! @brief The rom can't be copied.
		IPL(const IPL &) = delete;
		//! @brief The rom can't be assigned.
		IPL &operator=(IPL &) = delete;
		//! @brief A default destructor
		~IPL() override = default;

		//! @brief Read data from the component using the same method as the basic IMemory.
		//! @param addr The global 24 bits address. This method is responsible of mapping to the component's read.
		//! @throw InvalidAddress if the address is not mapped to the component.
		//! @return Return the data at the address given as parameter.
		uint8_t read(uint24_t addr) override;

		//! @brief Write data to this component using the same method as the basic IMemory.
		//! @param addr The global 24 bits address. This method is responsible of mapping to the component's write.
		//! @param data The new data to write.
		//! @throw InvalidAddress if the address is not mapped to the component.
		void write(uint24_t addr, uint8_t data) override;

		//! @brief Retrieve the data at the address given. This can be used instead of read or write.
		//! @param addr The address of the data to retrieve.
		//! @return The data at the address given as parameter.
		uint8_t &operator[](uint24_t addr);
		//! @brief Retrieve the data at the address given. This can be used instead of read or write.
		//! @param addr The address of the data to retrieve.
		//! @return The data at the address given as parameter.
		const uint8_t &operator[](uint24_t addr) const;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const override;

		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const override;

		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const override;

		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		[[nodiscard]] std::string getValueName(uint24_t addr) const override;
	};
}