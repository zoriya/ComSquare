//
// Created by anonymus-raccoon on 1/24/20.
//

#pragma once


#include "Memory/AMemory.hpp"
#include "Memory/MemoryBus.hpp"
#include "Models/Ints.hpp"
#include "Models/Callback.hpp"
#include "Cartridge/Cartridge.hpp"
#include "Memory/AMemory.hpp"
#include "Instruction.hpp"
#include "DMA/DMA.hpp"
#include "CPU/Registers.hpp"

#ifdef DEBUGGER_ENABLED
#include "Debugger/CPU/CPUDebug.hpp"
#include "Debugger/RegisterViewer.hpp"
#endif

namespace ComSquare::CPU
{
	//! @brief The main CPU
	class CPU : public Memory::AMemory
	{
	private:
		//! @brief All the registers of the CPU
		Registers _registers {};
		//! @brief Internal registers of the CPU (accessible from the bus via addr $4200 to $421F).
		InternalRegisters _internalRegisters {};

		//! @brief Is the CPU running in emulation mode (in 8bits)
		bool _isEmulationMode = true;
		//! @brief If the processor is stopped (using an STP instruction), the clock is stopped and no instruction will be run until a manual reset.
		bool _isStopped = false;
		//! @brief Is the processor waiting for an interrupt (if true, instructions are not run until an interrupt is requested).
		bool _isWaitingForInterrupt = false;

		//! @brief The memory bus to use for read/write.
		std::reference_wrapper<Memory::IMemoryBus> _bus;
		//! @brief The cartridge header (stored for interrupt vectors..)
		Cartridge::Header &_cartridgeHeader;

		//! @brief DMA channels witch are mapped to the bus.
		std::array<DMA, 8> _dmaChannels;

		//! @brief True if an addressing mode with an iterator (x, y) has crossed the page. (Used because crossing the page boundary take one more cycle to run certain instructions).
		bool _hasIndexCrossedPageBoundary = false;

		//! @brief Immediate address mode is specified with a value in 8 bits. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddr8Bits();
		//! @brief Immediate address mode is specified with a value in 16 bits. (This functions returns the 24bit space address of the value).
		uint24_t _getImmediateAddr16Bits();
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
		inline uint8_t _readPC()
		{
			uint8_t ret = this->getBus().read(this->_registers.pac);
			this->_registers.pc++;
			return ret;
		}

		//! @brief Check if an interrupt is requested and handle it.
		void _checkInterrupts();
		//! @brief Run an interrupt (save state of the processor and jump to the interrupt handler)
		void _runInterrupt(uint24_t nativeHandler, uint24_t emulationHandler);

