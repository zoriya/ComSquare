//
// Created by anonymus-raccoon on 2/16/20.
//

#pragma once

#include <string>
#include <ios>
#include <sstream>
#include "Models/Ints.hpp"

namespace ComSquare::Utility
{
	enum HexString {
		NoPrefix,
		AsmPrefix,
		StandardPrefix
	};

	std::string to_hex(uint8_t i, HexString prefix = AsmPrefix);
	std::string to_hex(uint16_t i, HexString prefix = AsmPrefix);
	std::string to_hex(uint24_t i, HexString prefix = AsmPrefix);

	std::string to_binary(uint8_t i);
	std::string to_binary(uint16_t i);
	std::string to_binary(uint24_t i);

	constexpr std::string_view WHITESPACES = " \t\n\r\f\v";
}