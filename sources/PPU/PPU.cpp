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
		case 0x06: //! $2106 MOSAIC (Screen Pixelation)
			this->_mosaic.raw = data;
			break;
		case 0x07: // BG1SC (BG1 Tilemap Address and Size)
		case 0x08: // BG2SC (BG2 Tilemap Address and Size)
		case 0x09: //! @brief BG3SC (BG3 Tilemap Address and Size)
		case 0x0A: //! @brief BG4SC (BG4 Tilemap Address and Size)
			this->_bgsc[addr - 0x07].raw = data;
			break;
		case 0x0B: //! @brief BG12NBA (BG1 and 2 Chr Address)
		case 0x0C: //! @brief BG34NBA (BG3 and 4 Chr Address)
			this->_bgnba[addr - 0x0B].raw = data;
			break;
		//TODO adding the rest of the registers. oaf !
		default:
			throw InvalidAddress("PPU Internal Registers write", addr);
		}
	}

	void PPU::update(unsigned cycles)
	{
		(void)cycles;
	}
}