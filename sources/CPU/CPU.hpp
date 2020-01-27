//
// Created by anonymus-raccoon on 1/24/20.
//

#ifndef COMSQUARE_CPU_HPP
#define COMSQUARE_CPU_HPP

#include "../Memory/IMemory.hpp"
#include "../Memory/MemoryBus.hpp"

namespace ComSquare::CPU
{
	//! @brief Struct containing registers for the main CPU.
	struct Registers {
		//! @brief The Accumulator
		union {
			struct {
				unsigned char ah;
				unsigned char al;
			};
			unsigned short a;
		};
		//! @brief The Data Bank Register;
		unsigned char dbr;
		//! @brief The Direct register;
		union {
			struct {
				unsigned char dh;
				unsigned char dl;
			};
			unsigned short d;
		};
		//! @brief The program banK register;
		unsigned char k;
		//! @brief The Program Counter;
		union {
			struct {
				unsigned char pch;
				unsigned char pcl;
			};
			unsigned short pc;
		};
		//! @brief The Stack pointer
		union {
			struct {
				unsigned char sh;
				unsigned char sl;
			};
			unsigned short s;
		};
		//! @brief The X index register
		union {
			struct {
				unsigned char xh;
				unsigned char xl;
			};
			unsigned short x;
		};
		//! @brief The Y index register
		union {
			struct {
				unsigned char yh;
				unsigned char yl;
			};
			unsigned short y;
		};

		//!	@brief The Emulation mode flag
		bool e;
		//! @brief The Processor status register;
		union p {
			//!	@brief The Negative flag
			bool n : 1;
			//! @brief The oVerflow flag
			bool v : 1;
			//! @brief The accumulator and Memory width flag (in native mode only)
			bool m : 1;
			union {
				//!	@brief The indeX register width flag (in native mode only)
				bool x : 1;
				//! @brief The Break flag (in emulation mode only)
				bool b : 1;
			};
			//!	@brief The Decimal mode flag
			bool d : 1;
			//!	@brief The Interrupt disable flag
			bool i : 1;
			//! @brief The Zero flag
			bool z : 1;
			//!	@brief The Carry flag
			bool c : 1;
		};
	};

	//! @brief The main CPU
	class CPU {
	private:
		//! @brief All the registers of the CPU
		Registers _registers;
		//! @brief Is the CPU running in emulation mode (in 8bits)
		bool _isEmulationMode;
		//! @brief The memory bus to use for read/write.
		std::shared_ptr<MemoryBus> _bus;

		//! @brief Execute a single instruction.
		//! @return The number of CPU cycles that the instruction took.
		int executreInstruction();
	public:
		explicit CPU(std::shared_ptr<MemoryBus> bus);
		//! @brief This function continue to execute the Cartridge code.
		//! @return The number of CPU cycles that elapsed
		int update();
	};
}

#endif //COMSQUARE_CPU_HPP
