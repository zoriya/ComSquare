//
// Created by Melefo on 28/01/2020.
//

#include "DSP.hpp"
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::DSP
{
	DSP::DSP()
	{ }

	uint8_t DSP::read(uint24_t addr)
	{
		switch (addr) {
		case 0x00:
			return this->_channels[0].volL;
		case 0x10:
			return this->_channels[1].volL;
		case 0x20:
			return this->_channels[2].volL;
		case 0x30:
			return this->_channels[3].volL;
		case 0x40:
			return this->_channels[4].volL;
		case 0x50:
			return this->_channels[5].volL;
		case 0x60:
			return this->_channels[6].volL;
		case 0x70:
			return this->_channels[7].volL;
		case 0x01:
			return this->_channels[0].volR;
		case 0x11:
			return this->_channels[1].volR;
		case 0x21:
			return this->_channels[2].volR;
		case 0x31:
			return this->_channels[3].volR;
		case 0x41:
			return this->_channels[4].volR;
		case 0x51:
			return this->_channels[5].volR;
		case 0x61:
			return this->_channels[6].volR;
		case 0x71:
			return this->_channels[7].volR;
		case 0x02:
			return this->_channels[0].pitchL;
		case 0x12:
			return this->_channels[1].pitchL;
		case 0x22:
			return this->_channels[2].pitchL;
		case 0x32:
			return this->_channels[3].pitchL;
		case 0x42:
			return this->_channels[4].pitchL;
		case 0x52:
			return this->_channels[5].pitchL;
		case 0x62:
			return this->_channels[6].pitchL;
		case 0x72:
			return this->_channels[7].pitchL;
		case 0x03:
			return this->_channels[0].pitchH;
		case 0x13:
			return this->_channels[1].pitchH;
		case 0x23:
			return this->_channels[2].pitchH;
		case 0x33:
			return this->_channels[3].pitchH;
		case 0x43:
			return this->_channels[4].pitchH;
		case 0x53:
			return this->_channels[5].pitchH;
		case 0x63:
			return this->_channels[6].pitchH;
		case 0x73:
			return this->_channels[7].pitchH;
		case 0x04:
			return this->_channels[0].srcn;
		case 0x14:
			return this->_channels[1].srcn;
		case 0x24:
			return this->_channels[2].srcn;
		case 0x34:
			return this->_channels[3].srcn;
		case 0x44:
			return this->_channels[4].srcn;
		case 0x54:
			return this->_channels[5].srcn;
		case 0x64:
			return this->_channels[6].srcn;
		case 0x74:
			return this->_channels[7].srcn;
		case 0x05:
			return this->_channels[0].adsr1;
		case 0x15:
			return this->_channels[1].adsr1;
		case 0x25:
			return this->_channels[2].adsr1;
		case 0x35:
			return this->_channels[3].adsr1;
		case 0x45:
			return this->_channels[4].adsr1;
		case 0x55:
			return this->_channels[5].adsr1;
		case 0x65:
			return this->_channels[6].adsr1;
		case 0x75:
			return this->_channels[7].adsr1;
		case 0x06:
			return this->_channels[0].adsr2;
		case 0x16:
			return this->_channels[1].adsr2;
		case 0x26:
			return this->_channels[2].adsr2;
		case 0x36:
			return this->_channels[3].adsr2;
		case 0x46:
			return this->_channels[4].adsr2;
		case 0x56:
			return this->_channels[5].adsr2;
		case 0x66:
			return this->_channels[6].adsr2;
		case 0x76:
			return this->_channels[7].adsr2;
		case 0x07:
			return this->_channels[0].gain;
		case 0x17:
			return this->_channels[1].gain;
		case 0x27:
			return this->_channels[2].gain;
		case 0x37:
			return this->_channels[3].gain;
		case 0x47:
			return this->_channels[4].gain;
		case 0x57:
			return this->_channels[5].gain;
		case 0x67:
			return this->_channels[6].gain;
		case 0x77:
			return this->_channels[7].gain;
		case 0x08:
			return this->_channels[0].envx;
		case 0x18:
			return this->_channels[1].envx;
		case 0x28:
			return this->_channels[2].envx;
		case 0x38:
			return this->_channels[3].envx;
		case 0x48:
			return this->_channels[4].envx;
		case 0x58:
			return this->_channels[5].envx;
		case 0x68:
			return this->_channels[6].envx;
		case 0x78:
			return this->_channels[7].envx;
		case 0x09:
			return this->_channels[0].outx;
		case 0x19:
			return this->_channels[1].outx;
		case 0x29:
			return this->_channels[2].outx;
		case 0x39:
			return this->_channels[3].outx;
		case 0x49:
			return this->_channels[4].outx;
		case 0x59:
			return this->_channels[5].outx;
		case 0x69:
			return this->_channels[6].outx;
		case 0x79:
			return this->_channels[7].outx;
		case 0x0C:
			return this->_registers.mvolL;
		case 0x1C:
			return this->_registers.mvolR;
		case 0x2C:
			return this->_registers.evolL;
		case 0x3C:
			return this->_registers.evolR;
		case 0x4C: {
			uint8_t kon = 0;

			for (int i = 0; i < 8; i++)
				kon |= this->_channels[i].kon << i;
			return kon;
		}
		case 0x5C: {
			uint8_t kof = 0;

			for (int i = 0; i < 8; i++)
				kof |= this->_channels[i].kof << i;
			return kof;
		}
		case 0x6C:
			return this->_registers.flg;
		case 0x7C: {
			uint8_t endx = 0;

			for (int i = 0; i < 8; i++)
				endx |= this->_channels[i].endx << i;
			return endx;
		}
		case 0x0D:
			return this->_registers.efb;
		case 0x1D:
			return this->_registers.unused;
		case 0x2D: {
			uint8_t pmon = 0;

			for (int i = 0; i < 8; i++)
				pmon |= this->_channels[i].pmon << i;
			return pmon;
		}
		case 0x3D: {
			uint8_t non = 0;

			for (int i = 0; i < 8; i++)
				non |= this->_channels[i].non << i;
			return non;
		}
		case 0x4D: {
			uint8_t eon = 0;

			for (int i = 0; i < 8; i++)
				eon |= this->_channels[i].eon << i;
			return eon;
		}
		case 0x5D:
			return this->_registers.dir;
		case 0x6D:
			return this->_registers.esa;
		case 0x7D:
			return this->_registers.edl;
		case 0x0F:
			return this->_channels[0].coeff;
		case 0x1F:
			return this->_channels[1].coeff;
		case 0x2F:
			return this->_channels[2].coeff;
		case 0x3F:
			return this->_channels[3].coeff;
		case 0x4F:
			return this->_channels[4].coeff;
		case 0x5F:
			return this->_channels[5].coeff;
		case 0x6F:
			return this->_channels[6].coeff;
		case 0x7F:
			return this->_channels[7].coeff;
		default:
			throw InvalidAddress("DSP Registers read", addr);
		}
	}

	void DSP::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00:
			this->_channels[0].volL = data;
			break;
		case 0x10:
			this->_channels[1].volL = data;
			break;
		case 0x20:
			this->_channels[2].volL = data;
			break;
		case 0x30:
			this->_channels[3].volL = data;
			break;
		case 0x40:
			this->_channels[4].volL = data;
			break;
		case 0x50:
			this->_channels[5].volL = data;
			break;
		case 0x60:
			this->_channels[6].volL = data;
			break;
		case 0x70:
			this->_channels[7].volL = data;
			break;
		case 0x01:
			this->_channels[0].volR = data;
			break;
		case 0x11:
			this->_channels[1].volR = data;
			break;
		case 0x21:
			this->_channels[2].volR = data;
			break;
		case 0x31:
			this->_channels[3].volR = data;
			break;
		case 0x41:
			this->_channels[4].volR = data;
			break;
		case 0x51:
			this->_channels[5].volR = data;
			break;
		case 0x61:
			this->_channels[6].volR = data;
			break;
		case 0x71:
			this->_channels[7].volR = data;
			break;
		case 0x02:
			this->_channels[0].pitchL = data;
			break;
		case 0x12:
			this->_channels[1].pitchL = data;
			break;
		case 0x22:
			this->_channels[2].pitchL = data;
			break;
		case 0x32:
			this->_channels[3].pitchL = data;
			break;
		case 0x42:
			this->_channels[4].pitchL = data;
			break;
		case 0x52:
			this->_channels[5].pitchL = data;
			break;
		case 0x62:
			this->_channels[6].pitchL = data;
			break;
		case 0x72:
			this->_channels[7].pitchL = data;
			break;
		case 0x03:
			this->_channels[0].pitchH = data;
			break;
		case 0x13:
			this->_channels[1].pitchH = data;
			break;
		case 0x23:
			this->_channels[2].pitchH = data;
			break;
		case 0x33:
			this->_channels[3].pitchH = data;
			break;
		case 0x43:
			this->_channels[4].pitchH = data;
			break;
		case 0x53:
			this->_channels[5].pitchH = data;
			break;
		case 0x63:
			this->_channels[6].pitchH = data;
			break;
		case 0x73:
			this->_channels[7].pitchH = data;
			break;
		case 0x04:
			this->_channels[0].srcn = data;
			break;
		case 0x14:
			this->_channels[1].srcn = data;
			break;
		case 0x24:
			this->_channels[2].srcn = data;
			break;
		case 0x34:
			this->_channels[3].srcn = data;
			break;
		case 0x44:
			this->_channels[4].srcn = data;
			break;
		case 0x54:
			this->_channels[5].srcn = data;
			break;
		case 0x64:
			this->_channels[6].srcn = data;
			break;
		case 0x74:
			this->_channels[7].srcn = data;
			break;
		case 0x05:
			this->_channels[0].adsr1 = data;
			break;
		case 0x15:
			this->_channels[1].adsr1 = data;
			break;
		case 0x25:
			this->_channels[2].adsr1 = data;
			break;
		case 0x35:
			this->_channels[3].adsr1 = data;
			break;
		case 0x45:
			this->_channels[4].adsr1 = data;
			break;
		case 0x55:
			this->_channels[5].adsr1 = data;
			break;
		case 0x65:
			this->_channels[6].adsr1 = data;
			break;
		case 0x75:
			this->_channels[7].adsr1 = data;
			break;
		case 0x06:
			this->_channels[0].adsr2 = data;
			break;
		case 0x16:
			this->_channels[1].adsr2 = data;
			break;
		case 0x26:
			this->_channels[2].adsr2 = data;
			break;
		case 0x36:
			this->_channels[3].adsr2 = data;
			break;
		case 0x46:
			this->_channels[4].adsr2 = data;
			break;
		case 0x56:
			this->_channels[5].adsr2 = data;
			break;
		case 0x66:
			this->_channels[6].adsr2 = data;
			break;
		case 0x76:
			this->_channels[7].adsr2 = data;
			break;
		case 0x07:
			this->_channels[0].gain = data;
			break;
		case 0x17:
			this->_channels[1].gain = data;
			break;
		case 0x27:
			this->_channels[2].gain = data;
			break;
		case 0x37:
			this->_channels[3].gain = data;
			break;
		case 0x47:
			this->_channels[4].gain = data;
			break;
		case 0x57:
			this->_channels[5].gain = data;
			break;
		case 0x67:
			this->_channels[6].gain = data;
			break;
		case 0x77:
			this->_channels[7].gain = data;
			break;
		case 0x08:
			this->_channels[0].envx = data;
			break;
		case 0x18:
			this->_channels[1].envx = data;
			break;
		case 0x28:
			this->_channels[2].envx = data;
			break;
		case 0x38:
			this->_channels[3].envx = data;
			break;
		case 0x48:
			this->_channels[4].envx = data;
			break;
		case 0x58:
			this->_channels[5].envx = data;
			break;
		case 0x68:
			this->_channels[6].envx = data;
			break;
		case 0x78:
			this->_channels[7].envx = data;
			break;
		case 0x09:
			this->_channels[0].outx = data;
			break;
		case 0x19:
			this->_channels[1].outx = data;
			break;
		case 0x29:
			this->_channels[2].outx = data;
			break;
		case 0x39:
			this->_channels[3].outx = data;
			break;
		case 0x49:
			this->_channels[4].outx = data;
			break;
		case 0x59:
			this->_channels[5].outx = data;
			break;
		case 0x69:
			this->_channels[6].outx = data;
			break;
		case 0x79:
			this->_channels[7].outx = data;
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
			for (int i = 0; i < 8; i++)
				this->_channels[i].kon |= data << i;
			break;
		case 0x5C:
			for (int i = 0; i < 8; i++)
				this->_channels[i].kof |= data << i;
			break;
		case 0x6C:
			this->_registers.flg = data;
			break;
		case 0x7C:
			for (int i = 0; i < 8; i++)
				this->_channels[i].endx |= data << i;
			break;
		case 0x0D:
			this->_registers.efb = data;
			break;
		case 0x1D:
			this->_registers.unused = data;
			break;
		case 0x2D:
			for (int i = 0; i < 8; i++)
				this->_channels[i].pmon |= data << i;
			break;
		case 0x3D:
			for (int i = 0; i < 8; i++)
				this->_channels[i].non |= data << i;
			break;
		case 0x4D:
			for (int i = 0; i < 8; i++)
				this->_channels[i].eon |= data << i;
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
			this->_channels[0].coeff = data;
			break;
		case 0x1F:
			this->_channels[1].coeff = data;
			break;
		case 0x2F:
			this->_channels[2].coeff = data;
			break;
		case 0x3F:
			this->_channels[3].coeff = data;
			break;
		case 0x4F:
			this->_channels[4].coeff = data;
			break;
		case 0x5F:
			this->_channels[5].coeff = data;
			break;
		case 0x6F:
			this->_channels[6].coeff = data;
			break;
		case 0x7F:
			this->_channels[7].coeff = data;
			break;
		default:
			throw InvalidAddress("DSP Registers write", addr);
		}
	}

	Registers DSP::getRegisters()
	{
		return this->_registers;
	}

	std::array<Channel, 8> DSP::getChannels()
	{
		return this->_channels;
	}
}