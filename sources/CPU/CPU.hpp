//
// Created by anonymus-raccoon on 1/24/20.
//

#ifndef COMSQUARE_CPU_HPP
#define COMSQUARE_CPU_HPP

#include "../Memory/IMemory.hpp"
#include "../Memory/MemoryBus.hpp"
#include "../Models/Int24.hpp"
#include "../Cartridge/Cartridge.hpp"

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
				//!	@brief The indeX register _width flag (in native mode only) OR the Break flag (in emulation mode only)
				bool x_b : 1;
				//! @brief The accumulator and Memory _width flag (in native mode only)
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

	//! @brief All the instructions opcode of the main CPU.
	//! @info The name of the instruction followed by their parameters (after an underscore) if any.
	//! @info Addr mode with an i at the end means indirect.
	//! @info Addr mode with an l at the end means long.
	enum Instructions
	{
		BRK = 0x00,
		COP = 0x02,
		RTI = 0x40,

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
		ADC_ABSXl = 0x7F,

		STA_ABS = 0x8D,
		STA_ABSl = 0x8F,
		STA_DP = 0x85,
		STA_DPi = 0x92,
		STA_DPil = 0x87,
		STA_ABSX = 0x9D,
		STA_ABSXl = 0x9F,
		STA_ABSY = 0x99,
		STA_DPX = 0x95,
		STA_DPXi = 0x81,
		STA_DPYi = 0x91,
		STA_DPYil = 0x97,
		STA_SR = 0x83,
		STA_SRYi = 0x93,

		STX_ABS = 0x8E,
		STX_DP = 0x86,
		STX_DPY = 0x96,

		STY_ABS = 0x8C,
		STY_DP = 0x84,
		STY_DPX = 0x94,

		STZ_ABS = 0x9C,
		STZ_DP = 0x64,
		STZ_ABSX = 0x9E,
		STZ_DPX = 0x74,

		LDA_IM = 0xA9,
		LDA_ABS = 0xAD,
		LDA_ABSl = 0xAF,
		LDA_DP = 0xA5,
		LDA_DPi = 0xB2,
		LDA_DPil = 0xA7,
		LDA_ABSX = 0xBD,
		LDA_ABSXl = 0xBF,
		LDA_ABSY = 0xB9,
		LDA_DPX = 0xB5,
		LDA_DPXi = 0xA1,
		LDA_DPYi = 0xB1,
		LDA_DPYil = 0xB7,
		LDA_SR = 0xA3,
		LDA_SRYi = 0xB3,

		LDX_IM = 0xA2,
		LDX_ABS = 0xAE,
		LDX_DP = 0xA6,
		LDX_ABSY = 0xBE,
		LDX_DPY = 0xB6,

		LDY_IM = 0xA0,
		LDY_ABS = 0xAC,
		LDY_DP = 0xA4,
		LDY_ABSY = 0xBC,
		LDY_DPY = 0xB4,

		SEP = 0xE2,

		REP = 0xC2,

		PHA = 0x48,
		PHB = 0x8B,
		PHD = 0x0B,
		PHK = 0x4B,
		PHP = 0x08,
		PHX = 0xDA,
		PHY = 0x5A,

		PLA = 0x68,
		PLB = 0xAB,
		PLD = 0x2B,
		PLP = 0x28,
		PLX = 0xFA,
		PLY = 0x7A,

		JSR_ABS = 0x20,
		JSR_ABSXi = 0xFC,

		JSL = 0x22,

		CLC = 0x18,
		CLI = 0x58,
		CLD = 0xD8,
		CLV = 0xB8,

		SEC = 0x38,
		SEI = 0x78,
		SED = 0xF8,

		AND_IM = 0x29,
		AND_ABS = 0x2D,
		AND_ABSl = 0x2F,
		AND_DP = 0x25,
		AND_DPi = 0x32,
		AND_DPil = 0x27,
		AND_ABSX = 0x3D,
		AND_ABSXl = 0x3F,
		AND_ABSY = 0x39,
		AND_DPX = 0x35,
		AND_DPXi = 0x21,
		AND_DPYi = 0x31,
		AND_DPYil = 0x37,
		AND_SR = 0x23,
		AND_SRYi = 0x33,

		XCE = 0xFB
	};

	//! @brief The main CPU
	class CPU : public Memory::IMemory {
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


		//! @brief Execute a single instruction.
		//! @return The number of CPU cycles that the instruction took.
		virtual unsigned _executeInstruction(uint8_t opcode);

		//! @brief Break instruction - Causes a software break. The PC is loaded from a vector table.
		void BRK();
		//! @brief Co-Processor Enable instruction - Causes a software break. The PC is loaded from a vector table.
		void COP();
		//! @brief Return from Interrupt - Used to return from a interrupt handler.
		void RTI();
		//! @brief Add with carry - Adds operand to the Accumulator; adds an additional 1 if carry is set.
		//! @return The number of extra cycles that this operation took.
		void ADC(uint24_t valueAddr);
		//! @brief Store the accumulator to memory.
		void STA(uint24_t addr);
		//! @brief Store the index register X to memory.
		void STX(uint24_t addr);
		//! @brief Store the index register Y to memory.
		void STY(uint24_t addr);
		//! @brief Store zero to the memory.
		void STZ(uint24_t addr);
		//! @brief Load the accumulator from memory.
		void LDA(uint24_t addr);
		//! @brief Load the X index register from memory.
		void LDX(uint24_t addr);
		//! @brief Load the Y index register from memory.
		void LDY(uint24_t addr);
		//! @brief Set status bits.
		void SEP(uint24_t valueAddr);
		//! @brief Reset status bits.
		void REP(uint24_t valueAddr);
		//! @brief Jump to subroutine
		void JSR(uint24_t addr);
		//! @brief Jump to subroutine (long)
		void JSL(uint24_t addr);
		//! @brief Push the accumulator to the stack.
		void PHA();
		//! @brief Push the data bank register to the stack.
		void PHB();
		//! @brief Push the direct page register to the stack.
		void PHD();
		//! @brief Push the program bank register to the stack.
		void PHK();
		//! @brief Push the processor status register to the stack.
		void PHP();
		//! @brief Push the x index register to the stack.
		void PHX();
		//! @brief Push the y index register to the stack.
		void PHY();
		//! @brief Pull the accumulator to the stack.
		void PLA();
		//! @brief Pull the data bank register to the stack.
		void PLB();
		//! @brief Pull the direct page register to the stack.
		void PLD();
		//! @brief Pull the processor status register to the stack.
		void PLP();
		//! @brief Pull the x index register to the stack.
		void PLX();
		//! @brief Pull the y index register to the stack.
		void PLY();
		//! @brief Clear the carry flag.
		void CLC();
		//! @brief Clear the Interrupt Disable flag.
		void CLI();
		//! @brief Clear the decimal flag.
		void CLD();
		//! @brief Clear the overflow flag.
		void CLV();
		//! @brief Set the carry Flag.
		void SEC();
		//! @brief Set the decimal flag.
		void SED();
		//! @brief Set the Interrupt Disable flag.
		void SEI();
		//! @brief Exchange Carry and Emulation Flags
		void XCE();
		//! @brief And accumulator with memory.
		void AND(uint24_t valueAddr);
	public:
		explicit CPU(std::shared_ptr<Memory::MemoryBus> bus, Cartridge::Header &cartridgeHeader);
		CPU(const CPU &) = default;
		CPU &operator=(const CPU &) = delete;
		~CPU() = default;
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

		//! @brief Reset interrupt - Called on boot and when the reset button is pressed.
		virtual void RESB();
	};
}

#endif //COMSQUARE_CPU_HPP