		//! @brief Get the parameter address of an instruction from it's addressing mode.
		//! @info The current program counter should point to the instruction's opcode + 1.
		//! @return The address of the data to read on the instruction.
		uint24_t _getValueAddr(const Instruction &instruction);

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
		//! @brief Or accumulator with memory.
		int ORA(uint24_t valueAddr, AddressingMode mode);
		//! @brief Return from subroutine.
		int RTS(uint24_t, AddressingMode);
		//! @brief Return from subroutine long.
		int RTL(uint24_t, AddressingMode);
		//! @brief Compare Accumulator with Memory.
		int CMP(uint24_t, AddressingMode);
		//! @brief Increment
		int INC(uint24_t, AddressingMode);
		//! @brief Decrement
		int DEC(uint24_t, AddressingMode);
		//! @brief XOR, Exclusive OR accumulator with memory.
		int EOR(uint24_t, AddressingMode);
		//! @brief Transfer 16 bit A to DP
		int TCD(uint24_t, AddressingMode);
		//! @brief Transfer 16 bit A to SP
		int TCS(uint24_t, AddressingMode);
		//! @brief Transfer DP to 16 bit A
		int TDC(uint24_t, AddressingMode);
		//! @brief Transfer DP to 16 bit A
		int TSC(uint24_t, AddressingMode);
		//! @brief Transfer SP to X
		int TSX(uint24_t, AddressingMode);
		//! @brief Transfer X to A
		int TXA(uint24_t, AddressingMode);
		//! @brief Transfer Y to A
		int TYA(uint24_t, AddressingMode);
		//! @brief Transfer X to Y
		int TXY(uint24_t, AddressingMode);
		//! @brief Transfer Y to X
		int TYX(uint24_t, AddressingMode);
		//! @brief Test and Set Memory Bits Against Accumulator
		int TSB(uint24_t, AddressingMode);
		//! @brief Test and Reset Memory Bits Against Accumulator
		int TRB(uint24_t, AddressingMode);
		//! @brief Exchange the B and A Accumulators
		int XBA(uint24_t, AddressingMode);
		//! @brief Test Memory Bits against Accumulator
		int BIT(uint24_t, AddressingMode);
		//! @brief Arithmetic Shift Left
		int ASL(uint24_t, AddressingMode);
		// !@brief Logical Shift Right
		int LSR(uint24_t, AddressingMode);
		// !@brief Rotate Left
		int ROL(uint24_t, AddressingMode);
		// !@brief Rotate Right
		int ROR(uint24_t, AddressingMode);
		//! @brief Push Effective PC Relative Indirect Address
		int PER(uint24_t, AddressingMode);
		//! @brief Push Effective Indirect Address
		int PEI(uint24_t, AddressingMode);
		//! @brief Push Effective Absolute Address
		int PEA(uint24_t, AddressingMode);
		//! @brief Stop the processor
		int STP(uint24_t, AddressingMode);
		//! @brief Wait for Interrupt
		int WAI(uint24_t, AddressingMode);
		//! @brief WDM Reserved for Future Expansion (used as a code breakpoint)
		int WDM(uint24_t, AddressingMode);
		//! @brief Block Move Next. This instruction is special: it takes parameter in the registers
		//! @param X_register Source address
		//! @param Y_register Destination address
		//! @param C_register (16 bits accumulator) Length -1
		int MVN(uint24_t, AddressingMode);
		//! @brief Block Move Previous. This instruction is special: it takes parameter in the registers
		//! @param X_register Source address (last byte)
		//! @param Y_register Destination address (last byte)
		//! @param C_register (16 bits accumulator) Length -1
		int MVP(uint24_t, AddressingMode);

	public:
		//! @brief Get the memory bus used by this CPU.
		[[nodiscard]] inline Memory::IMemoryBus &getBus()
		{
			return this->_bus;
		}
		//! @brief Set the memory bus used by this CPU
		//! @param bus The bus to use.
		void setBus(Memory::IMemoryBus &bus);

