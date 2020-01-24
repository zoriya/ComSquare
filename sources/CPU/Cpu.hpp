//
// Created by anonymus-raccoon on 1/24/20.
//

#ifndef COMSQUARE_CPU_HPP
#define COMSQUARE_CPU_HPP

#include "../Memory/IMemory.hpp"

namespace ComSquare::CPU
{
	//! @brief Struct containing registers for the main CPU.
	struct Registers {
		//! @brief The Accumulator
		union {
			struct {
				unsigned char al;
				unsigned char ah;
			};
			unsigned short a;
		};
		//! @brief The Data Bank Register;
		unsigned char dbr;
		//! @brief The Direct register;
		union {
			struct {
				unsigned char dl;
				unsigned char dh;
			};
			unsigned short d;
		};
		//! @brief The program banK register;
		unsigned char k;
		//! @brief The Program Counter;
		union {
			struct {
				unsigned char pcl;
				unsigned char pch;
			};
			unsigned short pc;
		};
		//! @brief The Stack pointer
		union {
			struct {
				unsigned char sl;
				unsigned char sh;
			};
			unsigned short s;
		};
		//! @brief The X index register
		union {
			struct {
				unsigned char xl;
				unsigned char xh;
			};
			unsigned short x;
		};
		//! @brief The Y index register
		union {
			struct {
				unsigned char yl;
				unsigned char yh;
			};
			unsigned short y;
		};

		//!	@brief The Emulation mode flag
		bool e;
		//! @brief The Processor status register;
		union p {
			//!	@brief The Carry flag
			bool c : 1;
			//! @brief The Zero flag
			bool z : 1;
			//!	@brief The Interrupt disable flag
			bool i : 1;
			//!	@brief The Decimal mode flag
			bool d : 1;
			union {
				//!	@brief The indeX register width flag (in native mode only)
				bool x : 1;
				//! @brief The Break flag (in emulation mode only)
				bool b : 1;
			};
			//! @brief The accumulator and Memory width flag (in native mode only)
			bool m : 1;
			//! @brief The oVerflow flag
			bool v : 1;
			//!	@brief The Negative flag
			bool n : 1;
		};
	};

	class CPU : IMemory {
	private:
		Registers _registers;
	};
}

#endif //COMSQUARE_CPU_HPP
