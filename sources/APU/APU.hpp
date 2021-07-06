//
// Created by Melefo on 24/01/2020.
//

#pragma once

#include <memory>
#include "DSP/DSP.hpp"
#include "Memory/AMemory.hpp"
#include "Ram/Ram.hpp"
#include "IPL/IPL.hpp"
#include "Renderer/IRenderer.hpp"
#include "Cartridge/Cartridge.hpp"

#ifdef DEBUGGER_ENABLED
#include "Debugger/APUDebug.hpp"
#endif

namespace ComSquare::APU
{
	struct InternalRegisters {

		//! @brief The X index register
		uint8_t x;

		//! @brief The YA register
		union {
			struct {
				//! @brief The Accumulator register
				uint8_t a;
				//! @brief The Y Index register
				uint8_t y;
			};
			uint16_t ya;
		};

		//! @brief The Stack pointer register
		uint8_t sp;

		//! @brief The Program counter register
		union {
			struct {
				uint8_t pcl;
				uint8_t pch;
			};
			uint16_t pc;
		};

		//! @brief Program Status Word register
		union {
			struct {
				//! @brief Carry flag
				bool c : 1;
				//! @brief Zero flag
				bool z : 1;
				//! @brief Interrupt enabled flag
				bool i : 1;
				//! @brief Half carry flag
				bool h : 1;
				//! @brief Break flag
				bool b : 1;
				//! @brief Direct page flag
				bool p : 1;
				//! @brief Overflow flag
				bool v : 1;
				//! @brief Negative flag
				bool n : 1;
			};
			uint8_t psw;
		};
	};

	struct Registers
	{
		//! @brief An undocumented register
		uint8_t unknown;

		//! @brief Control Register register
		uint8_t ctrlreg;

		//! @brief DSP Register Address register
		uint8_t dspregAddr;

		//! @brief Port 0 register
		uint8_t port0;
		//! @brief Port 1 register
		uint8_t port1;
		//! @brief Port 2 register
		uint8_t port2;
		//! @brief Port 3 register
		uint8_t port3;

		//! @brief Regular Memory register
		uint8_t regmem1;
		//! @brief Another Regular Memory register
		uint8_t regmem2;

		//! @brief Timer-0 register
		uint8_t timer0;
		//! @brief Timer-1 register
		uint8_t timer1;
		//! @brief Timer-2 register
		uint8_t timer2;

		//! @brief Counter-0 register
		uint8_t counter0;
		//! @brief Counter-1 register
		uint8_t counter1;
		//! @brief Counter-2 register
		uint8_t counter2;

	};

	enum StateMode
	{
		Running,
		Sleeping,
		Stopped
	};

	struct MemoryMap
	{
		//! @brief Zero page memory
		Ram::Ram Page0;
		//! @brief Stack space memory
		Ram::Ram Page1;
		//! @brief Any-use memory
		Ram::Ram Memory;
		//! @brief IPL ROM
		IPL::IPL IPL;

		MemoryMap();
		MemoryMap(const MemoryMap &) = delete;
		MemoryMap &operator=(const MemoryMap &) = delete;
		~MemoryMap() = default;
	};

	class APU : public Memory::AMemory {
	private:
		//! @brief All the registers of the APU CPU
		Registers _registers{};
		//! @brief Internal registers of the CPU (accessible from the bus via addr $4200 to $421F).
		InternalRegisters _internalRegisters{};

		//! @brief Internal APU memory separated according to their utility
		MemoryMap _map;

		//! @brief The DSP component used to produce sound
		DSP::DSP _dsp;

		//! @brief Read from the APU ram.
		//! @param addr The address to read from. The address 0x0000 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		//! @return Return the data.
		[[nodiscard]] uint8_t _internalRead(uint24_t addr) const;

		//! @brief Write data to the APU ram.
		//! @param addr The address to write to. The address 0x0000 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		void _internalWrite(uint24_t addr, uint8_t data);

