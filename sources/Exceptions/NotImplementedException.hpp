//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP
#define COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP

#include <exception>

namespace ComSquare
{
	class NotImplementedException : std::exception {
	public:
		explicit NotImplementedException() = default;
		const char *what() const noexcept override { return "Not implemented yet."; }
	};
}

#endif //COMSQUARE_NOTIMPLEMENTEDEXCEPTION_HPP
