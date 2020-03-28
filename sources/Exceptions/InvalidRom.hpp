//
// Created by anonymus-raccoon on 1/28/20.
//

#ifndef COMSQUARE_INVALIDROM_HPP
#define COMSQUARE_INVALIDROM_HPP

#include <exception>
#include <string>
#include "DebuggableError.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidRomException : public DebuggableError {
	private:
		std::string _msg;
	public:
		explicit InvalidRomException(const std::string &msg) : _msg(msg) {}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}

#endif //COMSQUARE_INVALIDROM_HPP