		//! @brief All the instructions of the CPU.
		//! @info Instructions are indexed by their opcode
		const Instruction instructions[0x100] = {
			{&CPU::BRK, 7, "brk", AddressingMode::Immediate8bits,                   2}, // 00
			{&CPU::ORA, 6, "ora", AddressingMode::DirectPageIndirectIndexedByX,     2}, // 01
			{&CPU::COP, 7, "cop", AddressingMode::Immediate8bits,                   2}, // 02
			{&CPU::ORA, 4, "ora", AddressingMode::StackRelative,                    2}, // 03
			{&CPU::TSB, 5, "tsb", AddressingMode::DirectPage,                       2}, // 04
			{&CPU::ORA, 3, "ora", AddressingMode::DirectPage,                       2}, // 05
			{&CPU::ASL, 5, "asl", AddressingMode::DirectPage,                       2}, // 06
			{&CPU::ORA, 6, "ora", AddressingMode::DirectPageIndirectLong,           2}, // 07
			{&CPU::PHP, 3, "php", AddressingMode::Implied,                          1}, // 08
			{&CPU::ORA, 2, "ora", AddressingMode::ImmediateForA,                    2}, // 09
			{&CPU::ASL, 2, "asl", AddressingMode::Implied,                          1}, // 0A
			{&CPU::PHD, 4, "phd", AddressingMode::Implied,                          1}, // 0B
			{&CPU::TSB, 6, "tsb", AddressingMode::Absolute,                         3}, // 0C
			{&CPU::ORA, 3, "ora", AddressingMode::Absolute,                         4}, // 0D
			{&CPU::ASL, 6, "asl", AddressingMode::Absolute,                         3}, // 0E
			{&CPU::ORA, 5, "ora", AddressingMode::AbsoluteLong,                     5}, // 0F
			{&CPU::BPL, 7, "bpl", AddressingMode::Immediate8bits,                   2}, // 10
			{&CPU::ORA, 5, "ora", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 11
			{&CPU::ORA, 5, "ora", AddressingMode::DirectPageIndirect,               2}, // 12
			{&CPU::ORA, 7, "ora", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // 13
			{&CPU::TRB, 5, "trb", AddressingMode::DirectPage,                       2}, // 14
			{&CPU::ORA, 4, "ora", AddressingMode::DirectPageIndexedByX,             2}, // 15
			{&CPU::ASL, 6, "asl", AddressingMode::DirectPageIndexedByX,             2}, // 16
			{&CPU::ORA, 6, "ora", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // 17
			{&CPU::CLC, 2, "clc", AddressingMode::Implied,                          1}, // 18
			{&CPU::ORA, 4, "ora", AddressingMode::AbsoluteIndexedByY,               3}, // 19
			{&CPU::INC, 2, "inc", AddressingMode::Implied,                          1}, // 1A
			{&CPU::TCS, 2, "tcs", AddressingMode::Implied,                          1}, // 1B
			{&CPU::TRB, 6, "trb", AddressingMode::Absolute,                         3}, // 1C
			{&CPU::ORA, 4, "ora", AddressingMode::AbsoluteIndexedByX,               3}, // 1D
			{&CPU::ASL, 7, "asl", AddressingMode::AbsoluteIndexedByX,               3}, // 1E
			{&CPU::ORA, 5, "ora", AddressingMode::AbsoluteIndexedByXLong,           4}, // 1F
			{&CPU::JSR, 6, "jsr", AddressingMode::Absolute,                         3}, // 20
			{&CPU::AND, 6, "and", AddressingMode::DirectPageIndirectIndexedByX,     2}, // 21
			{&CPU::JSL, 8, "jsl", AddressingMode::AbsoluteLong,                     4}, // 22
			{&CPU::AND, 4, "and", AddressingMode::StackRelative,                    2}, // 23
			{&CPU::BIT, 3, "bit", AddressingMode::DirectPage,                       2}, // 24
			{&CPU::AND, 3, "and", AddressingMode::DirectPage,                       2}, // 25
			{&CPU::ROL, 5, "rol", AddressingMode::DirectPage,                       2}, // 26
			{&CPU::AND, 6, "and", AddressingMode::DirectPageIndirectLong,           2}, // 27
			{&CPU::PLP, 4, "plp", AddressingMode::Implied,                          1}, // 28
			{&CPU::AND, 2, "and", AddressingMode::ImmediateForA,                    2}, // 29
			{&CPU::ROL, 2, "rol", AddressingMode::Implied,                          1}, // 2A
			{&CPU::PLD, 5, "pld", AddressingMode::Implied,                          1}, // 2B
			{&CPU::BIT, 4, "bit", AddressingMode::Absolute,                         3}, // 2C
			{&CPU::AND, 4, "and", AddressingMode::Absolute,                         3}, // 2D
			{&CPU::ROL, 6, "rol", AddressingMode::Absolute,                         3}, // 2E
			{&CPU::AND, 5, "and", AddressingMode::AbsoluteLong,                     4}, // 2F
			{&CPU::BMI, 2, "bmi", AddressingMode::Immediate8bits,                   2}, // 30
			{&CPU::AND, 5, "and", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 31
			{&CPU::AND, 5, "and", AddressingMode::DirectPageIndirect,               2}, // 32
			{&CPU::AND, 7, "and", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // 33
			{&CPU::BIT, 4, "bit", AddressingMode::DirectPageIndexedByX,             2}, // 34
			{&CPU::AND, 4, "and", AddressingMode::DirectPageIndexedByX,             2}, // 35
			{&CPU::ROL, 6, "rol", AddressingMode::DirectPageIndexedByX,             2}, // 36
			{&CPU::AND, 6, "and", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // 37
			{&CPU::SEC, 2, "sec", AddressingMode::Implied,                          1}, // 38
			{&CPU::AND, 4, "and", AddressingMode::AbsoluteIndexedByY,               3}, // 39
			{&CPU::DEC, 2, "dec", AddressingMode::Implied,                          1}, // 3A
			{&CPU::TSC, 2, "tsc", AddressingMode::Implied,                          1}, // 3B
			{&CPU::BIT, 4, "bit", AddressingMode::AbsoluteIndexedByX,               3}, // 3C
			{&CPU::AND, 4, "and", AddressingMode::AbsoluteIndexedByX,               3}, // 3D
			{&CPU::ROL, 7, "rol", AddressingMode::AbsoluteIndexedByX,               3}, // 3E
			{&CPU::AND, 5, "and", AddressingMode::AbsoluteIndexedByXLong,           4}, // 3F
			{&CPU::RTI, 6, "rti", AddressingMode::Implied,                          1}, // 40
			{&CPU::EOR, 6, "eor", AddressingMode::DirectPageIndirectIndexedByX,     2}, // 41
			{&CPU::WDM, 2, "wdm", AddressingMode::Immediate8bits,                   2}, // 42
			{&CPU::EOR, 4, "eor", AddressingMode::StackRelative,                    2}, // 43
			{&CPU::MVP, 0, "mvp", AddressingMode::Immediate16bits,                  3}, // 44
			{&CPU::EOR, 3, "eor", AddressingMode::DirectPage,                       2}, // 45
			{&CPU::LSR, 5, "lsr", AddressingMode::DirectPage,                       2}, // 46
			{&CPU::EOR, 6, "eor", AddressingMode::DirectPageIndirectLong,           2}, // 47
			{&CPU::PHA, 3, "pha", AddressingMode::Implied,                          1}, // 48
			{&CPU::EOR, 2, "eor", AddressingMode::ImmediateForA,                    2}, // 49
			{&CPU::LSR, 2, "lsr", AddressingMode::Implied,                          1}, // 4A
			{&CPU::PHK, 3, "phk", AddressingMode::Implied,                          1}, // 4B
			{&CPU::JMP, 3, "jmp", AddressingMode::Absolute,                         3}, // 4C
			{&CPU::EOR, 4, "eor", AddressingMode::Absolute,                         3}, // 4D
			{&CPU::LSR, 6, "lsr", AddressingMode::Absolute,                         3}, // 4E
			{&CPU::EOR, 5, "eor", AddressingMode::AbsoluteLong,                     4}, // 4F
			{&CPU::BVC, 2, "bvc", AddressingMode::Immediate8bits,                   2}, // 50
			{&CPU::EOR, 5, "eor", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 51
			{&CPU::EOR, 5, "eor", AddressingMode::DirectPageIndirect,               2}, // 52
			{&CPU::EOR, 4, "eor", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // 53
			{&CPU::MVN, 0, "mvn", AddressingMode::Immediate16bits,                  2}, // 54
			{&CPU::EOR, 4, "eor", AddressingMode::DirectPageIndexedByX,             2}, // 55
			{&CPU::LSR, 6, "lsr", AddressingMode::DirectPageIndexedByX,             2}, // 56
			{&CPU::EOR, 6, "eor", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // 57
			{&CPU::CLI, 2, "cli", AddressingMode::Implied,                          1}, // 58
			{&CPU::EOR, 4, "eor", AddressingMode::AbsoluteIndexedByY,               3}, // 59
			{&CPU::PHY, 3, "phy", AddressingMode::Implied,                          1}, // 5A
			{&CPU::TCD, 2, "tcd", AddressingMode::Implied,                          1}, // 5B
			{&CPU::JML, 4, "jml", AddressingMode::Implied,                          4}, // 5C
			{&CPU::EOR, 4, "eor", AddressingMode::AbsoluteIndexedByX,               3}, // 5D
			{&CPU::LSR, 7, "lsr", AddressingMode::AbsoluteIndexedByX,               3}, // 5E
			{&CPU::EOR, 5, "eor", AddressingMode::AbsoluteIndexedByXLong,           4}, // 5F
			{&CPU::RTS, 6, "rts", AddressingMode::Implied,                          1}, // 60
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectIndexedByX,     2}, // 61
			{&CPU::PER, 6, "per", AddressingMode::Immediate16bits,                  3}, // 62
			{&CPU::ADC, 4, "adc", AddressingMode::StackRelative,                    2}, // 63
			{&CPU::STZ, 3, "stz", AddressingMode::DirectPage,                       2}, // 64
			{&CPU::ADC, 3, "adc", AddressingMode::DirectPage,                       2}, // 65
			{&CPU::ROR, 5, "ror", AddressingMode::DirectPage,                       2}, // 66
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectLong,           2}, // 67
			{&CPU::PLA, 4, "pla", AddressingMode::Implied,                          1}, // 68
			{&CPU::ADC, 2, "adc", AddressingMode::ImmediateForA,                    2}, // 69
			{&CPU::ROR, 2, "ror", AddressingMode::Implied,                          1}, // 6A
			{&CPU::RTL, 6, "rtl", AddressingMode::Implied,                          1}, // 6B
			{&CPU::JMP, 5, "jmp", AddressingMode::AbsoluteIndirect,                 3}, // 6C
			{&CPU::ADC, 4, "adc", AddressingMode::Absolute,                         3}, // 6D
			{&CPU::ROR, 6, "ror", AddressingMode::Absolute,                         3}, // 6E
			{&CPU::ADC, 5, "adc", AddressingMode::AbsoluteLong,                     4}, // 6F
			{&CPU::BVS, 2, "bvs", AddressingMode::Immediate8bits,                   2}, // 70
			{&CPU::ADC, 5, "adc", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 71
			{&CPU::ADC, 5, "adc", AddressingMode::DirectPageIndirect,               2}, // 72
			{&CPU::ADC, 7, "adc", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // 73
			{&CPU::STZ, 4, "stz", AddressingMode::DirectPageIndexedByX,             2}, // 74
			{&CPU::ADC, 4, "adc", AddressingMode::DirectPageIndexedByX,             2}, // 75
			{&CPU::ROR, 6, "ror", AddressingMode::DirectPageIndexedByX,             2}, // 76
			{&CPU::ADC, 6, "adc", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 77
			{&CPU::SEI, 2, "sei", AddressingMode::Implied,                          1}, // 78
			{&CPU::ADC, 4, "adc", AddressingMode::AbsoluteIndexedByY,               2}, // 79
			{&CPU::PLY, 4, "ply", AddressingMode::Implied,                          1}, // 7A
			{&CPU::TDC, 2, "tdc", AddressingMode::Implied,                          1}, // 7B
			{&CPU::JMP, 6, "jmp", AddressingMode::AbsoluteIndirectIndexedByX,       3}, // 7C
			{&CPU::ADC, 4, "adc", AddressingMode::AbsoluteIndexedByX,               3}, // 7D
			{&CPU::ROR, 7, "ror", AddressingMode::AbsoluteIndexedByX,               3}, // 7E
			{&CPU::ADC, 5, "adc", AddressingMode::AbsoluteIndexedByXLong,           4}, // 7F
			{&CPU::BRA, 3, "bra", AddressingMode::Immediate8bits,                   2}, // 80
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndexedByX,             2}, // 81
			{&CPU::BRL, 4, "brl", AddressingMode::Absolute,                         3}, // 82
			{&CPU::STA, 4, "sta", AddressingMode::StackRelative,                    2}, // 83
			{&CPU::STY, 3, "sty", AddressingMode::DirectPage,                       2}, // 84
			{&CPU::STA, 3, "sta", AddressingMode::DirectPage,                       2}, // 85
			{&CPU::STX, 3, "stx", AddressingMode::DirectPage,                       2}, // 86
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectLong,           2}, // 87
			{&CPU::DEY, 2, "dey", AddressingMode::Implied,                          1}, // 88
			{&CPU::BIT, 2, "bit", AddressingMode::ImmediateForA,                    2}, // 89
			{&CPU::TXA, 2, "txa", AddressingMode::Implied,                          2}, // 8A
			{&CPU::PHB, 3, "phb", AddressingMode::Implied,                          1}, // 8B
			{&CPU::STY, 4, "sty", AddressingMode::Absolute,                         3}, // 8C
			{&CPU::STA, 4, "sta", AddressingMode::Absolute,                         3}, // 8D
			{&CPU::STX, 4, "stx", AddressingMode::Absolute,                         3}, // 8E
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteLong,                     4}, // 8F
			{&CPU::BCC, 2, "bcc", AddressingMode::Immediate8bits,                   2}, // 90
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectIndexedByY,     2}, // 91
			{&CPU::STA, 5, "sta", AddressingMode::DirectPageIndirect,               2}, // 92
			{&CPU::STA, 7, "sta", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // 93
			{&CPU::STY, 4, "sty", AddressingMode::DirectPageIndirectIndexedByX,     2}, // 94
			{&CPU::STA, 4, "sta", AddressingMode::DirectPageIndexedByX,             2}, // 95
			{&CPU::STX, 4, "stx", AddressingMode::DirectPageIndexedByY,             2}, // 96
			{&CPU::STA, 6, "sta", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // 97
			{&CPU::TYA, 2, "tya", AddressingMode::Implied,                          1}, // 98
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByY,               3}, // 99
			{&CPU::TXS, 2, "txs", AddressingMode::Implied,                          1}, // 9A
			{&CPU::TXY, 2, "txy", AddressingMode::Implied,                          1}, // 9B
			{&CPU::STZ, 4, "stz", AddressingMode::Absolute,                         3}, // 9C
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByX,               3}, // 9D
			{&CPU::STZ, 5, "stz", AddressingMode::AbsoluteIndexedByX,               3}, // 9E
			{&CPU::STA, 5, "sta", AddressingMode::AbsoluteIndexedByXLong,           4}, // 9F
			{&CPU::LDY, 2, "ldy", AddressingMode::ImmediateForX,                    2}, // A0
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectIndexedByX,     2}, // A1
			{&CPU::LDX, 2, "ldx", AddressingMode::ImmediateForX,                    2}, // A2
			{&CPU::LDA, 4, "lda", AddressingMode::StackRelative,                    2}, // A3
			{&CPU::LDY, 3, "ldy", AddressingMode::DirectPage,                       2}, // A4
			{&CPU::LDA, 3, "lda", AddressingMode::DirectPage,                       2}, // A5
			{&CPU::LDX, 3, "ldx", AddressingMode::DirectPage,                       2}, // A6
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectLong,           2}, // A7
			{&CPU::TAY, 2, "tay", AddressingMode::Implied,                          1}, // A8
			{&CPU::LDA, 2, "lda", AddressingMode::ImmediateForA,                    2}, // A9
			{&CPU::TAX, 2, "tax", AddressingMode::Implied,                          1}, // AA
			{&CPU::PLB, 4, "plb", AddressingMode::Implied,                          1}, // AB
			{&CPU::LDY, 4, "ldy", AddressingMode::Absolute,                         4}, // AC
			{&CPU::LDA, 4, "lda", AddressingMode::Absolute,                         3}, // AD
			{&CPU::LDX, 4, "ldx", AddressingMode::Absolute,                         3}, // AE
			{&CPU::LDA, 5, "lda", AddressingMode::AbsoluteLong,                     4}, // AF
			{&CPU::BCS, 2, "bcs", AddressingMode::Immediate8bits,                   2}, // B0
			{&CPU::LDA, 5, "lda", AddressingMode::DirectPageIndirectIndexedByY,     2}, // B1
			{&CPU::LDA, 5, "lda", AddressingMode::DirectPageIndirect,               2}, // B2
			{&CPU::LDA, 7, "lda", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // B3
			{&CPU::LDY, 4, "ldy", AddressingMode::DirectPageIndexedByX,             2}, // B4
			{&CPU::LDA, 4, "lda", AddressingMode::DirectPageIndexedByX,             2}, // B5
			{&CPU::LDX, 4, "ldx", AddressingMode::DirectPageIndexedByY,             2}, // B6
			{&CPU::LDA, 6, "lda", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // B7
			{&CPU::CLV, 7, "clv", AddressingMode::Implied,                          1}, // B8
			{&CPU::LDA, 4, "lda", AddressingMode::AbsoluteIndexedByY,               3}, // B9
			{&CPU::TSX, 2, "tsx", AddressingMode::Implied,                          1}, // BA
			{&CPU::TYX, 2, "tyx", AddressingMode::Implied,                          1}, // BB
			{&CPU::LDY, 4, "ldy", AddressingMode::AbsoluteIndexedByX,               3}, // BC
			{&CPU::LDA, 4, "lda", AddressingMode::AbsoluteIndexedByX,               3}, // BD
			{&CPU::LDX, 4, "ldx", AddressingMode::AbsoluteIndexedByY,               3}, // BE
			{&CPU::LDA, 5, "lda", AddressingMode::AbsoluteIndexedByXLong,           4}, // BF
			{&CPU::CPY, 2, "cpy", AddressingMode::ImmediateForX,                    2}, // C0
			{&CPU::CMP, 6, "cmp", AddressingMode::DirectPageIndirectIndexedByX,     2}, // C1
			{&CPU::REP, 3, "rep", AddressingMode::Immediate8bits,                   2}, // C2
			{&CPU::CMP, 4, "cmp", AddressingMode::StackRelative,                    2}, // C3
			{&CPU::CPY, 3, "cpy", AddressingMode::DirectPage,                       2}, // C4
			{&CPU::CMP, 3, "cmp", AddressingMode::DirectPage,                       2}, // C5
			{&CPU::DEC, 5, "dec", AddressingMode::DirectPage,                       2}, // C6
			{&CPU::CMP, 6, "cmp", AddressingMode::DirectPageIndirectLong,           2}, // C7
			{&CPU::INY, 2, "iny", AddressingMode::Implied,                          1}, // C8
			{&CPU::CMP, 2, "cmp", AddressingMode::ImmediateForA,                    2}, // C9
			{&CPU::DEX, 2, "dex", AddressingMode::Implied,                          1}, // CA
			{&CPU::WAI, 3, "wai", AddressingMode::Implied,                          1}, // CB
			{&CPU::CPY, 4, "cpy", AddressingMode::Absolute,                         3}, // CC
			{&CPU::CMP, 4, "cmp", AddressingMode::Absolute,                         3}, // CD
			{&CPU::DEC, 6, "dec", AddressingMode::Absolute,                         3}, // CE
			{&CPU::CMP, 6, "cmp", AddressingMode::AbsoluteLong,                     4}, // CF
			{&CPU::BNE, 2, "bne", AddressingMode::Immediate8bits,                   2}, // D0
			{&CPU::CMP, 5, "cmp", AddressingMode::DirectPageIndirectIndexedByY,     2}, // D1
			{&CPU::CMP, 5, "cmp", AddressingMode::DirectPageIndirect,               2}, // D2
			{&CPU::CMP, 7, "cmp", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // D3
			{&CPU::PEI, 6, "pei", AddressingMode::DirectPage,                       2}, // D4
			{&CPU::CMP, 4, "cmp", AddressingMode::DirectPageIndexedByX,             2}, // D5
			{&CPU::DEC, 6, "dec", AddressingMode::DirectPageIndexedByX,             2}, // D6
			{&CPU::CMP, 6, "cmp", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // D7
			{&CPU::CLD, 2, "cld", AddressingMode::Implied,                          2}, // D8
			{&CPU::CMP, 4, "cmp", AddressingMode::AbsoluteIndexedByY,               3}, // D9
			{&CPU::PHX, 3, "phx", AddressingMode::Implied,                          1}, // DA
			{&CPU::STP, 3, "stp", AddressingMode::Implied,                          1}, // DB
			{&CPU::JML, 7, "jml", AddressingMode::AbsoluteIndirectLong,             2}, // DC
			{&CPU::CMP, 4, "cmp", AddressingMode::AbsoluteIndexedByX,               3}, // DD
			{&CPU::DEC, 7, "dec", AddressingMode::AbsoluteIndexedByX,               3}, // DE
			{&CPU::CMP, 5, "cmp", AddressingMode::AbsoluteIndexedByXLong,           4}, // DF
			{&CPU::CPX, 2, "cpx", AddressingMode::ImmediateForX,                    2}, // E0
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectIndexedByX,     2}, // E1
			{&CPU::SEP, 3, "sep", AddressingMode::Immediate8bits,                   2}, // E2
			{&CPU::SBC, 4, "sbc", AddressingMode::StackRelative,                    2}, // E3
			{&CPU::CPX, 3, "cpx", AddressingMode::DirectPage,                       2}, // E4
			{&CPU::SBC, 3, "sbc", AddressingMode::DirectPage,                       2}, // E5
			{&CPU::INC, 5, "inc", AddressingMode::DirectPage,                       2}, // E6
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectLong,           2}, // E7
			{&CPU::INX, 2, "inx", AddressingMode::Implied,                          1}, // E8
			{&CPU::SBC, 2, "sbc", AddressingMode::ImmediateForA,                    2}, // E9
			{&CPU::NOP, 2, "nop", AddressingMode::Implied,                          1}, // EA
			{&CPU::XBA, 3, "xba", AddressingMode::Implied,                          1}, // EB
			{&CPU::CPX, 4, "cpx", AddressingMode::Absolute,                         3}, // EC
			{&CPU::SBC, 4, "sbc", AddressingMode::Absolute,                         3}, // ED
			{&CPU::INC, 6, "inc", AddressingMode::Absolute,                         3}, // EE
			{&CPU::SBC, 5, "sbc", AddressingMode::AbsoluteLong,                     4}, // EF
			{&CPU::BEQ, 2, "beq", AddressingMode::Immediate8bits,                   2}, // F0
			{&CPU::SBC, 5, "sbc", AddressingMode::DirectPageIndirectIndexedByY,     2}, // F1
			{&CPU::SBC, 5, "sbc", AddressingMode::DirectPageIndirect,               2}, // F2
			{&CPU::SBC, 7, "sbc", AddressingMode::StackRelativeIndirectIndexedByY,  2}, // F3
			{&CPU::PEA, 5, "pea", AddressingMode::Immediate16bits,                  3}, // F4
			{&CPU::SBC, 4, "sbc", AddressingMode::DirectPageIndexedByX,             2}, // F5
			{&CPU::INC, 6, "inc", AddressingMode::DirectPageIndexedByX,             2}, // F6
			{&CPU::SBC, 6, "sbc", AddressingMode::DirectPageIndirectIndexedByYLong, 2}, // F7
			{&CPU::SED, 2, "sed", AddressingMode::Implied,                          1}, // F8
			{&CPU::SBC, 4, "sbc", AddressingMode::AbsoluteIndexedByY,               3}, // F9
			{&CPU::PLX, 4, "plx", AddressingMode::Implied,                          1}, // FA
			{&CPU::XCE, 2, "xce", AddressingMode::Implied,                          1}, // FB
			{&CPU::JSR, 8, "jsr", AddressingMode::AbsoluteIndirectIndexedByX,       3}, // FC
			{&CPU::SBC, 4, "sbc", AddressingMode::AbsoluteIndexedByX,               3}, // FD
			{&CPU::INC, 7, "inc", AddressingMode::AbsoluteIndexedByX,               3}, // FE
			{&CPU::SBC, 5, "sbc", AddressingMode::AbsoluteIndexedByXLong,           4}, // FF
		};

		//! @brief Construct a new generic CPU.
		//! @param bus The memory bus to use to transfer data.
		//! @param cartridgeHeader The header used to know interrupts, main entry point etc...
		CPU(Memory::IMemoryBus &bus, Cartridge::Header &cartridgeHeader);
		//! @brief A default copy constructor
		CPU(const CPU &) = default;
		//! @brief A CPU is not assignable
		CPU &operator=(const CPU &) = delete;
		//! @brief A default destructor
		~CPU() override = default;

		//! @brief This function continue to execute the Cartridge code.
		//! @param maxCycle The maximum number of cycle to run.
		//! @return The number of CPU cycles that elapsed
		unsigned update(unsigned maxCycle);

		//! @brief Execute a single instruction.
		//! @return The number of CPU cycles that the instruction took.
		unsigned executeInstruction();

		//! @brief Run DMA's pending transfers.
		//! @param maxCycles The maximum of cycle to run
		//! @return The number of CPU cycles that elapsed
		unsigned runDMA(unsigned maxCycles);

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

		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		[[nodiscard]] std::string getValueName(uint24_t addr) const override;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const override;

		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const override;

		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const override;

		//! @brief Reset interrupt - Called on boot and when the reset button is pressed.
		//! @note This also triggers the callback onReset;
		int RESB();

		//! @brief The callback triggered on reset.
		Callback<> onReset;

		//! @brief Is an NMI (non-maskable interrupt) requested.
		bool IsNMIRequested = false;
		//! @brief Is an interrupt (maskable) requested.
		bool IsIRQRequested = false;
		//! @brief Is an abort requested
		bool IsAbortRequested = false;

		//! @brief True if you want to disable updates of this CPU.
		bool isDisabled = false;

#ifdef DEBUGGER_ENABLED
		friend Debugger::CPU::CPUDebug;
		friend Debugger::RegisterViewer;
#endif
	};
}