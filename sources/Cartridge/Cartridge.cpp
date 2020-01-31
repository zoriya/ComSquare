//
// Created by anonymus-raccoon on 1/27/20.
//

#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include "Cartridge.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidRom.hpp"
#include "../Exceptions/InvalidAction.hpp"

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
		throw InvalidAction("Witting to the ROM is not allowed.");
	}

	Header Cartridge::_mapHeader(uint32_t headerAddress)
	{
		Header head;
		headerAddress -= 0xC0u;

		ADDMAPPINGMODE(head.mappingMode, this->_data[headerAddress + 0xD5u] & 0x10u ? FastRom : SlowRom);
		ADDMAPPINGMODE(head.mappingMode, this->_data[headerAddress + 0xD5u] & 0x1u  ? HiRom : LoRom);
		if (this->_data[headerAddress + 0xD5u] & 0x2u || this->_data[headerAddress + 0xD5u] & 0x4u)
			ADDMAPPINGMODE(head.mappingMode, ExRom);
		head.romType = this->_data[headerAddress + 0xD6u];
		head.romSize = 0x400u << this->_data[headerAddress + 0xD7u];
		head.sramSize = 0x400u << this->_data[headerAddress + 0xD8u];
		head.creatorIDs[0] = this->_data[headerAddress + 0xD9u];
		head.creatorIDs[1] = this->_data[headerAddress + 0xDAu];
		head.version = this->_data[headerAddress + 0xDBu];
		head.checksumComplements[0] = this->_data[headerAddress + 0xDCu];
		head.checksumComplements[1] = this->_data[headerAddress + 0xDDu];
		head.checksums[0] = this->_data[headerAddress + 0xDEu];
		head.checksums[1] = this->_data[headerAddress + 0xDFu];

		head.nativeInterrupts.cop8[0] = this->_data[headerAddress + 0xE4u];
		head.nativeInterrupts.cop8[1] = this->_data[headerAddress + 0xE5u];
		head.nativeInterrupts.brk8[0] = this->_data[headerAddress + 0xE6u];
		head.nativeInterrupts.brk8[1] = this->_data[headerAddress + 0xE7u];
		head.nativeInterrupts.abort8[0] = this->_data[headerAddress + 0xE8u];
		head.nativeInterrupts.abort8[1] = this->_data[headerAddress + 0xE9u];
		head.nativeInterrupts.nmi8[0] = this->_data[headerAddress + 0xEAu];
		head.nativeInterrupts.nmi8[1] = this->_data[headerAddress + 0xEBu];
		head.nativeInterrupts.reset8[0] = this->_data[headerAddress + 0xECu];
		head.nativeInterrupts.reset8[1] = this->_data[headerAddress + 0xEDu];
		head.nativeInterrupts.irq8[0] = this->_data[headerAddress + 0xEEu];
		head.nativeInterrupts.irq8[1] = this->_data[headerAddress + 0xEFu];

		head.emulationInterrupts.cop8[0] = this->_data[headerAddress + 0xF4u];
		head.emulationInterrupts.cop8[1] = this->_data[headerAddress + 0xF5u];
		head.emulationInterrupts.abort8[0] = this->_data[headerAddress + 0xF8u];
		head.emulationInterrupts.abort8[1] = this->_data[headerAddress + 0xF9u];
		head.emulationInterrupts.nmi8[0] = this->_data[headerAddress + 0xFAu];
		head.emulationInterrupts.nmi8[1] = this->_data[headerAddress + 0xFBu];
		head.emulationInterrupts.reset8[0] = this->_data[headerAddress + 0xFCu];
		head.emulationInterrupts.reset8[1] = this->_data[headerAddress + 0xFDu];
		head.emulationInterrupts.brk8[0] = this->_data[headerAddress + 0xFEu];
		head.emulationInterrupts.brk8[1] = this->_data[headerAddress + 0xFFu];
		head.emulationInterrupts.irq8[0] = this->_data[headerAddress + 0xFEu];
		head.emulationInterrupts.irq8[1] = this->_data[headerAddress + 0xFFu];
		return head;
	}

	uint32_t Cartridge::_getHeaderAddress()
	{
		uint32_t address[] = {0x7FC0, 0xFFC0, 0x81C0, 0x101C0};
		int bestScore = -1;
		uint32_t bestAddress = 0;

		for (uint32_t addr : address) {
			int score = 0;

			if (addr + 0x32u >= this->_size)
				continue;

			Header info = this->_mapHeader(addr);
			if (info.romType <= 0x8u)
				score++;
			if (info.romSize < 0x400u << 0x10u)
				score++;
			if (info.sramSize < 0x400u << 0x08u)
				score++;
			if (info.checksum + info.checksumComplement == 0xFFFF && info.checksum != 0 && info.checksumComplement != 0)
				score += 8;

			if (info.emulationInterrupts.reset <= 0x8000u) // The reset vector is the first thing called by the SNES so It must execute the code inside the ROM (the rom starts at 0x8000).
				continue;


			if (score > bestScore) {
				bestScore = score;
				bestAddress = addr;
			}
		}
		return bestAddress;
	}

	bool Cartridge::_loadHeader()
	{
		uint32_t headerAddress = this->_getHeaderAddress();

		this->header = this->_mapHeader(headerAddress);
		char name[22];
		std::memcpy(name, &this->_data[headerAddress], 21);
		name[21] = '\0';
		this->header.gameName = std::string(name);
		return headerAddress & 0x200u;
	}
}