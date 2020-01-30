//
// Created by anonymus-raccoon on 1/27/20.
//

#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include "Cartridge.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidRom.hpp"
#include "../Exceptions/InvalidAcction.hpp"

namespace ComSquare::Cartridge
{
	Cartridge::Cartridge(const std::string &romPath)
	{
		try {
			size_t size = Cartridge::getRomSize(romPath);
			FILE *rom = fopen(romPath.c_str(), "rb");

			if (!rom)
				throw InvalidRomException("Could not open the rom file at " + romPath + ". " + strerror(errno));
			this->_size = size;
			this->_data = new uint8_t[size];
			std::memset(this->_data, 0, size);
			fread(this->_data, 1, size, rom);
			this->_loadHeader();
		} catch (InvalidRomException &ex) {
			std::cerr << "Invalid Rom Error: " << ex.what() << std::endl;
		}
	}

	Cartridge::~Cartridge()
	{
		delete[] this->_data;
	}

	size_t Cartridge::getRomSize(const std::string &romPath)
	{
		struct stat info;

		if (stat(romPath.c_str(), &info) < 0)
			throw InvalidRomException("Could not stat the rom file at " + romPath + ". " + strerror(errno));
		return info.st_size;
	}


	uint8_t Cartridge::read_internal(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Cartridge read", addr);
		return this->_data[addr];
	}

	void Cartridge::write_internal(uint24_t addr, uint8_t data)
	{
		(void)addr;
		(void)data;
		throw InvalidAcction("Witting to the ROM is not allowed.");
	}

	uint32_t Cartridge::_getHeaderAddress()
	{
		uint32_t address[] = {0x7FC0, 0xFFC0, 0x81C0, 0x101C0};
		int bestScore = -1;
		uint32_t bestAddress = 0;

		for (uint32_t addr : address) {
			int score = 0;

			// TODO Implement a scoring system for the address here.
			if (score > bestScore) {
				bestScore = score;
				bestAddress = addr;
			}
		}
		return bestAddress;
	}

	bool Cartridge::_loadHeader()
	{
		unsigned headerAddress = this->_getHeaderAddress();

		char name[22];
		std::memcpy(name, &this->_data[headerAddress], 21);
		name[21] = '\0';
		this->header.gameName = std::string(name);
		ADDMAPPINGMODE(this->_data[headerAddress + 21] & 0x10u ? FastRom : SlowRom);
		ADDMAPPINGMODE(this->_data[headerAddress + 21] & 0x1u  ? HiRom : LoRom);
		if (this->_data[headerAddress + 21] & 0x2u || this->_data[headerAddress + 210] & 0x4u)
			ADDMAPPINGMODE(ExRom);
		this->header.romType = this->_data[headerAddress + 22];
		this->header.romSize = 0x400u << this->_data[headerAddress + 23];
		this->header.sramSize = 0x400u << this->_data[headerAddress + 24];
		this->header.creatorID = this->_data[headerAddress + 25];
		this->header.version = this->_data[headerAddress + 27];
		this->header.checksumComplement = this->_data[headerAddress + 28];
		this->header.checksum = this->_data[headerAddress + 29];
		return headerAddress & 0x200u;
	}
}