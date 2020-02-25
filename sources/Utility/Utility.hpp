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
	std::string to_hex(uint8_t i);

	std::string to_hex(uint16_t i);

	std::string to_hex(uint24_t i);

	std::string to_binary(uint8_t i);

	std::string to_binary(uint16_t i);

	std::string to_binary(uint24_t i);
}

#endif //COMSQUARE_UTILITY_HPP
