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
			//! @brief Take the first bit to set the bool.
			this->_fBlank = data >> 7U;
			//! @brief Take the 4 last bits representing the brightness.
			this->_brightness = data & 0xFU;
			break;
		case 0x01: //! @brief $2101 OBSEL Register (Object Size and Character Address)
			//! @brief The first 3 bits.
			this->_objectSize = (data & 0xE0U) >> 5U;
			//! @brief The last 3 bits;
			this->_baseSelect = data & 0x07U;
			//! @brief th 2 center bits.
			this->_nameSelect = (data & 0x18U) >> 3U;
			break;
		case 0x02: //! @brief $2102 OAMADDL (OAM Address low byte).
			//! @brief we recreate the oamAddress with the previous oamAddress because there is one bit missing (given by the register $2103)
			this->_oamAddress = (this->_oamAddress & 0x100U) | data;
			break;
		case 0x03: //! @brief $2103 OAMADDH (OAM Address high bit and Obj Priority)
			//! @brief Same as $2102 we recreate the oamAddress with the previous oamAddress because there are 8 bits missing (given by the register $2102)
			this->_oamAddress = (this->_oamAddress & 0xFFU) | ((data & 0x01U) << 8U);
			//! @brief The objPriority is given by the first bit of the data
 			this->_objPriority = (data & 0x80U) >> 7U;
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

	void PPU::update(int cycles)
	{
		(void)cycles;
	}
}