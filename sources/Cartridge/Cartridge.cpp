//
// Created by anonymus-raccoon on 1/27/20.
//

#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include "Cartridge.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidRom.hpp"

namespace ComSquare::Cartridge
{
	size_t Cartridge::getRomSize(const std::string &romPath)
	{
		struct stat info;

		if (stat(romPath.c_str(), &info) < 0)
			throw InvalidRomException("Could not stat the rom file at " + romPath + ". " + strerror(errno));
		return info.st_size;
	}

	Cartridge::Cartridge(const std::string &romPath)
	{
		try {
			size_t size = this->getRomSize(romPath);
			FILE *rom = fopen(romPath.c_str(), "rb");

			if (!rom)
				throw InvalidRomException("Could not open the rom file at " + romPath + ". " + strerror(errno));
			this->_size = size;
			this->_data = new unsigned char[size];
			std::memset(this->_data, 0, size);
			fread(this->_data, 1, size, rom);
		} catch (InvalidRomException &ex) {
			std::cerr << "Invalid Rom Error: " << ex.what() << std::endl;
		}
	}

	uint8_t Cartridge::read(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Cartridge read", addr);
		return this->_data[addr];
	}

	void Cartridge::write(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Cartridge write", addr);
		this->_data[addr] = data;
	}
}