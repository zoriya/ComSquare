//
// Created by anonymus-raccoon on 3/26/20.
//

#pragma once

#include <bits/exception.h>

namespace ComSquare
{
	class DebuggableError : public std::exception
	{
	protected:
		std::string _msg;
	public:
		explicit DebuggableError(std::string msg)
			: _msg(std::move(msg))
		{}

		[[nodiscard]] const char *what() const noexcept override
		{
			return this->_msg.c_str();
		}
	};
}
