//
// Created by Melefo on 28/01/2020.
//

#include "DSP.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::DSP
{
	DSP::DSP()
	{ }

	uint8_t DSP::read(uint24_t addr)
	{
		switch (addr) {
		case 0x00:
			return this->_registers.volL[0];
		case 0x10:
			return this->_registers.volL[1];
		case 0x20:
			return this->_registers.volL[2];
		case 0x30:
			return this->_registers.volL[3];
		case 0x40:
			return this->_registers.volL[4];
		case 0x50:
			return this->_registers.volL[5];
		case 0x60:
			return this->_registers.volL[6];
		case 0x70:
			return this->_registers.volL[7];
		case 0x01:
			return this->_registers.volR[0];
		case 0x11:
			return this->_registers.volR[1];
		case 0x21:
			return this->_registers.volR[2];
		case 0x31:
			return this->_registers.volR[3];
		case 0x41:
			return this->_registers.volR[4];
		case 0x51:
			return this->_registers.volR[5];
		case 0x61:
			return this->_registers.volR[6];
		case 0x71:
			return this->_registers.volR[7];
		case 0x02:
			return this->_registers.pitchL[0];
		case 0x12:
			return this->_registers.pitchL[1];
		case 0x22:
			return this->_registers.pitchL[2];
		case 0x32:
			return this->_registers.pitchL[3];
		case 0x42:
			return this->_registers.pitchL[4];
		case 0x52:
			return this->_registers.pitchL[5];
		case 0x62:
			return this->_registers.pitchL[6];
		case 0x72:
			return this->_registers.pitchL[7];
		case 0x03:
			return this->_registers.pitchH[0];
		case 0x13:
			return this->_registers.pitchH[1];
		case 0x23:
			return this->_registers.pitchH[2];
		case 0x33:
			return this->_registers.pitchH[3];
		case 0x43:
			return this->_registers.pitchH[4];
		case 0x53:
			return this->_registers.pitchH[5];
		case 0x63:
			return this->_registers.pitchH[6];
		case 0x73:
			return this->_registers.pitchH[7];
		case 0x04:
			return this->_registers.srcn[0];
		case 0x14:
			return this->_registers.srcn[1];
		case 0x24:
			return this->_registers.srcn[2];
		case 0x34:
			return this->_registers.srcn[3];
		case 0x44:
			return this->_registers.srcn[4];
		case 0x54:
			return this->_registers.srcn[5];
		case 0x64:
			return this->_registers.srcn[6];
		case 0x74:
			return this->_registers.srcn[7];
		case 0x05:
			return this->_registers.adsr1[0];
		case 0x15:
			return this->_registers.adsr1[1];
		case 0x25:
			return this->_registers.adsr1[2];
		case 0x35:
			return this->_registers.adsr1[3];
		case 0x45:
			return this->_registers.adsr1[4];
		case 0x55:
			return this->_registers.adsr1[5];
		case 0x65:
			return this->_registers.adsr1[6];
		case 0x75:
			return this->_registers.adsr1[7];
		case 0x06:
			return this->_registers.adsr2[0];
		case 0x16:
			return this->_registers.adsr2[1];
		case 0x26:
			return this->_registers.adsr2[2];
		case 0x36:
			return this->_registers.adsr2[3];
		case 0x46:
			return this->_registers.adsr2[4];
		case 0x56:
			return this->_registers.adsr2[5];
		case 0x66:
			return this->_registers.adsr2[6];
		case 0x76:
			return this->_registers.adsr2[7];
		case 0x07:
			return this->_registers.gain[0];
		case 0x17:
			return this->_registers.gain[1];
		case 0x27:
			return this->_registers.gain[2];
		case 0x37:
			return this->_registers.gain[3];
		case 0x47:
			return this->_registers.gain[4];
		case 0x57:
			return this->_registers.gain[5];
		case 0x67:
			return this->_registers.gain[6];
		case 0x77:
			return this->_registers.gain[7];
		case 0x08:
			return this->_registers.envx[0];
		case 0x18:
			return this->_registers.envx[1];
		case 0x28:
			return this->_registers.envx[2];
		case 0x38:
			return this->_registers.envx[3];
		case 0x48:
			return this->_registers.envx[4];
		case 0x58:
			return this->_registers.envx[5];
		case 0x68:
			return this->_registers.envx[6];
		case 0x78:
			return this->_registers.envx[7];
		case 0x09:
			return this->_registers.outx[0];
		case 0x19:
			return this->_registers.outx[1];
		case 0x29:
			return this->_registers.outx[2];
		case 0x39:
			return this->_registers.outx[3];
		case 0x49:
			return this->_registers.outx[4];
		case 0x59:
			return this->_registers.outx[5];
		case 0x69:
			return this->_registers.outx[6];
		case 0x79:
			return this->_registers.outx[7];
		case 0x0C:
			return this->_registers.mvolL;
		case 0x1C:
			return this->_registers.mvolR;
		case 0x2C:
			return this->_registers.evolL;
		case 0x3C:
			return this->_registers.evolR;
		case 0x4C:
			return this->_registers.kon;
		case 0x5C:
			return this->_registers.kof;
		case 0x6C:
			return this->_registers.flg;
		case 0x7C:
			return this->_registers.endx;
		case 0x0D:
			return this->_registers.efb;
		case 0x1D:
			return this->_registers.unused;
		case 0x2D:
			return this->_registers.pmon;
		case 0x3D:
			return this->_registers.non;
		case 0x4D:
			return this->_registers.eon;
		case 0x5D:
			return this->_registers.dir;
		case 0x6D:
			return this->_registers.esa;
		case 0x7D:
			return this->_registers.edl;
		case 0x0F:
			return this->_registers.coeff[0];
		case 0x1F:
			return this->_registers.coeff[1];
		case 0x2F:
			return this->_registers.coeff[2];
		case 0x3F:
			return this->_registers.coeff[3];
		case 0x4F:
			return this->_registers.coeff[4];
		case 0x5F:
			return this->_registers.coeff[5];
		case 0x6F:
			return this->_registers.coeff[6];
		case 0x7F:
			return this->_registers.coeff[7];
		default:
			throw InvalidAddress("DSP Internal Registers read", addr);
		}
	}

	void DSP::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00:
			this->_registers.volL[0] = data;
			break;
		case 0x10:
			this->_registers.volL[1] = data;
			break;
		case 0x20:
			this->_registers.volL[2] = data;
			break;
		case 0x30:
			this->_registers.volL[3] = data;
			break;
		case 0x40:
			this->_registers.volL[4] = data;
			break;
		case 0x50:
			this->_registers.volL[5] = data;
			break;
		case 0x60:
			this->_registers.volL[6] = data;
			break;
		case 0x70:
			this->_registers.volL[7] = data;
			break;
		case 0x01:
			this->_registers.volR[0] = data;
			break;
		case 0x11:
			this->_registers.volR[1] = data;
			break;
		case 0x21:
			this->_registers.volR[2] = data;
			break;
		case 0x31:
			this->_registers.volR[3] = data;
			break;
		case 0x41:
			this->_registers.volR[4] = data;
			break;
		case 0x51:
			this->_registers.volR[5] = data;
			break;
		case 0x61:
			this->_registers.volR[6] = data;
			break;
		case 0x71:
			this->_registers.volR[7] = data;
			break;
		case 0x02:
			this->_registers.pitchL[0] = data;
			break;
		case 0x12:
			this->_registers.pitchL[1] = data;
			break;
		case 0x22:
			this->_registers.pitchL[2] = data;
			break;
		case 0x32:
			this->_registers.pitchL[3] = data;
			break;
		case 0x42:
			this->_registers.pitchL[4] = data;
			break;
		case 0x52:
			this->_registers.pitchL[5] = data;
			break;
		case 0x62:
			this->_registers.pitchL[6] = data;
			break;
		case 0x72:
			this->_registers.pitchL[7] = data;
			break;
		case 0x03:
			this->_registers.pitchH[0] = data;
			break;
		case 0x13:
			this->_registers.pitchH[1] = data;
			break;
		case 0x23:
			this->_registers.pitchH[2] = data;
			break;
		case 0x33:
			this->_registers.pitchH[3] = data;
			break;
		case 0x43:
			this->_registers.pitchH[4] = data;
			break;
		case 0x53:
			this->_registers.pitchH[5] = data;
			break;
		case 0x63:
			this->_registers.pitchH[6] = data;
			break;
		case 0x73:
			this->_registers.pitchH[7] = data;
			break;
		case 0x04:
			this->_registers.srcn[0] = data;
			break;
		case 0x14:
			this->_registers.srcn[1] = data;
			break;
		case 0x24:
			this->_registers.srcn[2] = data;
			break;
		case 0x34:
			this->_registers.srcn[3] = data;
			break;
		case 0x44:
			this->_registers.srcn[4] = data;
			break;
		case 0x54:
			this->_registers.srcn[5] = data;
			break;
		case 0x64:
			this->_registers.srcn[6] = data;
			break;
		case 0x74:
			this->_registers.srcn[7] = data;
			break;
		case 0x05:
			this->_registers.adsr1[0] = data;
			break;
		case 0x15:
			this->_registers.adsr1[1] = data;
			break;
		case 0x25:
			this->_registers.adsr1[2] = data;
			break;
		case 0x35:
			this->_registers.adsr1[3] = data;
			break;
		case 0x45:
			this->_registers.adsr1[4] = data;
			break;
		case 0x55:
			this->_registers.adsr1[5] = data;
			break;
		case 0x65:
			this->_registers.adsr1[6] = data;
			break;
		case 0x75:
			this->_registers.adsr1[7] = data;
			break;
		case 0x06:
			this->_registers.adsr2[0] = data;
			break;
		case 0x16:
			this->_registers.adsr2[1] = data;
			break;
		case 0x26:
			this->_registers.adsr2[2] = data;
			break;
		case 0x36:
			this->_registers.adsr2[3] = data;
			break;
		case 0x46:
			this->_registers.adsr2[4] = data;
			break;
		case 0x56:
			this->_registers.adsr2[5] = data;
			break;
		case 0x66:
			this->_registers.adsr2[6] = data;
			break;
		case 0x76:
			this->_registers.adsr2[7] = data;
			break;
		case 0x07:
			this->_registers.gain[0] = data;
			break;
		case 0x17:
			this->_registers.gain[1] = data;
			break;
		case 0x27:
			this->_registers.gain[2] = data;
			break;
		case 0x37:
			this->_registers.gain[3] = data;
			break;
		case 0x47:
			this->_registers.gain[4] = data;
			break;
		case 0x57:
			this->_registers.gain[5] = data;
			break;
		case 0x67:
			this->_registers.gain[6] = data;
			break;
		case 0x77:
			this->_registers.gain[7] = data;
			break;
		case 0x08:
			this->_registers.envx[0] = data;
			break;
		case 0x18:
			this->_registers.envx[1] = data;
			break;
		case 0x28:
			this->_registers.envx[2] = data;
			break;
		case 0x38:
			this->_registers.envx[3] = data;
			break;
		case 0x48:
			this->_registers.envx[4] = data;
			break;
		case 0x58:
			this->_registers.envx[5] = data;
			break;
		case 0x68:
			this->_registers.envx[6] = data;
			break;
		case 0x78:
			this->_registers.envx[7] = data;
			break;
		case 0x09:
			this->_registers.outx[0] = data;
			break;
		case 0x19:
			this->_registers.outx[1] = data;
			break;
		case 0x29:
			this->_registers.outx[2] = data;
			break;
		case 0x39:
			this->_registers.outx[3] = data;
			break;
		case 0x49:
			this->_registers.outx[4] = data;
			break;
		case 0x59:
			this->_registers.outx[5] = data;
			break;
		case 0x69:
			this->_registers.outx[6] = data;
			break;
		case 0x79:
			this->_registers.outx[7] = data;
			break;
		case 0x0C:
			this->_registers.mvolL = data;
			break;
		case 0x1C:
			this->_registers.mvolR = data;
			break;
		case 0x2C:
			this->_registers.evolL = data;
			break;
		case 0x3C:
			this->_registers.evolR = data;
			break;
		case 0x4C:
			this->_registers.kon = data;
			break;
		case 0x5C:
			this->_registers.kof = data;
			break;
		case 0x6C:
			this->_registers.flg = data;
			break;
		case 0x7C:
			this->_registers.endx = data;
			break;
		case 0x0D:
			this->_registers.efb = data;
			break;
		case 0x1D:
			this->_registers.unused = data;
			break;
		case 0x2D:
			this->_registers.pmon = data;
			break;
		case 0x3D:
			this->_registers.non = data;
			break;
		case 0x4D:
			this->_registers.eon = data;
			break;
		case 0x5D:
			this->_registers.dir = data;
			break;
		case 0x6D:
			this->_registers.esa = data;
			break;
		case 0x7D:
			this->_registers.edl = data;
			break;
		case 0x0F:
			this->_registers.coeff[0] = data;
			break;
		case 0x1F:
			this->_registers.coeff[1] = data;
			break;
		case 0x2F:
			this->_registers.coeff[2] = data;
			break;
		case 0x3F:
			this->_registers.coeff[3] = data;
			break;
		case 0x4F:
			this->_registers.coeff[4] = data;
			break;
		case 0x5F:
			this->_registers.coeff[5] = data;
			break;
		case 0x6F:
			this->_registers.coeff[6] = data;
			break;
		case 0x7F:
			this->_registers.coeff[7] = data;
			break;
		default:
			throw InvalidAddress("DSP Internal Registers write", addr);
		}
	}
}