//
// Created by anonymus-raccoon on 2/17/20.
//

#include "Utility.hpp"

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