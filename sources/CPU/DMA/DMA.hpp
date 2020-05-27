//
// Created by anonymus-raccoon on 5/26/20.
//

#ifndef COMSQUARE_DMA_HPP
#define COMSQUARE_DMA_HPP

#include <cstdint>
#include "../../Models/Int24.hpp"

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

	//! @brief Class handling all DMA/HDMA transfers (Direct Memory Access or H-Blank Direct Memory Access)
	class DMA {
	public:
		//! @brief DMA Control register (various information about the transfer)
		union {
			struct {
				//! @brief The direction of the transfer (0: CPU to PPU aka A to B, 1: PPU to CPU aka B to A).
				bool direction: 1;
				//! @brief Two unused bites.
				bool _: 2;
				//! @brief if this flag is 0: increment. Else: decrement. (The A address)
				bool increment: 1;
				//! @brief If this flag is set, no increment/decrement will be done.
				bool fixed: 1;
				//! @brief DMA's mode: how many bytes/registers there is, how many writes...
				enum DMAMode mode: 3;
			};
			uint8_t raw;
		} controlRegister;
		//! @brief If this is 'xx', the register accessed will be $21xx.
		uint8_t port;
		//! @brief The absolute long address of the data from the A bus.
		union {
			uint8_t bytes[3];
			uint24_t raw: 24;
		} aAddress;
		//! @brief The number of bytes to be transferred.
		union {
			uint8_t bytes[2];
			uint16_t raw;
		} count;

		//! @brief Bus helper to read from this channel.
		uint8_t read(uint8_t addr);
		//! @brief Bus helper to write to this channel.
		void write(uint8_t addr, uint8_t data);

		//! @brief Run the DMA for x cycles
		//! @param cycles The maximum number of cycles this DMA should run.
		//! @return the number of cycles taken
		uint8_t run(unsigned cycles);

		DMA() = default;
		DMA(const DMA &) = default;
		DMA &operator=(const DMA &) = default;
		~DMA() = default;
	};
}

#endif //COMSQUARE_DMA_HPP
