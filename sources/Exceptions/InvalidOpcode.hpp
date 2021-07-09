//
// Created by anonymus-raccoon on 1/30/20.
//

#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <utility>
#include "DebuggableError.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidOpcode : public DebuggableError
	{
	public:
		explicit InvalidOpcode(std::string what)
			: DebuggableError(std::move(what))
		{}

		explicit InvalidOpcode(const std::string &pu, unsigned opcode)
			: DebuggableError("")
		{
			std::stringstream stream;
			stream << "The " + pu + " got an invalid opcode: 0x" << std::hex << opcode;
			this->_msg = stream.str();
		}
	};
}

