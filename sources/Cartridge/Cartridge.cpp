//
// Created by anonymus-raccoon on 1/27/20.
//

#include "Cartridge.hpp"
#include "Exceptions/InvalidAction.hpp"
#include "Exceptions/InvalidRom.hpp"
#include <cstring>
#include <sys/stat.h>
#include <fstream>

namespace ComSquare::Cartridge
{
	constexpr unsigned HeaderSize = 0x40u;

	Cartridge::Cartridge()
		: Ram::Ram(0, Rom, "Cartridge")
	{}

	Cartridge::Cartridge(const std::string &romPath)
		: Ram::Ram(0, Rom, "Cartridge"),
		  _romPath(romPath)
	{
		this->loadRom(romPath);
	}

	void Cartridge::loadRom(const std::string &path)
	{
		size_t size = Cartridge::getRomSize(path);
		std::ifstream rom(path, std::ios::binary);

		if (!rom)
			throw InvalidRomException("Could not open the rom file at " + path + ". " + strerror(errno));
		this->_data.resize(size);
		rom.read(reinterpret_cast<char *>(this->_data.data()), size);
		this->_loadHeader();
	}

	size_t Cartridge::getRomSize(const std::string &romPath)
	{
		struct stat info;

		if (stat(romPath.c_str(), &info) < 0)
			throw InvalidRomException("Could not stat the rom file at " + romPath + ". " + strerror(errno));
		return info.st_size;
	}

	uint8_t Cartridge::read(uint24_t addr)
	{
		return Ram::read(addr + this->_romStart);
	}

	void Cartridge::write(uint24_t, uint8_t)
	{
		throw InvalidAction("Witting to the ROM is not allowed.");
	}

	std::filesystem::path Cartridge::getRomPath() const
	{
		return this->_romPath;
	}

	Header Cartridge::_mapHeader(uint32_t headerAddress)
	{
		Header head;
		headerAddress -= 0xC0u;

		head.mappingMode |= this->_data[headerAddress + 0xD5u] & 0x10u ? FastRom : SlowRom;
		head.mappingMode |= this->_data[headerAddress + 0xD5u] & 0x1u ? HiRom : LoRom;
		if (this->_data[headerAddress + 0xD5u] & 0x2u || this->_data[headerAddress + 0xD5u] & 0x4u)
			head.mappingMode |= ExRom;
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
		const std::vector<uint32_t> address = {0x7FC0, 0xFFC0};
		unsigned int smc = this->getSize() % 1024;
		int bestScore = -1;
		uint32_t bestAddress = 0;

		for (uint32_t addr : address) {
			int score = 0;

			addr += smc;
			if (addr + 0x32u >= this->getSize())
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

			// The reset vector is the first thing called by the SNES so It must execute the code inside the ROM (the rom starts at 0x8000).
			if (info.emulationInterrupts.reset < 0x8000u)
				continue;
			uint8_t resetOpCode = this->_data[info.emulationInterrupts.reset - 0x8000u];
			switch (resetOpCode) {
			case 0x18://CLI
			case 0x78://SEI
			case 0x4C://JMP
			case 0x5C://JMP
			case 0x20://JSR
			case 0x22://JSL
			case 0x9C://STZ
				score += 8;
				break;
			case 0xC2://REP
			case 0xE2://SEP
			case 0xA9://LDA
			case 0xA2://LDX
			case 0xA0://LDY
				score += 4;
				break;
			case 0x00://BRK
			case 0xFF://SBC
			case 0xCC://CPY
				score -= 8;
				break;
			default:
				break;
			}

			if (score > bestScore) {
				bestScore = score;
				bestAddress = addr;
			}
		}
		return bestAddress;
	}

	bool Cartridge::_isSPCFile()
	{
		if (this->getSize() < 0x25)
			return false;
		std::string str = std::string(reinterpret_cast<char *>(this->getData().data()), 0x21);

		if (str != Cartridge::_magicSPC)
			return false;
		if (this->_data[0x21] != 0x1A || this->_data[0x22] != 0x1A)
			return false;
		if (this->_data[0x23] != 0x1A && this->_data[0x23] != 0x1B)
			return false;
		if (this->_data[0x24] != 0x1E)
			return false;
		return true;
	}

	bool Cartridge::_loadHeader()
	{
		if (this->_isSPCFile()) {
			this->_type = Audio;
			return false;
		}
		this->_type = Game;

		uint32_t headerAddress = this->_getHeaderAddress();
		if (headerAddress + HeaderSize > this->getSize())
			return false;

		this->header = this->_mapHeader(headerAddress);
		this->header.gameName = std::string(reinterpret_cast<char *>(&this->_data[headerAddress]), 21);
		if ((headerAddress + 0x40u) & 0x200u) {
			this->_romStart = 0x200u;
			return true;
		}
		return false;
	}

	CartridgeType Cartridge::getType()
	{
		return this->_type;
	}

	uint24_t Cartridge::getSize() const
	{
		return Ram::getSize() - this->_romStart;
	}

	MappingMode operator|(const MappingMode &self, const MappingMode &other)
	{
		return static_cast<MappingMode>(static_cast<int>(self) | static_cast<int>(other));
	}

	MappingMode &operator|=(MappingMode &self, const MappingMode &other)
	{
		int &selfInt = reinterpret_cast<int &>(self);
		selfInt |= static_cast<int>(other);
		return self;
	}
}// namespace ComSquare::Cartridge