//
// Created by anonymus-raccoon on 1/27/20.
//

#pragma once

#include <exception>

namespace ComSquare
{
	//! @brief When this is thrown, it means that we should work more.
	class NotImplementedException : public std::exception {
	public:
		explicit NotImplementedException() = default;

		[[nodiscard]] const char *what() const noexcept override { return "Not implemented yet."; }
	};
}