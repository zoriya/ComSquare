//
// Created by cbihan on 1/27/20.
//

#include "PPU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::PPU
{
	uint8_t PPU::read(uint24_t addr) {
		switch (addr) {
		case 0x34:
			return this->mpy.mpyl;
		case 0x35:
			return this->mpy.mpym;
		case 0x36:
			return this->mpy.mpyh;
		default:
			throw InvalidAddress("PPU Internal Registers read", addr);
		}
	}

	void PPU::write(uint24_t addr, uint8_t data) {
		switch (addr) {
		case 0x00:
			this->inidisp.raw = data;
			break;
		case 0x01:
			this->obsel.raw = data;
			break;
		case 0x02:
			this->oamadd.oamaddl = data;
			break;
		case 0x03:
			this->oamadd.oamaddh = data;
			break;
		case 0x04:
			this->oamdata = data;
			break;
		case 0x05:
			this->bgmode.raw = data;
			break;
		case 0x06:
			this->mosaic.raw = data;
			break;
		case 0x07:
			this->bg1sc.raw = data;
			break;
		case 0x08:
			this->bg2sc.raw = data;
			break;
		case 0x09:
			this->bg3sc.raw = data;
			break;
		case 0x0A:
			this->bg4sc.raw = data;
			break;
		case 0x0B:
			this->bg12nba.raw = data;
			break;
		case 0x0C:
			this->bg34nba.raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	void PPU::update(int cycles) {

	}
}