//
// Created by anonymus-raccoon on 5/26/20.
//

#pragma once

#include "Memory/MemoryBus.hpp"
#include "Models/Ints.hpp"
#include <cstdint>
#include <memory>

#ifdef DEBUGGER_ENABLED
#include "Debugger/RegisterViewer.hpp"
#endif

namespace ComSquare::CPU
{
	//! @brief Class handling all DMA/HDMA transfers (Direct Memory Access or H-Blank Direct Memory Access)
	class DMA
	{
	public:
		//! @brief The first three bytes of the DMA's control register. Used to tell how many bytes/registers there is.
		enum DMAMode
		{
			//! @brief 1 byte is transferred to 1 register (write once)
			OneToOne = 0b000,
			//! @brief 2 byte is transferred to 2 register (write once)
			TwoToTwo = 0b001,
			//! @brief 2 byte is transferred to 1 register (write twice)
			TwoToOne = 0b010,
			//! @brief 4 byte is transferred to 2 register (write twice)
			FourToTwo = 0b011,
			//! @brief 4 byte is transferred to 4 register (write once)
			FourToFour = 0b100,
			//! @brief Exactly the same as TwoToTwo (not implemented on the SNES so this fallbacks)
			TwoToTwoBis = 0b101,
			//! @brief Exactly the same as TwoToOne (not implemented on the SNES so this fallbacks)
			TwoToOneBis = 0b110,
			//! @brief Exactly the same as FourToTwo (not implemented on the SNES so this fallbacks)
			FourToTwoBis = 0b111
		};

		enum Direction
		{
			AtoB,
			BtoA
		};

	private:
		//! @brief Write one byte using the A address, the port and the _direction. Handle special cases where no write occurs.
		//! @return The number of cycles used.
		unsigned _writeOneByte(uint24_t aAddress, uint24_t bAddress);
		//! @brief Get an offset corresponding to the current DMAMode and the index of the currently transferred byte.
		[[nodiscard]] int _getModeOffset(int index) const;

		//! @brief DMA Control register (various information about the transfer)
		union
		{
			struct {
				//! @brief DMA's mode: how many bytes/registers there is, how many writes...
				DMAMode mode : 3;
				//! @brief If this flag is set, no increment/decrement will be done.
				bool fixed : 1;
				//! @brief if this flag is 0: increment. Else: decrement. (The A address)
				bool increment : 1;
				//! @brief Two unused bites.
				bool _ : 2;
				//! @brief The direction of the transfer.
				Direction direction : 1;
			};
			uint8_t raw;
		} _controlRegister {};

		//! @brief If this is 'xx', the register accessed will be $21xx.
		uint8_t _port {};

		//! @brief The absolute long address of the data from the A bus.
		union
		{
			uint8_t bytes[3];
			struct {
				uint16_t page;
				uint8_t bank;
			};
			uint24_t raw : 24;
		} _aAddress {};

		//! @brief The number of bytes to be transferred.
		union
		{
			uint8_t bytes[2];
			uint16_t raw;
		} _count {};

		//! @brief The memory bus to use for read/write.
		Memory::IMemoryBus &_bus;

	public:
		//! @brief Get the memory bus used by this CPU.
		[[nodiscard]] inline Memory::IMemoryBus &getBus()
		{
			return this->_bus;
		}
		//! @brief Set the memory bus used by this CPU
		//! @param bus The bus to use.
		void setBus(Memory::IMemoryBus &bus);

		//! @brief Is this channel set to run?
		bool enabled;

		//! @brief Bus helper to read from this channel.
		//! @param addr The address to read from
		//! @return The value at the given address.
		[[nodiscard]] uint8_t read(uint8_t addr) const;

		//! @brief Bus helper to write to this channel.
		//! @param addr The address to write to
		//! @param data The data to write.
		void write(uint8_t addr, uint8_t data);

		//! @brief Run the DMA for x cycles
		//! @param cycles The maximum number of cycles this DMA should run.
		//! @return the number of cycles taken
		unsigned run(unsigned cycles);

		//! @brief Create a DMA channel with a given bus
		//! @param bus The memory bus to use.
		explicit DMA(Memory::IMemoryBus &bus);
		//! @brief A DMA is copy constructable.
		DMA(const DMA &) = default;
		//! @brief A DMA is not assignable
		DMA &operator=(const DMA &) = delete;
		//! @brief A default destructor.
		~DMA() = default;

#ifdef DEBUGGER_ENABLED
		friend Debugger::RegisterViewer;
#endif
	};
}// namespace ComSquare::CPU
