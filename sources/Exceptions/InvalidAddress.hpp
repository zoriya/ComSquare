//
// Created by anonymus-raccoon on 1/27/20.
//

#pragma once

#include "DebuggableError.hpp"
#include "Models/Ints.hpp"
#include <exception>
#include <ios>
#include <sstream>
#include <string>

namespace ComSquare
{
	//! @brief Exception thrown when trying to read/write to an invalid address.
	class InvalidAddress : public DebuggableError
	{
	private:
		std::string _msg;

	public:
		InvalidAddress(std::string where, uint24_t addr)
		{
			std::stringstream stream;
			stream << "Could not read/write data at address: " << addr << " from " << where;
			this->_msg = stream.str();
		}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};

	//! @brief Exception thrown when trying to read/write to an invalid address in a rectangle memory region.
	class InvalidRectangleAddress : public std::exception
	{
	private:
		std::string _msg;

	public:
		InvalidRectangleAddress(std::string where, int32_t addr, int16_t subaddr, int16_t start, int16_t end)
		{
			std::stringstream stream;
			stream << "Could not read/write data at address: 0x" << std::hex << addr << " from " << where;
			if (subaddr < start)
				stream << " (" << std::hex << subaddr << " < " << start << ")";
			else
				stream << " (" << std::hex << subaddr << " > " << end << ")";
			this->_msg = stream.str();
		}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}// namespace ComSquare
