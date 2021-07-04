//
// Created by anonymus-raccoon on 1/31/20.
//

#pragma once

#include <cinttypes>

namespace ComSquare::Cartridge
{
	struct InterruptVectors
	{
		//! @brief The Co-Processor enable vector.
		union
		{
			uint8_t cop8[2];
			uint16_t cop;
		};
		//! @brief The Break vector.
		union
		{
			uint8_t brk8[2];
			uint16_t brk;
		};
		//! @brief The Abort vector.
		union
		{
			uint8_t abort8[2];
			uint16_t abort;
		};
		//! @brief The non-maskable interrupt (The V-Blank interrupt).
		union
		{
			uint8_t nmi8[2];
			uint16_t nmi;
		};
		//! @brief The Reset vector (execution of the SNES starts with this reset vector in emulation mode).
		union
		{
			uint8_t reset8[2];
			uint16_t reset;
		};
		//! @brief The Interrupt Request vector.
		union
		{
			uint8_t irq8[2];
			uint16_t irq;
		};
	};
}// namespace ComSquare::Cartridge