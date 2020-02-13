//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP
#define COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP

#include <exception>

namespace ComSquare
{
	//! @brief When this is thrown, it means that we should work more.
	class NotImplementedException : public std::exception {
	public:
		explicit NotImplementedException() = default;
		const char *what() const noexcept override { return "Not implemented yet."; }
	};
}

#endif //COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP
