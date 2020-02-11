//
// Created by anonymus-raccoon on 1/24/20.
//

#ifndef COMSQUARE_CPU_HPP
#define COMSQUARE_CPU_HPP

#include "../Memory/IMemory.hpp"
#include "../Memory/MemoryBus.hpp"
#include "../Models/Ints.hpp"
#include "Instructions/CommonInstructions.hpp"
#include "../Cartridge/Cartridge.hpp"

namespace ComSquare::CPU
{
	//! @brief Struct containing registers for the main CPU.
	struct Registers {
		//! @brief The Accumulator
		union {
			struct {
				uint8_t ah;
				uint8_t al;
			};
			uint16_t a;
		};
		//! @brief The Data Bank Register;
		uint8_t dbr;
		//! @brief The Direct register;
		union {
			struct {
				uint8_t dh;
				uint8_t dl;
			};
			uint16_t d;
		};
		union {
			struct {
				//! @brief The Program Bank Register;
				uint8_t pbr;
				//! @brief The Program Counter;
				union {
					struct {
						uint8_t pch;
						uint8_t pcl;
					};
					uint16_t pc;
				};
			};
			//! @brief The current Program Address Counter (does not exist in a snes but is useful here).
			uint24_t pac;
		};
		//! @brief The Stack pointer
		union {
			struct {
				uint8_t sh;
				uint8_t sl;
			};
			uint16_t s;
		};
		//! @brief The X index register
		union {
			struct {
				uint8_t xh;
				uint8_t xl;
			};
			uint16_t x;
		};
		//! @brief The Y index register
		union {
			struct {
				uint8_t yh;
				uint8_t yl;
			};
			uint16_t y;
		};

		//! @brief The Processor status register;
		union {
			struct {
				//!	@brief The Negative flag
				bool n : 1;
				//! @brief The oVerflow flag
				bool v : 1;
				//! @brief The accumulator and Memory width flag (in native mode only)
				bool m : 1;
				//!	@brief The indeX register width flag (in native mode only) OR the Break flag (in emulation mode only)
				bool x_b : 1;
				//!	@brief The Decimal mode flag
				bool d : 1;
				//!	@brief The Interrupt request disable flag
				bool i : 1;
				//! @brief The Zero flag
				bool z : 1;
				//!	@brief The Carry flag
				bool c : 1;
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

		//! @brief DMA Enable Register
		uint8_t mdmaen;

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

	//! @brief All the instructions opcode of the main CPI.
	//! @info The name of the instruction followed by their parameters (after an underscore) if any.
	//! @info Addr mode with an i at the end means indirect.
	//! @info Addr mode with an l at the end means long.
	enum Instructions
	{
		BRK = 0x00,
		ADC_DPXi = 0x61,
		ADC_SR = 0x63,
		ADC_DP = 0x65,
		ADC_DPil = 0x67,
		ADC_IM = 0x69,
		ADC_ABS = 0x6D,
		ADC_ABSl = 0x6F,
		ADC_DPYi = 0x71,
		ADC_DPi = 0x72,
		ADC_SRYi = 0x73,
		ADC_DPX = 0x75,
		ADC_DPYil = 0x77,
		ADC_ABSY = 0x79,
		ADC_ABSX = 0x7D,
		ADC_ABSXl = 0x7F
	};

	//! @brief The main CPU
	class CPU : public CommonInstructions, public Memory::IMemory {
	private:
		//! @brief All the registers of the CPU
		Registers _registers{};
		//! @brief Is the CPU running in emulation mode (in 8bits)
		bool _isEmulationMode = true;
		//! @brief Internal registers of the CPU (accessible from the bus via addr $4200 to $421F).
		InternalRegisters _internalRegisters{};
		//! @brief The memory bus to use for read/write.
		std::shared_ptr<Memory::MemoryBus> _bus;
		//! @brief The cartridge header (stored for interrupt vectors..
		Cartridge::Header &_cartridgeHeader;

