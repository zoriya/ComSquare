//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_INVALIDADDRESS_HPP
#define COMSQUARE_INVALIDADDRESS_HPP

#include <exception>
#include <string>
#include <ios>
#include <sstream>

namespace ComSquare
{
	//! @brief Exception thrown when trying to read/write to an invalid address.
	class InvalidAddress : std::exception {
	private:
		std::string _msg;
	public:
		explicit InvalidAddress(int32_t addr)
		{
			std::stringstream stream;
			stream << "Could not read/write data at address: 0x" << std::hex << addr;
			this->_msg = stream.str();
		}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};
}

#endif //COMSQUARE_INVALIDADDRESS_HPP