		//! @brief Current state of APU CPU
		StateMode _state = Running;

		//! @brief Keep the number of excess cycles executed to pad the next update
		unsigned int _paddingCycles = 0;

		//! @brief Get value of the Pointer Counter
		uint8_t _getImmediateData();
		//! @brief Get direct page offset
		uint24_t _getDirectAddr();
		//! @brief Get Index X offset
		uint24_t _getIndexXAddr();
		//! @brief Get Index Y offset
		uint24_t _getIndexYAddr();
		//! @brief Get direct page offset and add to it the X Index Flag
		uint24_t _getDirectAddrByX();
		//! @brief Get direct page offset and add to it the Y Index Flag
		uint24_t _getDirectAddrByY();
		//! @brief Get absolute direct page offset
		uint24_t _getAbsoluteAddr();
		//! @brief _get absolute direct page + X Index offset
		uint24_t _getAbsoluteByXAddr();
		//! @brief Get absolute direct page offset and add to it the X Index Flag
		uint24_t _getAbsoluteAddrByX();
		//! @brief Get absolute direct page offset and add to it the Y Index Flag
		uint24_t _getAbsoluteAddrByY();
		//! @brief Get absolute offset of the direct page added to the X Index Flag
		uint24_t _getAbsoluteDirectByXAddr();
		//! @brief Get absolute offset of the direct page and add the Y Index Flag to it
		uint24_t _getAbsoluteDirectAddrByY();
		//! @brief Get absolute offset and separate its bits
		std::pair<uint24_t, uint24_t> _getAbsoluteBit();

		//! @brief Set Negative and Zero flags with value after an instruction
		void _setNZflags(uint8_t value);

		//! @brief Execute a single instruction.
		//! @return The number of cycles that the instruction took.
		int _executeInstruction();

		//! @brief No Operation instruction, do nothing than delay
		int NOP();
		//! @brief Sleep instruction, halts the processor with SLEEP mode
		int SLEEP();
		//! @brief Stop instruction, halts the processor with STOP mode
		int STOP();

		//! @brief Clear Carry instruction, set Carry flag to 0
		int CLRC();
		//! @brief Set Carry instruction, Set Carry flag to 1
		int SETC();
		//! @brief Complement Carry instruction, invert Carry flag value
		int NOTC();
		//! @brief Clear Overflow instruction, Set Overflow & Half Carry flags to 0
		int CLRV();
		//! @brief Clear Direct Page instruction, Set Direct Page flag to 0
		int CLRP();
		//! @brief Set Direct Page instruction, Set Direct Page flag to 1
		int SETP();
		//! @brief Enable interrupts instruction, Set Zero flag to 1
		int EI();
		//! @brief Disable interrupts instruction, Set Zero flag to 0
		int DI();

		//! @brief Set 1-bit instruction, set a bit in direct page
		int SET1(uint24_t dp, uint8_t bit);
		//! @brief Clear 1-bit instruction, clear a bit in direct page
		int CLR1(uint24_t dp, uint8_t bit);
		//! @brief test set 1-bit instruction, Test and set bits with absolute address
		int TSET1(uint24_t abs);
		//! @brief test clear 1-bit instruction, Test and clear bits with absolute address
		int TCLR1(uint24_t abs);
		//! @brief Performs a bitwise AND on the value or inverse value of the specified bit with Carry flag and stores the result in the Carry flag.
		int AND1(std::pair<uint24_t, uint24_t> operand, bool invert = false);
		//! @brief Performs a bitwise OR on the value or inverse value of the specified bit with Carry flag and stores the result in the Carry flag.
		int OR1(std::pair<uint24_t, uint24_t> operand, bool invert = false);
		//! @brief Performs a exclusive OR on the value of the bit specified with Carry flag and stores the result in the Carry flag.
		int EOR1(std::pair<uint24_t, uint24_t> operand);
		//! @brief Performs a logical NOT on the value of the specified bit and stores the result.
		int NOT1(std::pair<uint24_t, uint24_t> operand);
		//! @brief Either moves the specified bit into carry or moves carry into the specified bit.
		int MOV1(std::pair<uint24_t, uint24_t> operand, bool to_carry = false);

