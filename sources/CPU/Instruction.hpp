//
// Created by anonymus-raccoon on 3/25/20.
//

#ifndef COMSQUARE_INSTRUCTION_HPP
#define COMSQUARE_INSTRUCTION_HPP

#include <string>
#include "../Models/Int24.hpp"

namespace ComSquare::CPU
{
	class CPU;

	//! @brief Different addressing modes that instructions can use for the main CPU.
	enum AddressingMode {
		Implied,

		Immediate8bits,
		ImmediateForA,
		ImmediateForX,

		Absolute,
		AbsoluteLong,

		DirectPage,
		DirectPageIndirect,
		DirectPageIndirectLong,

		AbsoluteIndexedByX,
		AbsoluteIndexedByXLong,
		AbsoluteIndexedByY,

		DirectPageIndexedByX,
		DirectPageIndexedByY,
		DirectPageIndirectIndexedByX,
		DirectPageIndirectIndexedByY,
		DirectPageIndirectIndexedByYLong,

		StackRelative,
		StackRelativeIndirectIndexedByY,


		ProgramCounterRelative,
		ProgramCounterRelativeLong,

		AbsoluteIndirect,
		AbsoluteIndirectIndexedByX,

		AbsoluteIndirectLong
	};

	//! @brief Struct containing basic information about instructions.
	struct Instruction {
		int (CPU::*call)(uint24_t valueAddr);
		int cycleCount;
		std::string name;
		AddressingMode addressingMode;
		int size;
	};
}
#endif //COMSQUARE_INSTRUCTION_HPP