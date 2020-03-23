//
// Created by Melefo on 24/01/2020.
//

#ifndef COMSQUARE_APU_HPP
#define COMSQUARE_APU_HPP

#include <memory>
#include "DSP/DSP.hpp"
#include "../Memory/IMemory.hpp"
#include "../Ram/Ram.hpp"

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
				//! @brief Negative flag
				bool n : 1;
				//! @brief Overflow flag
				bool v : 1;
				//! @brief Direct page flag
				bool p : 1;
				//! @brief Break flag
				bool b : 1;
				//! @brief Half carry flag
				bool h : 1;
				//! @brief Interrupt enabled flag
				bool i : 1;
				//! @brief Zero flag
				bool z : 1;
				//! @brief Carry flag
				bool c : 1;
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
		//! @brief DSP Register data register
		uint8_t dspregData;

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
		Ram::Ram IPL;

		MemoryMap();
		MemoryMap(const MemoryMap &) = delete;
		MemoryMap &operator=(const MemoryMap &) = delete;
		~MemoryMap() = default;
	};

	class APU : public Memory::IMemory {
	protected:
		//! @brief All the registers of the APU CPU
		Registers _registers{};
		//! @brief Internal registers of the CPU (accessible from the bus via addr $4200 to $421F).
		InternalRegisters _internalRegisters{};

		//! @brief Internal APU memory separated according to their utility
		std::shared_ptr<MemoryMap> _map;

		//! @brief The DSP component used to produce sound
		std::shared_ptr<DSP::DSP> _dsp;

		//! @brief Read from the APU ram.
		//! @param addr The address to read from. The address 0x0000 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		//! @return Return the data.
		uint8_t _internalRead(uint24_t addr);
		//! @brief Write data to the APU ram.
		//! @param addr The address to write to. The address 0x0000 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FFFF (the number of register).
		void _internalWrite(uint24_t addr, uint8_t data);

		//! @brief Current state of APU CPU
		StateMode _state = Running;

		//! @brief Keep the number of excess cycles executed to pad the next update
		unsigned int _paddingCycles = 0;

		//! @brief Get direct page offset
		uint24_t _getDirectAddr();
		//! @brief Get absolute direct page offset
		uint24_t _getAbsoluteAddr();
		//! @brief Get absolute offset and separate its bits
		std::pair<uint24_t, uint24_t> _getAbsoluteBit();

		//! @brief Execute a single instruction.
		//! @return The number of cycles that the instruction took.
		virtual int _executeInstruction();

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
		int BRA();
		//! @brief Branch if Zero Flag is set
		int BEQ();
		//! @brief Branch if Zero Flag is clear
		int BNE();
		//! @brief Branch if Carry Flag is set
		int BCS();
		//! @brief Branch if Carry Flag is clear
		int BCC();
		//! @brief Branch if Overflow Flag is set
		int BVS();
		//! @brief Branch if Overflow Flag is set
		int BVC();
		//! @brief Branch if Negative Flag is set
		int BMI();
		//! @brief Branch if Negative Flag is clear
		int BPL();
	public:
		explicit APU(std::shared_ptr<MemoryMap> &map);
		APU(const APU &) = default;
		APU &operator=(const APU &) = default;
		~APU() = default;

		//! @brief Read from the internal APU register.
		//! @param addr The address to read from. The address 0x00 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $0F (the number of register).
		//! @return Return the value of the register.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the internal APU register.
		//! @param addr The address to write to. The address 0x00 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $0F (the number of register).
		void write(uint24_t addr, uint8_t data) override;
		//! @brief This function execute the instructions received until the maximum number of cycles is reached.
		//! @return The number of cycles that elapsed.
		virtual void update(unsigned cycles);

		//! @brief This function is executed when the SNES is powered on or the reset button is pushed.
		void reset();

		//! @brief Return true if the CPU is overloaded with debugging features.
		virtual bool isDebugger();
	};
}

#endif //COMSQUARE_APU_HPP
