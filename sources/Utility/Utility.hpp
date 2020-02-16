//
// Created by anonymus-raccoon on 2/16/20.
//

#ifndef COMSQUARE_UTILITY_HPP
#define COMSQUARE_UTILITY_HPP

#include <string>
#include <ios>
#include <sstream>
#include "../Models/Int24.hpp"

namespace ComSquare::Utility
{
	std::string to_hex(uint8_t i)
	{
		char buf[5];
		sprintf(buf, "0x%02X", i);
		return buf;
	}

	std::string to_hex(uint16_t i)
	{
		char buf[7];
		sprintf(buf, "0x%04X", i);
		return buf;
	}

	std::string to_hex(uint24_t i)
	{
		char buf[9];
		sprintf(buf, "0x%06X", i);
		return buf;
	}
}

#endif //COMSQUARE_UTILITY_HPP
