//
// Created by anonymus-raccoon on 1/24/20.
//

#ifndef COMSQUARE_CPU_HPP
#define COMSQUARE_CPU_HPP

#include "../Memory/AMemory.hpp"
#include "../Memory/MemoryBus.hpp"
#include "../Models/Int24.hpp"
#include "../Cartridge/Cartridge.hpp"
#include "../Memory/AMemory.hpp"
#include "Instruction.hpp"

namespace ComSquare::CPU
{
	//! @brief Struct containing registers for the main CPU.
	struct Registers {
		//! @brief The Accumulator
		union {
			struct {
				uint8_t al;
				uint8_t ah;
			};
			uint16_t a;
		};
		//! @brief The Data Bank Register;
		uint8_t dbr;
		//! @brief The Direct Page register;
		union {
			struct {
				uint8_t dl;
				uint8_t dh;
			};
			uint16_t d;
		};
		union {
			struct {
				//! @brief The Program Counter;
				union {
					struct {
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
		union {
			struct {
				uint8_t sl;
				uint8_t sh;
			};
			uint16_t s;
		};
		//! @brief The X index register
		union {
			struct {
				uint8_t xl;
				uint8_t xh;
			};
			uint16_t x;
		};
		//! @brief The Y index register
		union {
			struct {
				uint8_t yl;
				uint8_t yh;
			};
			uint16_t y;
		};

		//! @brief The Processor status register;
		union {
			struct {
				//!	@brief The Carry flag
				bool c : 1;
				//! @brief The Zero flag
				bool z : 1;
				//!	@brief The Interrupt request disable flag
				bool i : 1;
				//!	@brief The Decimal mode flag
				bool d : 1;
				//!	@brief The indeX register width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode OR the Break flag (in emulation mode only)
				bool x_b : 1;
				//! @brief The accumulator and Memory width flag (in native mode only) - 0 = 16 bits mode, 1 = 8 bits mode.
				bool m : 1;
				//! @brief The oVerflow flag
				bool v : 1;
				//!	@brief The Negative flag
				bool n : 1;
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

	//! @brief The main CPU
	class CPU : public Memory::AMemory {
	protected:
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

		//! @brief True if an addressing mode with an iterator (x, y) has crossed the page. (Used because crossing the page boundary take one more cycle to run certain instructions).
		bool _hasIndexCrossedPageBoundary = false;

		//! @brief Immediate address mode is specified with a value in 8. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddr8Bits();
		//! @brief Immediate address mode is specified with a value in 8 or 16 bits. The value is 16 bits if the m flag is unset. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddrForA();
		//! @brief Immediate address mode is specified with a value in 8 or 16 bits. The value is 16 bits if the x flag is unset. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddrForX();
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
		//! @brief 2 bytes are pulled from the <abs exp> to form the effective address.
		uint24_t _getAbsoluteIndirectAddr();
		//! @brief 3 bytes are pulled from the <abs exp> to form the effective address.
		uint24_t _getAbsoluteIndirectLongAddr();
		//! @brief The <abs exp> is added with X, then 2 bytes are pulled from that address to form the new location.
		uint24_t _getAbsoluteIndirectIndexedByXAddr();
		//! @brief 2 bytes are pulled from the direct page address to form the 16-bit address. It is combined with DBR to form a 24-bit effective address.
		uint24_t _getDirectIndirectAddr();
		//! @brief 3 bytes are pulled from the direct page address to form an effective address.
		uint24_t _getDirectIndirectLongAddr();
		//! @brief The stack register is added to the <8-bit exp> to form the effective address.
		uint24_t _getStackRelativeAddr();
		//! @brief The <8-bit exp> is added to S and combined with DBR to form the base address. Y is added to the base address to form the effective address.
		uint24_t _getStackRelativeIndirectIndexedYAddr();


