//
// Created by anonymus-raccoon on 1/30/20.
//

#ifndef COMSQUARE_INVALIDACTION_HPP
#define COMSQUARE_INVALIDACTION_HPP

#include <exception>
#include <string>
#include <sstream>
#include "DebuggableError.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidOpcode : public DebuggableError {
	private:
		std::string _msg;
	public:
		explicit InvalidOpcode(const std::string &what)
		{
			this->_msg = what;
		}

		explicit InvalidOpcode(const std::string &pu, unsigned opcode)
		{
			std::stringstream stream;
			stream << "The " + pu + " got an invalid opcode: 0x" << std::hex << opcode;
			this->_msg = stream.str();
		}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}
#endif //COMSQUARE_INVALIDACTION_HPP
