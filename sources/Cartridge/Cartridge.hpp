//
// Created by anonymus-raccoon on 1/27/20.
//

#ifndef COMSQUARE_CARTRIDGE_HPP
#define COMSQUARE_CARTRIDGE_HPP

#include <string>
#include "../Memory/IMemory.hpp"

namespace ComSquare::Cartridge
{
	class InvalidRomException : std::exception {
	private:
		std::string _msg;
	public:
		explicit InvalidRomException(const std::string &msg) : _msg(msg) {}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};

	class Cartridge : IMemory {
	private:
		unsigned char *_data;
		size_t _size;
		static size_t getRomSize(const std::string &romPath);
	public:
		explicit Cartridge(const std::string &romPath);
		uint8_t read(uint32_t addr) override;
		void write(uint32_t addr, uint8_t data) override;
	};
}

#endif //COMSQUARE_CARTRIDGE_HPP
