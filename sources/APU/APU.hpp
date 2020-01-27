//
// Created by Melefo on 24/01/2020.
//

#ifndef COMSQUARE_APU_HPP
#define COMSQUARE_APU_HPP

#include "../Memory/IMemory.hpp"

namespace ComSquare::APU
{
	struct Registers {
		//! @brief The Accumulator register
		unsigned char a;

		//! @brief The X index register
		unsigned char x;

		//! @brief The Y index register
		unsigned char y;

		//! @brief The Stack pointer register
		unsigned char sp;

		//! @brief The Program counter register
		union {
			struct {
				unsigned char pch;
				unsigned char pcl;
			};
			unsigned short pc;
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

	class APU : IMemory {
	private:
		Registers _registers;
	};
}

#endif //COMSQUARE_APU_HPP