		//! @brief Immediate address mode is specified with a value. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddr();
		//! @brief The destination is formed by adding the direct page register with the 8-bit address to form an effective address. (This functions returns the 24bit space address of the value).
		uint24_t _getDirectAddr();
		//! @brief The effective address is formed by DBR:<16-bit exp>. (This functions returns the 24bit space address of the value).
		uint24_t _getAbsoluteAddr();
		//! @brief The effective address is the expression. (This functions returns the 24bit space address of the value).
		uint24_t _getAbsoluteLongAddr();
		//! @brief The address is DBR:$(read($($Value + D)) + Y). (This functions returns the 24bit space address of the value).
		uint24_t _getDirectIndirectIndexedYAddr();
		//! @brief This mode is like the previous addressing mode, but the difference is that rather than pulling 2 bytes from the DP address, it pulls 3 bytes to form the effective address.
		uint24_t _getDirectIndirectIndexedYLongAddr();
		//! @brief The direct page address is calculated and added with x. 2 bytes from the dp address combined with DBR will form the effective address.
		uint24_t _getDirectIndirectIndexedXAddr();
		//! @brief The DP address is added to X to form the effective address. The effective address is always in bank 0.
		uint24_t _getDirectIndexedByXAddr();
		//! @brief The DP address is added to Y to form the effective address. The effective address is always in bank 0.
		uint24_t _getDirectIndexedByYAddr();
		//! @brief The absolute expression is added with X and combined with DBR to form the effective address.
		uint24_t _getAbsoluteIndexedByXAddr();
		//! @brief The absolute expression is added with Y and combined with DBR to form the effective address.
		uint24_t _getAbsoluteIndexedByYAddr();
		//! @brief The effective address is formed by adding the <long exp> with X.
		uint24_t _getAbsoluteIndexedByXLongAddr();
		//! @brief The <8-bit signed exp> is added to PC (program counter) to form the new location.
		uint24_t _getProgramCounterRelativeAddr();
		//! @brief The <16-bit signed exp> is added to PC (program counter) to form the new location.
		uint24_t _getProgramCounterRelativeLongAddr();
		//! @brief 2 bytes are pulled from the <abs exp> to form the effective address.
		uint24_t _getAbsoluteIndirectAddr();
		//! @brief The <abs exp> is added with X, then 2 bytes are pulled from that address to form the new location.
		uint24_t _getAbsoluteIndexedIndirectAddr();
		//! @brief 2 bytes are pulled from the direct page address to form the 16-bit address. It is combined with DBR to form a 24-bit effective address.
		uint24_t _getDirectIndirectAddr();
		//! @brief 3 bytes are pulled from the direct page address to form an effective address.
		uint24_t _getDirectIndirectLongAddr();
		//! @brief The stack register is added to the <8-bit exp> to form the effective address.
		uint24_t _getStackRelativeAddr();
		//! @brief The <8-bit exp> is added to S and combined with DBR to form the base address. Y is added to the base address to form the effective address.
		uint24_t _getStackRelativeIndirectIndexedYAddr();


		//! @brief Execute a single instruction.
		//! @return The number of CPU cycles that the instruction took.
		int executeInstruction();

		//! @brief Break instruction - Causes a software break. The PC is loaded from a vector table.
		int BRK();
		//! @brief Add with carry - Adds operand to the Accumulator; adds an additional 1 if carry is set.
		int ADC(uint24_t valueAddr);
	public:
		explicit CPU(std::shared_ptr<Memory::MemoryBus> bus, Cartridge::Header &cartridgeHeader);
		//! @brief This function continue to execute the Cartridge code.
		//! @return The number of CPU cycles that elapsed
		int update();
		//! @brief Read from the internal CPU register.
		//! @param addr The address to read from. The address 0x0 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $1F (the number of register).
		//! @return Return the value of the register.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the internal CPU register.
		//! @param addr The address to write to. The address 0x0 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $1F (the number of register).
		void write(uint24_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_CPU_HPP
