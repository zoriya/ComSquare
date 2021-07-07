//
// Created by anonymus-raccoon on 2/17/20.
//

#include <bitset>
#include <iomanip>
#include "Utility.hpp"

namespace ComSquare::Utility
{
	std::string to_hex(uint8_t i, HexString prefix)
	{
		std::stringstream ss;
		if (prefix == AsmPrefix)
			ss << "$";
		else if (prefix == StandardPrefix)
			ss << "0x";
		ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(i);
		return ss.str();
	}

	std::string to_hex(uint16_t i, HexString prefix)
	{
		std::stringstream ss;
		if (prefix == AsmPrefix)
			ss << "$";
		else if (prefix == StandardPrefix)
			ss << "0x";
		ss << std::hex << std::setfill('0') << std::setw(4) << i;
		return ss.str();
	}

	std::string to_hex(uint24_t i, HexString prefix)
	{
		std::stringstream ss;
		if (prefix == AsmPrefix)
			ss << "$";
		else if (prefix == StandardPrefix)
			ss << "0x";
		ss << std::hex << std::setfill('0') << std::setw(6) << i;
		return ss.str();
	}

	std::string to_binary(uint8_t i)
	{
		return std::bitset<8>(i).to_string();
	}

	std::string to_binary(uint16_t i)
	{
		return std::bitset<16>(i).to_string();
	}

	std::string to_binary(uint24_t i)
	{
		return std::bitset<24>(i).to_string();
	}
}

std::ostream &operator<<(std::ostream &os, uint8_t value)
{
	return os << ComSquare::Utility::to_hex(value);
}