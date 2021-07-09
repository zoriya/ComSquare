//
// Created by anonymus-raccoon on 1/28/20.
//

#pragma once

#include <exception>
#include <string>
#include <utility>
#include "DebuggableError.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidRomException : public DebuggableError
	{
	public:
		explicit InvalidRomException(std::string msg)
			: DebuggableError(std::move(msg))
		{}
	};
}
