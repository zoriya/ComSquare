//
// Created by Zoe Roux on 2021-07-03.
//


#pragma once

#include "Models/Ints.hpp"

namespace ComSquare::CPU
{
	//! @brief Struct containing registers for the main CPU.
	struct Registers
	{
		//! @brief The Accumulator
		union
		{
			struct
			{
				uint8_t al;
				uint8_t ah;
			};
			uint16_t a;
		};
		//! @brief The Data Bank Register;
		uint8_t dbr;
		//! @brief The Direct Page register;
		union
		{
			struct
			{
				uint8_t dl;
				uint8_t dh;
			};
			uint16_t d;
		};
		union
		{
			struct
			{
				//! @brief The Program Counter;
				union
				{
					struct
					{
						uint8_t pcl;
						uint8_t pch;
					};
					uint16_t pc;
				};
				//! @brief The Program Bank Register;
				uint8_t pbr;
			};
			//! @brief The current Program Address Counter (does not exist in a snes but is useful here).
			uint24_t pac;
		};
		//! @brief The Stack pointer
		union
		{
			struct
			{
				uint8_t sl;
				uint8_t sh;
			};
			uint16_t s;
		};
		//! @brief The X index register
		union
		{
			struct
			{
				uint8_t xl;
				uint8_t xh;
			};
			uint16_t x;
		};
		//! @brief The Y index register
		union
		{
			struct
			{
				uint8_t yl;
				uint8_t yh;
			};
			uint16_t y;
		};

		//! @brief The Processor status register;
		union
		{
			struct
			{
				//!	@brief The Carry flag
				bool c: 1;
				//! @brief The Zero flag
				bool z: 1;
				//!	@brief The Interrupt request disable flag
				bool i: 1;
				//!	@brief The Decimal mode flag
				bool d: 1;
				//!	@brief The indeX register width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode OR the Break flag (in emulation mode only)
				bool x_b: 1;
				//! @brief The accumulator and Memory width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode.
				bool m: 1;
				//! @brief The oVerflow flag
				bool v: 1;
				//!	@brief The Negative flag
				bool n: 1;
			};
			uint8_t flags;
		} p;
	};

	//! @brief Struct containing internal registers of the CPU.
	struct InternalRegisters
	{
		//! @brief Interrupt Enable Register
		uint8_t nmitimen;

		//! @brief IO Port Write Register
		uint8_t wrio;

		//! @brief Multiplicand Register A
		uint8_t wrmpya;
		//! @brief Multiplicand Register B
		uint8_t wrmpyb;

		//! @brief Divisor & Dividend Registers (A - Low)
		uint8_t wrdivl;
		//! @brief Divisor & Dividend Registers (A - High)
		uint8_t wrdivh;
		//! @brief Divisor & Dividend Registers (B)
		uint8_t wrdivb;

		//! @brief IRQ Timer Registers (Horizontal - Low)
		uint8_t htimel;
		//! @brief IRQ Timer Registers (Horizontal - High)
		uint8_t htimeh;

		//! @brief IRQ Timer Registers (Vertical - Low)
		uint8_t vtimel;
		//! @brief IRQ Timer Registers (Vertical - High)
		uint8_t vtimeh;

		//! @brief HDMA Enable Register
		uint8_t hdmaen;

		//! @brief ROM Speed Register
		uint8_t memsel;

		//! @brief Interrupt Flag Registers
		uint8_t rdnmi;
		//! @brief Interrupt Flag Registers - TimeUp
		uint8_t timeup;

		//! @brief PPU Status Register
		uint8_t hvbjoy;

		//! @brief IO Port Read Register
		uint8_t rdio;

		//! @brief Divide Result Registers (can sometimes be used as multiplication result register) - LOW
		uint8_t rddivl;
		//! @brief Divide Result Registers (can sometimes be used as multiplication result register) - HIGH
		uint8_t rddivh;

		//! @brief Multiplication Result Registers (can sometimes be used as divide result register) - LOW
		uint8_t rdmpyl;
		//! @brief Multiplication Result Registers (can sometimes be used as divide result register) - HIGH
		uint8_t rdmpyh;

		//! @brief Controller Port Data Registers (Pad 1 - Low)
		uint8_t joy1l;
		//! @brief Controller Port Data Registers (Pad 1 - High)
		uint8_t joy1h;

		//! @brief Controller Port Data Registers (Pad 2 - Low)
		uint8_t joy2l;
		//! @brief Controller Port Data Registers (Pad 2 - High)
		uint8_t joy2h;

		//! @brief Controller Port Data Registers (Pad 3 - Low)
		uint8_t joy3l;
		//! @brief Controller Port Data Registers (Pad 3 - High)
		uint8_t joy3h;

		//! @brief Controller Port Data Registers (Pad 4 - Low)
		uint8_t joy4l;
		//! @brief Controller Port Data Registers (Pad 4 - High)
		uint8_t joy4h;
	};
}