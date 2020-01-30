//
// Created by anonymus-raccoon on 1/30/20.
//

#ifndef COMSQUARE_INVALIDACCTION_HPP
#define COMSQUARE_INVALIDACCTION_HPP

#include <exception>
#include <string>

namespace ComSquare
{
	//! @brief Exception thrown when someone tries to load an invalid rom.
	class InvalidAcction : std::exception {
	private:
		std::string _msg;
	public:
		explicit InvalidAcction(const std::string &msg) : _msg(msg) {}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}
#endif //COMSQUARE_INVALIDACCTION_HPP