		//! @brief Push a value onto the stack and decrement SP Register.
		int PUSH(uint8_t value);
		//! @brief Increment SP Register and pop a single value from the stack.
		int POP(uint8_t &destination);

		//! @brief Push PC of the next instruction on the stack, then jump to the address at the specified location.
		int CALL(uint24_t abs);
		//! @brief Perform a call in the upper page of memory, read PC Register and add 0xFF00 to it.
		int PCALL();
		//! @brief Performs a call on one of the 16 vectors in the memory range of $FFC0 to $FFDF.
		int TCALL(uint8_t bit);
		//! @brief Cause a software interrupt.
		int BRK();
		//! @brief Return from subroutine.
		int RET();
		//! @brief Return from interrupt.
		int RETI();

		//! @brief Branch Always, go to the specified location from the next instruction.
		int BRA(int8_t offset);
		//! @brief Branch if Zero Flag is set.
		int BEQ(int8_t offset);
		//! @brief Branch if Zero Flag is clear.
		int BNE(int8_t offset);
		//! @brief Branch if Carry Flag is set.
		int BCS(int8_t offset);
		//! @brief Branch if Carry Flag is clear.
		int BCC(int8_t offset);
		//! @brief Branch if Overflow Flag is set.
		int BVS(int8_t offset);
		//! @brief Branch if Overflow Flag is set.
		int BVC(int8_t offset);
		//! @brief Branch if Negative Flag is set.
		int BMI(int8_t offset);
		//! @brief Branch if Negative Flag is clear.
		int BPL(int8_t offset);
		//! @brief Branch if the specified is set in the address, go to the specified location from the next instruction.
		int BBS(uint24_t addr, int8_t offset, uint8_t bit);
		//! @brief Branch if the specified is clear in the address, go to the specified location from the next instruction.
		int BBC(uint24_t addr, int8_t offset, uint8_t bit);
		//! @brief Branch if the value at the specified address is not equal to the Accumulator Flag.
		int CBNE(uint24_t addr, int8_t offset, bool by_x = false);
		//! @brief Decrement a value then branch to the specified location if the value is not zero.
		int DBNZ(int8_t offset, bool direct_addr = false);
		//! @brief Jump to the specified location.
		int JMP(uint24_t addr, bool by_x = false);

		//! @brief Decimal adjust A for addition.
		int DAA();
		//! @brief Decimal adjust A for subtraction.
		int DAS();

		//! @brief Store the 16-bit value of Y * A into YA
		int MUL();
		//! @brief Divide the 16-bit value YA by X, storing the quotient in A and the remainder in Y.
		int DIV();

		//! @brief Increment a word value.
		int INCW(uint24_t addr);
		//! @brief Decrement a word value.
		int DECW(uint24_t addr);
		//! @brief Add YA with a word value.
		int ADDW(uint24_t addr);
		//! @brief Subtract YA with a word value.
		int SUBW(uint24_t addr);
		//! @brief Compare YA with a word value.
		int CMPW(uint24_t addr);

		//! @brief Sets a word value equal to another.
		int MOVW(uint24_t addr, bool to_ya = false);

		//! @brief Arithmetic Shift Left.
		int ASL(uint24_t operand, int cycles, bool accumulator = false);
		//! @brief Logical Shift Right.
		int LSR(uint24_t operand, int cycles, bool accumulator = false);
		//! @brief Rotate Bits Left.
		int ROL(uint24_t operand, int cycles, bool accumulator = false);
		//! @brief Rotate Bits Right.
		int ROR(uint24_t operand, int cycles, bool accumulator = false);
		//! @brief Exchange Nibbles.
		int XCN();

