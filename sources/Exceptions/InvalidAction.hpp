//
// Created by anonymus-raccoon on 1/30/20.
//

#ifndef COMSQUARE_INVALIDACTION_HPP
#define COMSQUARE_INVALIDACTION_HPP

#include <exception>
#include <string>
#include "DebuggableError.hpp"

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidAction : public DebuggableError {
	private:
		std::string _msg;
	public:
		explicit InvalidAction(const std::string &msg) : _msg(msg) {}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}
#endif //COMSQUARE_INVALIDACTION_HPP
