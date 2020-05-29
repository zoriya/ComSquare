//
// Created by anonymus-raccoon on 5/26/20.
//

#ifndef COMSQUARE_DMA_HPP
#define COMSQUARE_DMA_HPP

#include <cstdint>
#include <memory>
#include "../../Models/Int24.hpp"
#include "../../Memory/MemoryBus.hpp"
#include "../../Debugger/RegisterViewer.hpp"

namespace ComSquare::CPU
{
	//! @brief The first three bytes of the DMA's control register. Used to tell how many bytes/registers there is.
	enum DMAMode {
		//! @brief 1 byte is transferred to 1 register (write once)
		OneToOne = 0b000,
		//! @brief 2 byte is transferred to 2 register (write once)
		TwoToTwo = 0b001,
		//! @brief 2 byte is transferred to 1 register (write twice)
		TwoToOne = 0b010,
		//! @brief 4 byte is transferred to 2 register (write twice)
		FourToTwo = 0b011,
		//! @brief 4 byte is transferred to 4 register (write once)
		FourToFour = 0b100
	};

	enum Direction {
		AToB,
		BToA
	};

	//! @brief Class handling all DMA/HDMA transfers (Direct Memory Access or H-Blank Direct Memory Access)
	class DMA {
	private:
		//! @brief Write one byte using the A address, the port and the _direction. Handle special cases where no write occurs.
		//! @return The number of cycles used.
		unsigned _writeOneByte(uint24_t aAddress, uint24_t bAddress);
		//! @brief Get an offset corresponding to the current DMAMode and the index of the currently transferred byte.
		int _getModeOffset(int index);

		//! @brief DMA Control register (various information about the transfer)
		union {
			struct {
				//! @brief DMA's mode: how many bytes/registers there is, how many writes...
				DMAMode mode: 3;
				//! @brief If this flag is set, no increment/decrement will be done.
				bool fixed: 1;
				//! @brief if this flag is 0: increment. Else: decrement. (The A address)
				bool increment: 1;
				//! @brief Two unused bites.
				bool _: 2;
				//! @brief The direction of the transfer.
				Direction direction: 1;
			};
			uint8_t raw;
		} _controlRegister;
		//! @brief If this is 'xx', the register accessed will be $21xx.
		uint8_t _port;
		//! @brief The absolute long address of the data from the A bus.
		union {
			uint8_t bytes[3];
			struct {
				uint16_t page;
				uint8_t bank;
			};
			uint24_t raw: 24;
		} _aAddress;
		//! @brief The number of bytes to be transferred.
		union {
			uint8_t bytes[2];
			uint16_t raw;
		} _count;

		//! @brief The memory bus to use for read/write.
		std::shared_ptr<Memory::MemoryBus> _bus;

	public:
		//! @brief Is this channel set to run?
		bool enabled;

		//! @brief Set the memory bus used by this dma channel.
		void setBus(std::shared_ptr<Memory::MemoryBus> bus);

		//! @brief Bus helper to read from this channel.
		uint8_t read(uint8_t addr);
		//! @brief Bus helper to write to this channel.
		void write(uint8_t addr, uint8_t data);

		//! @brief Run the DMA for x cycles
		//! @param cycles The maximum number of cycles this DMA should run.
		//! @return the number of cycles taken
		uint8_t run(unsigned cycles);

		DMA() = default;
		DMA(std::shared_ptr<Memory::MemoryBus> bus);
		DMA(const DMA &) = default;
		DMA &operator=(const DMA &) = default;
		~DMA() = default;

	#ifdef DEBUGGER_ENABLED
		friend Debugger::RegisterViewer;
	#endif
	};
}

#endif //COMSQUARE_DMA_HPP
