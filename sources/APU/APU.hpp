//
// Created by Melefo on 24/01/2020.
//

#ifndef COMSQUARE_APU_HPP
#define COMSQUARE_APU_HPP

#include <memory>
#include "../Memory/IMemory.hpp"

namespace ComSquare::APU
{
	struct Registers {

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
		union psw {
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
	};

	struct InternalRegisters
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

	class DSP {
	};

	class APU : public IMemory {
	private:
		Registers _registers;
		InternalRegisters _internalRegisters{};
	public:
		explicit APU();

		//! @brief The DSP component used to produce sound
		std::shared_ptr<DSP> _dsp;

		//! @brief Read from the internal APU register.
		//! @param addr The address to read from. The address 0xF0 should refer to the first byte of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FF (the number of register).
		//! @return Return the value of the register.
		uint8_t read(uint24_t addr) override;
		//! @brief Write data to the internal APY register.
		//! @param addr The address to write to. The address 0xF0 should refer to the first byte of register.
		//! @param data The new value of the register.
		//! @throw InvalidAddress will be thrown if the address is more than $FF (the number of register).
		void write(uint24_t addr, uint8_t data) override;
		bool update();
	};
}

#endif //COMSQUARE_APU_HPP