		//! @brief Increment a value at an address.
		int INC(uint24_t addr, int cycles);
		//! @brief Increment a register.
		int INCreg(uint8_t &value);
		//! @brief Decrement a value at an address.
		int DEC(uint24_t addr, int cycles);
		//! @brief Decrement a register.
		int DECreg(uint8_t &value);

		//! @brief Perform a bitwise AND.
		int AND(uint24_t operand1, uint24_t operand2, int cycles);
		//! @brief Perform a bitwise AND on the Accumulator flag.
		int ANDacc(uint24_t addr, int cycles);
		//! @brief Perform a bitwise OR.
		int OR(uint24_t operand1, uint24_t operand2, int cycles);
		//! @brief Perform a bitwise OR on the Accumulator flag.
		int ORacc(uint24_t addr, int cycles);
		//! @brief Perform an Exclusive OR.
		int EOR(uint24_t operand1, uint24_t operand2, int cycles);
		//! @brief Perform an Exclusive OR on the Accumulator flag.
		int EORacc(uint24_t addr, int cycles);

		//! @brief Add operand1 with operand2 and carry.
		int ADC(uint24_t operand1, uint24_t operand2, int cycles);
		//! !@brief Add Accumulator Flag with value at address and carry.
		int ADCacc(uint24_t addr, int cycles);
		//! @brief Subtract operand1 with operand2 and carry.
		int SBC(uint24_t operand1, uint24_t operand2, int cycles);
		//! @brief Subtract Accumulator Flag with address and carry.
		int SBCacc(uint24_t addr, int cycles);
		//! @brief Compare the two values of the operands and set NZC flags.
		int CMP(uint24_t operand1, uint24_t operand2, int cycles);
		//! @brief Compare a Register Flag with the value of the operand and set NZC flags.
		int CMPreg(uint8_t  &reg, uint24_t addr, int cycles);

		int MOV(uint8_t &regFrom, uint8_t &regTo, bool setFlags = true);
		int MOV(uint8_t &regFrom, uint24_t memTo, int cycles, bool incrementX = false);
		int MOV(uint24_t memFrom, uint8_t &regTo, int cycles, bool incrementX = false);
		int MOV(uint24_t memFrom, uint24_t memTo);
	public:
		explicit APU(Renderer::IRenderer &renderer);
		APU(const APU &) = default;
		APU &operator=(const APU &) = delete;
		~APU() override = default;

		//! @brief Is this APU disabled?
		bool isDisabled = false;

		//! @brief Read from the APU ram.
		//! @param addr The address to read from. The address 0x0000 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		//! @return Return the data.
		uint8_t read(uint24_t addr) override;

		//! @brief Write data to the APU ram.
		//! @param addr The address to write to. The address 0x0000 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		void write(uint24_t addr, uint8_t data) override;

		//! @brief Get the name of this accessor (used for debug purpose)
		[[nodiscard]] std::string getName() const override;

		//! @brief Get the component of this accessor (used for debug purpose)
		[[nodiscard]] Component getComponent() const override;

		//! @brief Get the name of the data at the address
		//! @param addr The address (in local space)
		[[nodiscard]] std::string getValueName(uint24_t addr) const override;

		//! @brief Get the size of the data. This size can be lower than the mapped data.
		//! @return The number of bytes inside this memory.
		[[nodiscard]] uint24_t getSize() const override;

		//! @brief Parses rom data to uploads directly into RAM and corresponding registers
		void loadFromSPC(Cartridge::Cartridge &cartridge);

		//! @brief This function execute the instructions received until the maximum number of cycles is reached.
		//! @return The number of cycles that elapsed.
		void update(unsigned cycles);

		//! @brief This function is executed when the SNES is powered on or the reset button is pushed.
		void reset();

#ifdef DEBUGGER_ENABLED
		friend Debugger::APU::APUDebug;
#endif
	};
}
