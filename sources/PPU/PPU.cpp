//
// Created by cbihan on 1/27/20.
//

#include "PPU.hpp"
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::PPU
{
	uint8_t PPU::read(uint24_t addr)
	{
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

	void PPU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00: //! @brief $2100 INIDISP Register (F-blank and Brightness)
			this->_inidisp.raw = data;
			break;
		case 0x01: //! @brief $2101 OBSEL Register (Object Size and Character Address)
			this->_obsel.raw = data;
			break;
		case 0x02: //! @brief $2102 OAMADDL (OAM Address low byte).
			this->_oamadd.oamaddl = data;
			//! @brief we recreate the oamAddress with the previous oamAddress because there is one bit missing (given by the register $2103)
			break;
		case 0x03: //! @brief $2103 OAMADDH (OAM Address high bit and Obj Priority)
			this->_oamadd.oamaddh = data;
			//! @brief Same as $2102 we recreate the oamAddress with the previous oamAddress because there are 8 bits missing (given by the register $2102)
			break;
		case 0x04: //! @brief $2104 OAMDATA (Data for OAM write).
			//! @brief not implemented yet.
		//	throw InvalidAddress("PPU Internal Registers write", addr);
			this->_oamdata = data;
			break;
		case 0x05: //! @brief $2105 BGMODE (BG Mode and Character Size).
			this->_bgmode.raw = data;
			break;
		case 0x06:
			this->_mosaic.raw = data;
			break;
		case 0x07:
			this->_bg1sc.raw = data;
			break;
		case 0x08:
			this->_bg2sc.raw = data;
			break;
		case 0x09:
			this->_bg3sc.raw = data;
			break;
		case 0x0A:
			this->_bg4sc.raw = data;
			break;
		case 0x0B:
			this->_bg12nba.raw = data;
			break;
		case 0x0C:
			this->_bg34nba.raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	void PPU::update(int cycles)
	{
		(void)cycles;
	}
}