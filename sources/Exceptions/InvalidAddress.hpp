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
#include "Utility/Utility.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when trying to read/write to an invalid address.
	class InvalidAddress : public DebuggableError
	{
	private:
		std::string _msg;
		uint24_t _addr;
		std::string _where;
	public:

		InvalidAddress(const std::string &where, uint24_t addr)
			: _addr(addr),
			  _where(where)
		{
			this->_msg = "Could not read/write data at address: " + Utility::to_hex(addr) + " from " + where;
		}

		[[nodiscard]] const char *what() const noexcept override { return this->_msg.c_str(); }

		[[nodiscard]] uint24_t getAddress() const
		{
			return this->_addr;
		}

		[[nodiscard]] std::string getWhere() const
		{
			return this->_where;
		}
	};

	//! @brief Exception thrown when trying to read/write to an invalid address in a rectangle memory region.
	class InvalidRectangleAddress : public std::exception
	{
	private:
		std::string _msg;

	public:
		InvalidRectangleAddress(const std::string &where, int32_t addr, int16_t subaddr, int16_t start, int16_t end)
		{
			std::stringstream stream;
			stream << "Could not read/write data at address: 0x" << std::hex << addr << " from " << where;
			if (subaddr < start)
				stream << " (" << std::hex << subaddr << " < " << start << ")";
			else
				stream << " (" << std::hex << subaddr << " > " << end << ")";
			this->_msg = stream.str();
		}
		[[nodiscard]] const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}// namespace ComSquare