		//! @brief Push 8 bits of data to the stack.
		void _push(uint8_t data);
		//! @brief Push 16 bits of data to the stack.
		void _push(uint16_t data);
		//! @brief Pop 8 bits of data from the stack.
		uint8_t _pop();
		//! @brief Pop 16 bits of data from the stack.
		uint16_t _pop16();

		//! @brief Return the data at the program bank concatenated with the program counter. It also increment the program counter (the program bank is not incremented on overflows).
		uint8_t readPC();

		//! @brief Execute a single instruction.
		//! @return The number of CPU cycles that the instruction took.
		virtual unsigned _executeInstruction(uint8_t opcode);

		//! @brief Break instruction - Causes a software break. The PC is loaded from a vector table.
		int BRK(uint24_t, AddressingMode);
		//! @brief Co-Processor Enable instruction - Causes a software break. The PC is loaded from a vector table.
		int COP(uint24_t, AddressingMode);
		//! @brief Return from Interrupt - Used to return from a interrupt handler.
		int RTI(uint24_t, AddressingMode);
		//! @brief Add with carry - Adds operand to the Accumulator; adds an additional 1 if carry is set.
		int ADC(uint24_t valueAddr, AddressingMode);
		//! @brief Store the accumulator to memory.
		int STA(uint24_t addr, AddressingMode);
		//! @brief Store the index register X to memory.
		int STX(uint24_t addr, AddressingMode);
		//! @brief Store the index register Y to memory.
		int STY(uint24_t addr, AddressingMode);
		//! @brief Store zero to the memory.
		int STZ(uint24_t addr, AddressingMode);
		//! @brief Load the accumulator from memory.
		int LDA(uint24_t addr, AddressingMode);
		//! @brief Load the X index register from memory.
		int LDX(uint24_t addr, AddressingMode);
		//! @brief Load the Y index register from memory.
		int LDY(uint24_t addr, AddressingMode);
		//! @brief Set status bits.
		int SEP(uint24_t valueAddr, AddressingMode);
		//! @brief Reset status bits.
		int REP(uint24_t valueAddr, AddressingMode);
		//! @brief Jump to subroutine
		int JSR(uint24_t addr, AddressingMode);
		//! @brief Jump to subroutine (long)
		int JSL(uint24_t addr, AddressingMode);
		//! @brief Push the accumulator to the stack.
		int PHA(uint24_t, AddressingMode);
		//! @brief Push the data bank register to the stack.
		int PHB(uint24_t, AddressingMode);
		//! @brief Push the direct page register to the stack.
		int PHD(uint24_t, AddressingMode);
		//! @brief Push the program bank register to the stack.
		int PHK(uint24_t, AddressingMode);
		//! @brief Push the processor status register to the stack.
		int PHP(uint24_t, AddressingMode);
		//! @brief Push the x index register to the stack.
		int PHX(uint24_t, AddressingMode);
		//! @brief Push the y index register to the stack.
		int PHY(uint24_t, AddressingMode);
		//! @brief Pull the accumulator to the stack.
		int PLA(uint24_t, AddressingMode);
		//! @brief Pull the data bank register to the stack.
		int PLB(uint24_t, AddressingMode);
		//! @brief Pull the direct page register to the stack.
		int PLD(uint24_t, AddressingMode);
		//! @brief Pull the processor status register to the stack.
		int PLP(uint24_t, AddressingMode);
		//! @brief Pull the x index register to the stack.
		int PLX(uint24_t, AddressingMode);
		//! @brief Pull the y index register to the stack.
		int PLY(uint24_t, AddressingMode);
		//! @brief Clear the carry flag.
		int CLC(uint24_t, AddressingMode);
		//! @brief Clear the Interrupt Disable flag.
		int CLI(uint24_t, AddressingMode);
		//! @brief Clear the decimal flag.
		int CLD(uint24_t, AddressingMode);
		//! @brief Clear the overflow flag.
		int CLV(uint24_t, AddressingMode);
		//! @brief Set the carry Flag.
		int SEC(uint24_t, AddressingMode);
		//! @brief Set the decimal flag.
		int SED(uint24_t, AddressingMode);
		//! @brief Set the Interrupt Disable flag.
		int SEI(uint24_t, AddressingMode);
		//! @brief Exchange Carry and Emulation Flags
		int XCE(uint24_t, AddressingMode);
		//! @brief And accumulator with memory.
		int AND(uint24_t valueAddr, AddressingMode);
		//! @brief Subtract with Borrow from Accumulator.
		int SBC(uint24_t valueAddr, AddressingMode);
		//! @brief Transfer A to X
		int TAX(uint24_t, AddressingMode);
		//! @brief Transfer A to Y
		int TAY(uint24_t, AddressingMode);
		//! @brief Transfer X to SP
		int TXS(uint24_t, AddressingMode);
		//! @brief Increment the X register
		int INX(uint24_t, AddressingMode);
		//! @brief Increment the Y register
		int INY(uint24_t, AddressingMode);
		//! @brief Compare the X register with the memory
		int CPX(uint24_t valueAddr, AddressingMode);
		//! @brief Compare the Y register with the memory
		int CPY(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if carry clear
		int BCC(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if carry set
		int BCS(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if equal
		int BEQ(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if not equal
		int BNE(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if minus
		int BMI(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if plus
		int BPL(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if Overflow Clear
		int BVC(uint24_t valueAddr, AddressingMode);
		//! @brief Branch if Overflow Set
		int BVS(uint24_t valueAddr, AddressingMode);
		//! @brief Branch always
		int BRA(uint24_t valueAddr, AddressingMode);
		//! @brief Branch always long
		int BRL(uint24_t valueAddr, AddressingMode);
		//! @brief Jump.
		int JMP(uint24_t valueAddr, AddressingMode);
		//! @brief Long jump.
		int JML(uint24_t valueAddr, AddressingMode);
		//! @brief No OP.
		int NOP(uint24_t, AddressingMode);
		//! @brief Decrement the X register.
		int DEX(uint24_t, AddressingMode);
		//! @brief Decrement the Y register.
		int DEY(uint24_t, AddressingMode);

		//! @brief All the instructions of the CPU.
		//! @info Instructions are indexed by their opcode
		Instruction _instructions[0x100] = {
			{&CPU::BRK, 7, "brk", AddressingMode::Implied, 2}, // 00
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 01
			{&CPU::COP, 7, "cop", AddressingMode::Implied, 2}, // 02
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 03
			{&CPU::BRK, 7, "tsb #-#", AddressingMode::Implied, 2}, // 04
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 05
			{&CPU::BRK, 7, "asl #-#", AddressingMode::Implied, 2}, // 06
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 07
			{&CPU::PHP, 3, "php", AddressingMode::Implied, 3}, // 08
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 09
			{&CPU::BRK, 7, "asl #-#", AddressingMode::Implied, 2}, // 0A
			{&CPU::PHD, 4, "phd", AddressingMode::Implied, 1}, // 0B
			{&CPU::BRK, 7, "tsb #-#", AddressingMode::Implied, 2}, // 0C
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 0D
			{&CPU::BRK, 7, "asl #-#", AddressingMode::Implied, 2}, // 0E
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 0F
			{&CPU::BPL, 7, "bpl", AddressingMode::Implied, 2}, // 10
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 11
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 12
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 13
			{&CPU::BRK, 7, "trb #-#", AddressingMode::Implied, 2}, // 14
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 15
			{&CPU::BRK, 7, "asl #-#", AddressingMode::Implied, 2}, // 16
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 17
			{&CPU::CLC, 2, "clc", AddressingMode::Implied, 1}, // 18
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 19
			{&CPU::BRK, 7, "inc #-#", AddressingMode::Implied, 2}, // 1A
			{&CPU::BRK, 7, "tcs #-#", AddressingMode::Implied, 2}, // 1B
			{&CPU::BRK, 7, "trb #-#", AddressingMode::Implied, 2}, // 1C
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 1D
			{&CPU::BRK, 7, "asl #-#", AddressingMode::Implied, 2}, // 1E
			{&CPU::BRK, 7, "ora #-#", AddressingMode::Implied, 2}, // 1F
			{&CPU::JSR, 6, "jsr", AddressingMode::Absolute, 3}, // 20
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 21
			{&CPU::JSL, 8, "jsl", AddressingMode::AbsoluteLong, 4}, // 22
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 23
			{&CPU::BRK, 7, "bit #-#", AddressingMode::Implied, 2}, // 24
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 25
			{&CPU::BRK, 7, "rol #-#", AddressingMode::Implied, 2}, // 26
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 27
			{&CPU::PLP, 4, "plp", AddressingMode::Implied, 1}, // 28
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 29
			{&CPU::BRK, 7, "rol #-#", AddressingMode::Implied, 2}, // 2A
			{&CPU::PLD, 5, "pld", AddressingMode::Implied, 1}, // 2B
			{&CPU::BRK, 7, "bit #-#", AddressingMode::Implied, 2}, // 2C
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 2D
			{&CPU::BRK, 7, "rol #-#", AddressingMode::Implied, 2}, // 2E
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 2F
			{&CPU::BMI, 2, "bmi", AddressingMode::Implied, 2}, // 30
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 31
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 32
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 33
			{&CPU::BRK, 7, "bit #-#", AddressingMode::Implied, 2}, // 34
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 35
			{&CPU::BRK, 7, "rol #-#", AddressingMode::Implied, 2}, // 36
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 37
			{&CPU::SEC, 2, "sec", AddressingMode::Implied, 1}, // 38
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 39
			{&CPU::BRK, 7, "dec #-#", AddressingMode::Implied, 2}, // 3A
			{&CPU::BRK, 7, "tsc #-#", AddressingMode::Implied, 2}, // 3B
			{&CPU::BRK, 7, "bit #-#", AddressingMode::Implied, 2}, // 3C
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 3D
			{&CPU::BRK, 7, "rol #-#", AddressingMode::Implied, 2}, // 3E
			{&CPU::BRK, 7, "and #-#", AddressingMode::Implied, 2}, // 3F
			{&CPU::RTI, 6, "rti", AddressingMode::Implied, 1}, // 40
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 41
			{&CPU::BRK, 7, "wdm #-#", AddressingMode::Implied, 2}, // 42
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 43
			{&CPU::BRK, 7, "mvp #-#", AddressingMode::Implied, 2}, // 44
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 45
			{&CPU::BRK, 7, "lsr #-#", AddressingMode::Implied, 2}, // 46
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 47
			{&CPU::PHA, 3, "pha", AddressingMode::Implied, 1}, // 48
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 49
			{&CPU::BRK, 7, "lsr #-#", AddressingMode::Implied, 2}, // 4A
			{&CPU::PHK, 3, "phk", AddressingMode::Implied, 1}, // 4B
			{&CPU::JMP, 3, "jmp", AddressingMode::Absolute, 3}, // 4C
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 4D
			{&CPU::BRK, 7, "lsr #-#", AddressingMode::Implied, 2}, // 4E
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 4F
			{&CPU::BVC, 2, "bvc", AddressingMode::Implied, 2}, // 50
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 51
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 52
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 53
			{&CPU::BRK, 7, "mvn #-#", AddressingMode::Implied, 2}, // 54
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 55
			{&CPU::BRK, 7, "lsr #-#", AddressingMode::Implied, 2}, // 56
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 57
			{&CPU::CLI, 2, "cli", AddressingMode::Implied, 1}, // 58
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 59
			{&CPU::PHY, 3, "phy", AddressingMode::Implied, 1}, // 5A
			{&CPU::BRK, 7, "tcd #-#", AddressingMode::Implied, 2}, // 5B
			{&CPU::JML, 4, "jml", AddressingMode::Implied, 4}, // 5C
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 5D
			{&CPU::BRK, 7, "lsr #-#", AddressingMode::Implied, 2}, // 5E
			{&CPU::BRK, 7, "eor #-#", AddressingMode::Implied, 2}, // 5F
			{&CPU::BRK, 7, "rtl #-#", AddressingMode::Implied, 1}, // 60
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectIndexedByX, 2}, // 61
			{&CPU::BRK, 7, "per #-#", AddressingMode::Implied, 2}, // 62
			{&CPU::ADC, 4, "adc", AddressingMode::StackRelative, 2}, // 63
			{&CPU::STZ, 3, "stz", AddressingMode::DirectPage, 2}, // 64
			{&CPU::ADC, 3, "adc", AddressingMode::DirectPage, 2}, // 65
			{&CPU::BRK, 7, "ror #-#", AddressingMode::Implied, 2}, // 66
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectLong, 2}, // 67
			{&CPU::PLA, 4, "pla", AddressingMode::Implied, 1}, // 68
			{&CPU::ADC, 2, "adc", AddressingMode::ImmediateForA, 2}, // 69
			{&CPU::BRK, 7, "ror #-#", AddressingMode::Implied, 2}, // 6A
			{&CPU::BRK, 7, "rts #-#", AddressingMode::Implied, 1}, // 6B
			{&CPU::JMP, 5, "jmp", AddressingMode::AbsoluteIndirect, 3}, // 6C
			{&CPU::ADC, 4, "adc", AddressingMode::Absolute, 3}, // 6D
			{&CPU::BRK, 7, "ror #-#", AddressingMode::Implied, 2}, // 6E
			{&CPU::ADC, 5, "adc", AddressingMode::AbsoluteLong, 4}, // 6F
			{&CPU::BVS, 2, "bvs", AddressingMode::Implied, 2}, // 70
			{&CPU::ADC, 5, "adc", AddressingMode::DirectPageIndirectIndexedByY, 2}, // 71
			{&CPU::ADC, 5, "adc", AddressingMode::DirectPageIndirect, 2}, // 72
			{&CPU::ADC, 7, "adc", AddressingMode::StackRelativeIndirectIndexedByY, 2}, // 73
			{&CPU::STZ, 4, "stz", AddressingMode::DirectPageIndexedByX, 2}, // 74
			{&CPU::ADC, 4, "adc", AddressingMode::DirectPageIndexedByX, 2}, // 75
			{&CPU::BRK, 7, "ror #-#", AddressingMode::Implied, 2}, // 76
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectIndexedByY, 2}, // 77
			{&CPU::SEI, 2, "sei", AddressingMode::Implied, 1}, // 78
			{&CPU::ADC, 4, "adc", AddressingMode::AbsoluteIndexedByY, 2}, // 79
			{&CPU::PLY, 4, "ply", AddressingMode::Implied, 1}, // 7A
			{&CPU::BRK, 7, "tdc #-#", AddressingMode::Implied, 2}, // 7B
			{&CPU::JMP, 6, "jmp", AddressingMode::AbsoluteIndirectIndexedByX, 3}, // 7C
			{&CPU::ADC, 4, "adc", AddressingMode::AbsoluteIndexedByX, 3}, // 7D
			{&CPU::BRK, 7, "ror #-#", AddressingMode::Implied, 2}, // 7E
			{&CPU::ADC, 5, "adc", AddressingMode::AbsoluteIndexedByXLong, 4}, // 7F
			{&CPU::BRA, 3, "bra", AddressingMode::Implied, 2}, // 80
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndexedByX, 2}, // 81
			{&CPU::BRL, 4, "brl", AddressingMode::Implied, 3}, // 82
			{&CPU::STA, 4, "sta", AddressingMode::StackRelative, 2}, // 83
			{&CPU::STY, 3, "sty", AddressingMode::DirectPage, 2}, // 84
			{&CPU::STA, 3, "sta", AddressingMode::DirectPage, 2}, // 85
			{&CPU::STX, 3, "stx", AddressingMode::DirectPage, 2}, // 86
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectLong, 2}, // 87
			{&CPU::DEY, 2, "dey", AddressingMode::Implied, 1}, // 88
			{&CPU::BRK, 7, "bit #-#", AddressingMode::Implied, 2}, // 89
			{&CPU::BRK, 7, "txa #-#", AddressingMode::Implied, 2}, // 8A
			{&CPU::PHB, 3, "phb", AddressingMode::Implied, 1}, // 8B
			{&CPU::STY, 4, "sty", AddressingMode::Absolute, 3}, // 8C
			{&CPU::STA, 4, "sta", AddressingMode::Absolute, 3}, // 8D
			{&CPU::STX, 4, "stx", AddressingMode::Absolute, 3}, // 8E
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteLong, 4}, // 8F
			{&CPU::BCC, 2, "bcc", AddressingMode::Implied, 2}, // 90
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectIndexedByY, 2}, // 91
			{&CPU::STA, 5, "sta", AddressingMode::DirectPageIndirect, 2}, // 92
			{&CPU::STA, 7, "sta", AddressingMode::StackRelativeIndirectIndexedByY, 2}, // 93
			{&CPU::STY, 4, "sty", AddressingMode::DirectPageIndirectIndexedByX, 2}, // 94
			{&CPU::STA, 4, "sta", AddressingMode::DirectPageIndexedByX, 2}, // 95
			{&CPU::STX, 4, "stx", AddressingMode::DirectPageIndexedByY, 2}, // 96
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // 97
			{&CPU::BRK, 7, "tya #-#", AddressingMode::Implied, 2}, // 98
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByY, 3}, // 99
			{&CPU::TXS, 2, "txs", AddressingMode::Implied, 1}, // 9A
			{&CPU::BRK, 7, "txy #-#", AddressingMode::Implied, 2}, // 9B
			{&CPU::STZ, 4, "stz", AddressingMode::Absolute, 3}, // 9C
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByX, 3}, // 9D
			{&CPU::STZ, 5, "stz", AddressingMode::AbsoluteIndexedByX, 3}, // 9E
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByXLong, 4}, // 9F
			{&CPU::LDY, 2, "ldy", AddressingMode::ImmediateForX, 2}, // A0
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectIndexedByX, 2}, // A1
			{&CPU::LDX, 2, "ldx", AddressingMode::ImmediateForX, 2}, // A2
			{&CPU::LDA, 4, "lda", AddressingMode::StackRelative, 2}, // A3
			{&CPU::LDY, 3, "ldy", AddressingMode::DirectPage, 2}, // A4
			{&CPU::LDA, 3, "lda", AddressingMode::DirectPage, 2}, // A5
			{&CPU::LDX, 3, "ldx", AddressingMode::DirectPage, 2}, // A6
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectLong, 2}, // A7
			{&CPU::TAY, 2, "tay", AddressingMode::Implied, 1}, // A8
			{&CPU::LDA, 2, "lda", AddressingMode::ImmediateForA, 2}, // A9
			{&CPU::TAX, 2, "tax", AddressingMode::Implied, 1}, // AA
			{&CPU::BRK, 7, "trb #-#", AddressingMode::Implied, 2}, // AB
			{&CPU::LDY, 4, "ldy", AddressingMode::Absolute, 4}, // AC
			{&CPU::LDA, 4, "lda", AddressingMode::Absolute, 3}, // AD
			{&CPU::LDX, 4, "ldx", AddressingMode::Absolute, 3}, // AE
			{&CPU::LDA, 5, "lda", AddressingMode::AbsoluteLong, 4}, // AF
			{&CPU::BCS, 2, "bcs", AddressingMode::Implied, 2}, // B0
			{&CPU::LDA, 5, "lda", AddressingMode::DirectPageIndirectIndexedByY, 2}, // B1
			{&CPU::LDA, 5, "lda", AddressingMode::DirectPageIndirect, 2}, // B2
			{&CPU::LDA, 7, "lda", AddressingMode::StackRelativeIndirectIndexedByY, 2}, // B3
			{&CPU::LDY, 4, "ldy", AddressingMode::DirectPageIndexedByX, 2}, // B4
			{&CPU::LDA, 4, "lda", AddressingMode::DirectPageIndexedByX, 2}, // B5
			{&CPU::LDX, 4, "ldx", AddressingMode::DirectPageIndexedByY, 2}, // B6
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // B7
			{&CPU::CLV, 7, "clv", AddressingMode::Implied, 1}, // B8
			{&CPU::LDA, 4, "lda", AddressingMode::AbsoluteIndexedByY, 3}, // B9
			{&CPU::BRK, 7, "mvn #-#", AddressingMode::Implied, 2}, // BA
			{&CPU::BRK, 7, "tyx #-#", AddressingMode::Implied, 2}, // BB
			{&CPU::LDY, 4, "ldy", AddressingMode::AbsoluteIndexedByX, 3}, // BC
			{&CPU::LDA, 4, "lda", AddressingMode::AbsoluteIndexedByX, 3}, // BD
			{&CPU::LDX, 4, "ldx", AddressingMode::AbsoluteIndexedByY, 3}, // BE
			{&CPU::LDA, 5, "lda", AddressingMode::AbsoluteIndexedByXLong, 4}, // BF
			{&CPU::BRK, 7, "cpy #-#", AddressingMode::Implied, 2}, // C0
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // C1
			{&CPU::REP, 3, "rep", AddressingMode::Immediate8bits, 2}, // C2
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // C3
			{&CPU::BRK, 7, "cpy #-#", AddressingMode::Implied, 2}, // C4
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // C5
			{&CPU::BRK, 7, "dec #-#", AddressingMode::Implied, 2}, // C6
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // C7
			{&CPU::INY, 2, "iny", AddressingMode::Implied, 1}, // C8
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // C9
			{&CPU::DEX, 2, "dex", AddressingMode::Implied, 1}, // CA
			{&CPU::BRK, 7, "wai #-#", AddressingMode::Implied, 2}, // CB
			{&CPU::BRK, 7, "cpy #-#", AddressingMode::Implied, 2}, // CC
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // CD
			{&CPU::BRK, 7, "dec #-#", AddressingMode::Implied, 2}, // CE
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // CF
			{&CPU::BNE, 2, "bne", AddressingMode::Implied, 2}, // D0
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D1
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D2
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D3
			{&CPU::BRK, 7, "pei #-#", AddressingMode::Implied, 2}, // D4
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D5
			{&CPU::BRK, 7, "dec #-#", AddressingMode::Implied, 2}, // D6
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D7
			{&CPU::CLD, 27, "cld", AddressingMode::Implied, 2}, // D8
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // D9
			{&CPU::PHX, 3, "phx", AddressingMode::Implied, 1}, // DA
			{&CPU::BRK, 7, "stp #-#", AddressingMode::Implied, 2}, // DB
			{&CPU::JML, 7, "jml", AddressingMode::AbsoluteIndirectLong, 2}, // DC
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // DD
			{&CPU::BRK, 7, "dec #-#", AddressingMode::Implied, 2}, // DE
			{&CPU::BRK, 7, "cmp #-#", AddressingMode::Implied, 2}, // DF
			{&CPU::BRK, 7, "cpx #-#", AddressingMode::Implied, 2}, // E0
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectIndexedByX, 2}, // E1
			{&CPU::SEP, 3, "sep", AddressingMode::Immediate8bits, 2}, // E2
			{&CPU::SBC, 4, "sbc", AddressingMode::StackRelative, 2}, // E3
			{&CPU::BRK, 7, "cpx #-#", AddressingMode::Implied, 2}, // E4
			{&CPU::SBC, 3, "sbc", AddressingMode::DirectPage, 2}, // E5
			{&CPU::BRK, 7, "inc #-#", AddressingMode::Implied, 2}, // E6
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectLong, 2}, // E7
			{&CPU::INX, 2, "inx", AddressingMode::Implied, 1}, // E8
			{&CPU::SBC, 2, "sbc", AddressingMode::ImmediateForA, 2}, // E9
			{&CPU::NOP, 2, "nop", AddressingMode::Implied, 1}, // EA
			{&CPU::BRK, 7, "xba #-#", AddressingMode::Implied, 2}, // EB
			{&CPU::BRK, 7, "cpx #-#", AddressingMode::Implied, 2}, // EC
			{&CPU::SBC, 4, "sbc", AddressingMode::Absolute, 3}, // ED
			{&CPU::BRK, 7, "inc #-#", AddressingMode::Implied, 2}, // EE
			{&CPU::SBC, 5, "sbc", AddressingMode::AbsoluteLong, 4}, // EF
			{&CPU::BEQ, 2, "beq", AddressingMode::Implied, 2}, // F0
			{&CPU::SBC, 5, "sbc", AddressingMode::DirectPageIndirectIndexedByY, 2}, // F1
			{&CPU::SBC, 5, "sbc", AddressingMode::DirectPageIndirect, 2}, // F2
			{&CPU::SBC, 7, "sbc", AddressingMode::StackRelativeIndirectIndexedByY, 2}, // F3
			{&CPU::BRK, 7, "pea #-#", AddressingMode::Implied, 2}, // F4
			{&CPU::SBC, 4, "sbc", AddressingMode::DirectPageIndexedByX, 2}, // F5
			{&CPU::BRK, 7, "inc #-#", AddressingMode::Implied, 2}, // F6
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // F7
			{&CPU::SED, 2, "sed", AddressingMode::Implied, 1}, // F8
			{&CPU::SBC, 4, "sbc", AddressingMode::AbsoluteIndexedByY, 3}, // F9
			{&CPU::PLX, 4, "plx", AddressingMode::Implied, 1}, // FA
			{&CPU::XCE, 2, "xce", AddressingMode::Implied, 1}, // FB
			{&CPU::JSR, 8, "jsr", AddressingMode::AbsoluteIndirectIndexedByX, 3}, // FC
			{&CPU::SBC, 4, "sbc", AddressingMode::AbsoluteIndexedByX, 3}, // FD
			{&CPU::BRK, 7, "inc #-#", AddressingMode::Implied, 2}, // FE
			{&CPU::SBC, 5, "sbc", AddressingMode::AbsoluteIndexedByXLong, 4}, // FF
		};
	public:
		explicit CPU(std::shared_ptr<Memory::MemoryBus> bus, Cartridge::Header &cartridgeHeader);
		CPU(const CPU &) = default;
		CPU &operator=(const CPU &) = delete;
		~CPU() override = default;
		//! @brief This function continue to execute the Cartridge code.
		//! @return The number of CPU cycles that elapsed
		virtual unsigned update();
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

		//! @brief Get the name of this accessor (used for debug purpose)
		std::string getName() override;

		//! @brief Get the component of this accessor (used for debug purpose)
		Component getComponent() override;

		//! @brief Reset interrupt - Called on boot and when the reset button is pressed.
		virtual int RESB();

		//! @brief Return true if the CPU is overloaded with debugging features.
		virtual bool isDebugger();

		//! @brief Change the memory bus used by the CPU.
		void setMemoryBus(std::shared_ptr<Memory::MemoryBus> bus);
	};
}

#endif //COMSQUARE_CPU_HPP
