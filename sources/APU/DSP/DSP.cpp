//
// Created by Melefo on 28/01/2020.
//

#include "DSP.hpp"
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::DSP
{
    DSP::DSP(int16_t *buffer, int32_t size)
    {
        this->_state.buffer = buffer;
        this->_state.bufferStart = buffer;
        this->_state.bufferEnd = buffer + size;
    }

	uint8_t DSP::read(uint24_t addr)
	{
		switch (addr) {
		case 0x00:
			return this->_voices[0].volL;
		case 0x10:
			return this->_voices[1].volL;
		case 0x20:
			return this->_voices[2].volL;
		case 0x30:
			return this->_voices[3].volL;
		case 0x40:
			return this->_voices[4].volL;
		case 0x50:
			return this->_voices[5].volL;
		case 0x60:
			return this->_voices[6].volL;
		case 0x70:
			return this->_voices[7].volL;
		case 0x01:
			return this->_voices[0].volR;
		case 0x11:
			return this->_voices[1].volR;
		case 0x21:
			return this->_voices[2].volR;
		case 0x31:
			return this->_voices[3].volR;
		case 0x41:
			return this->_voices[4].volR;
		case 0x51:
			return this->_voices[5].volR;
		case 0x61:
			return this->_voices[6].volR;
		case 0x71:
			return this->_voices[7].volR;
		case 0x02:
			return this->_voices[0].pitchL;
		case 0x12:
			return this->_voices[1].pitchL;
		case 0x22:
			return this->_voices[2].pitchL;
		case 0x32:
			return this->_voices[3].pitchL;
		case 0x42:
			return this->_voices[4].pitchL;
		case 0x52:
			return this->_voices[5].pitchL;
		case 0x62:
			return this->_voices[6].pitchL;
		case 0x72:
			return this->_voices[7].pitchL;
		case 0x03:
			return this->_voices[0].pitchH;
		case 0x13:
			return this->_voices[1].pitchH;
		case 0x23:
			return this->_voices[2].pitchH;
		case 0x33:
			return this->_voices[3].pitchH;
		case 0x43:
			return this->_voices[4].pitchH;
		case 0x53:
			return this->_voices[5].pitchH;
		case 0x63:
			return this->_voices[6].pitchH;
		case 0x73:
			return this->_voices[7].pitchH;
		case 0x04:
			return this->_voices[0].srcn;
		case 0x14:
			return this->_voices[1].srcn;
		case 0x24:
			return this->_voices[2].srcn;
		case 0x34:
			return this->_voices[3].srcn;
		case 0x44:
			return this->_voices[4].srcn;
		case 0x54:
			return this->_voices[5].srcn;
		case 0x64:
			return this->_voices[6].srcn;
		case 0x74:
			return this->_voices[7].srcn;
		case 0x05:
			return this->_voices[0].adsr1;
		case 0x15:
			return this->_voices[1].adsr1;
		case 0x25:
			return this->_voices[2].adsr1;
		case 0x35:
			return this->_voices[3].adsr1;
		case 0x45:
			return this->_voices[4].adsr1;
		case 0x55:
			return this->_voices[5].adsr1;
		case 0x65:
			return this->_voices[6].adsr1;
		case 0x75:
			return this->_voices[7].adsr1;
		case 0x06:
			return this->_voices[0].adsr2;
		case 0x16:
			return this->_voices[1].adsr2;
		case 0x26:
			return this->_voices[2].adsr2;
		case 0x36:
			return this->_voices[3].adsr2;
		case 0x46:
			return this->_voices[4].adsr2;
		case 0x56:
			return this->_voices[5].adsr2;
		case 0x66:
			return this->_voices[6].adsr2;
		case 0x76:
			return this->_voices[7].adsr2;
		case 0x07:
			return this->_voices[0].gain;
		case 0x17:
			return this->_voices[1].gain;
		case 0x27:
			return this->_voices[2].gain;
		case 0x37:
			return this->_voices[3].gain;
		case 0x47:
			return this->_voices[4].gain;
		case 0x57:
			return this->_voices[5].gain;
		case 0x67:
			return this->_voices[6].gain;
		case 0x77:
			return this->_voices[7].gain;
		case 0x08:
			return this->_voices[0].envx;
		case 0x18:
			return this->_voices[1].envx;
		case 0x28:
			return this->_voices[2].envx;
		case 0x38:
			return this->_voices[3].envx;
		case 0x48:
			return this->_voices[4].envx;
		case 0x58:
			return this->_voices[5].envx;
		case 0x68:
			return this->_voices[6].envx;
		case 0x78:
			return this->_voices[7].envx;
		case 0x09:
			return this->_voices[0].outx;
		case 0x19:
			return this->_voices[1].outx;
		case 0x29:
			return this->_voices[2].outx;
		case 0x39:
			return this->_voices[3].outx;
		case 0x49:
			return this->_voices[4].outx;
		case 0x59:
			return this->_voices[5].outx;
		case 0x69:
			return this->_voices[6].outx;
		case 0x79:
			return this->_voices[7].outx;
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
				kon |= this->_voices[i].kon << i;
			return kon;
		}
		case 0x5C: {
			uint8_t kof = 0;

			for (int i = 0; i < 8; i++)
				kof |= this->_voices[i].kof << i;
			return kof;
		}
		case 0x6C:
			return this->_registers.flg;
		case 0x7C: {
			uint8_t endx = 0;

			for (int i = 0; i < 8; i++)
				endx |= this->_voices[i].endx << i;
			return endx;
		}
		case 0x0D:
			return this->_registers.efb;
		case 0x1D:
			return this->_registers.unused;
		case 0x2D: {
			uint8_t pmon = 0;

			for (int i = 0; i < 8; i++)
				pmon |= this->_voices[i].pmon << i;
			return pmon;
		}
		case 0x3D: {
			uint8_t non = 0;

			for (int i = 0; i < 8; i++)
				non |= this->_voices[i].non << i;
			return non;
		}
		case 0x4D: {
			uint8_t eon = 0;

			for (int i = 0; i < 8; i++)
				eon |= this->_voices[i].eon << i;
			return eon;
		}
		case 0x5D:
			return this->_registers.dir;
		case 0x6D:
			return this->_registers.esa;
		case 0x7D:
			return this->_registers.edl;
		case 0x0F:
			return this->_voices[0].coeff;
		case 0x1F:
			return this->_voices[1].coeff;
		case 0x2F:
			return this->_voices[2].coeff;
		case 0x3F:
			return this->_voices[3].coeff;
		case 0x4F:
			return this->_voices[4].coeff;
		case 0x5F:
			return this->_voices[5].coeff;
		case 0x6F:
			return this->_voices[6].coeff;
		case 0x7F:
			return this->_voices[7].coeff;
		default:
			throw InvalidAddress("DSP Registers read", addr);
		}
	}

	void DSP::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00:
			this->_voices[0].volL = data;
			break;
		case 0x10:
			this->_voices[1].volL = data;
			break;
		case 0x20:
			this->_voices[2].volL = data;
			break;
		case 0x30:
			this->_voices[3].volL = data;
			break;
		case 0x40:
			this->_voices[4].volL = data;
			break;
		case 0x50:
			this->_voices[5].volL = data;
			break;
		case 0x60:
			this->_voices[6].volL = data;
			break;
		case 0x70:
			this->_voices[7].volL = data;
			break;
		case 0x01:
			this->_voices[0].volR = data;
			break;
		case 0x11:
			this->_voices[1].volR = data;
			break;
		case 0x21:
			this->_voices[2].volR = data;
			break;
		case 0x31:
			this->_voices[3].volR = data;
			break;
		case 0x41:
			this->_voices[4].volR = data;
			break;
		case 0x51:
			this->_voices[5].volR = data;
			break;
		case 0x61:
			this->_voices[6].volR = data;
			break;
		case 0x71:
			this->_voices[7].volR = data;
			break;
		case 0x02:
			this->_voices[0].pitchL = data;
			break;
		case 0x12:
			this->_voices[1].pitchL = data;
			break;
		case 0x22:
			this->_voices[2].pitchL = data;
			break;
		case 0x32:
			this->_voices[3].pitchL = data;
			break;
		case 0x42:
			this->_voices[4].pitchL = data;
			break;
		case 0x52:
			this->_voices[5].pitchL = data;
			break;
		case 0x62:
			this->_voices[6].pitchL = data;
			break;
		case 0x72:
			this->_voices[7].pitchL = data;
			break;
		case 0x03:
			this->_voices[0].pitchH = data;
			break;
		case 0x13:
			this->_voices[1].pitchH = data;
			break;
		case 0x23:
			this->_voices[2].pitchH = data;
			break;
		case 0x33:
			this->_voices[3].pitchH = data;
			break;
		case 0x43:
			this->_voices[4].pitchH = data;
			break;
		case 0x53:
			this->_voices[5].pitchH = data;
			break;
		case 0x63:
			this->_voices[6].pitchH = data;
			break;
		case 0x73:
			this->_voices[7].pitchH = data;
			break;
		case 0x04:
			this->_voices[0].srcn = data;
			break;
		case 0x14:
			this->_voices[1].srcn = data;
			break;
		case 0x24:
			this->_voices[2].srcn = data;
			break;
		case 0x34:
			this->_voices[3].srcn = data;
			break;
		case 0x44:
			this->_voices[4].srcn = data;
			break;
		case 0x54:
			this->_voices[5].srcn = data;
			break;
		case 0x64:
			this->_voices[6].srcn = data;
			break;
		case 0x74:
			this->_voices[7].srcn = data;
			break;
		case 0x05:
			this->_voices[0].adsr1 = data;
			break;
		case 0x15:
			this->_voices[1].adsr1 = data;
			break;
		case 0x25:
			this->_voices[2].adsr1 = data;
			break;
		case 0x35:
			this->_voices[3].adsr1 = data;
			break;
		case 0x45:
			this->_voices[4].adsr1 = data;
			break;
		case 0x55:
			this->_voices[5].adsr1 = data;
			break;
		case 0x65:
			this->_voices[6].adsr1 = data;
			break;
		case 0x75:
			this->_voices[7].adsr1 = data;
			break;
		case 0x06:
			this->_voices[0].adsr2 = data;
			break;
		case 0x16:
			this->_voices[1].adsr2 = data;
			break;
		case 0x26:
			this->_voices[2].adsr2 = data;
			break;
		case 0x36:
			this->_voices[3].adsr2 = data;
			break;
		case 0x46:
			this->_voices[4].adsr2 = data;
			break;
		case 0x56:
			this->_voices[5].adsr2 = data;
			break;
		case 0x66:
			this->_voices[6].adsr2 = data;
			break;
		case 0x76:
			this->_voices[7].adsr2 = data;
			break;
		case 0x07:
			this->_voices[0].gain = data;
			break;
		case 0x17:
			this->_voices[1].gain = data;
			break;
		case 0x27:
			this->_voices[2].gain = data;
			break;
		case 0x37:
			this->_voices[3].gain = data;
			break;
		case 0x47:
			this->_voices[4].gain = data;
			break;
		case 0x57:
			this->_voices[5].gain = data;
			break;
		case 0x67:
			this->_voices[6].gain = data;
			break;
		case 0x77:
			this->_voices[7].gain = data;
			break;
		case 0x08:
			this->_voices[0].envx = data;
			break;
		case 0x18:
			this->_voices[1].envx = data;
			break;
		case 0x28:
			this->_voices[2].envx = data;
			break;
		case 0x38:
			this->_voices[3].envx = data;
			break;
		case 0x48:
			this->_voices[4].envx = data;
			break;
		case 0x58:
			this->_voices[5].envx = data;
			break;
		case 0x68:
			this->_voices[6].envx = data;
			break;
		case 0x78:
			this->_voices[7].envx = data;
			break;
		case 0x09:
			this->_voices[0].outx = data;
			break;
		case 0x19:
			this->_voices[1].outx = data;
			break;
		case 0x29:
			this->_voices[2].outx = data;
			break;
		case 0x39:
			this->_voices[3].outx = data;
			break;
		case 0x49:
			this->_voices[4].outx = data;
			break;
		case 0x59:
			this->_voices[5].outx = data;
			break;
		case 0x69:
			this->_voices[6].outx = data;
			break;
		case 0x79:
			this->_voices[7].outx = data;
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
				this->_voices[i].kon |= data << i;
			break;
		case 0x5C:
			for (int i = 0; i < 8; i++)
				this->_voices[i].kof |= data << i;
			break;
		case 0x6C:
			this->_registers.flg = data;
			break;
		case 0x7C:
			for (int i = 0; i < 8; i++)
				this->_voices[i].endx |= data << i;
			break;
		case 0x0D:
			this->_registers.efb = data;
			break;
		case 0x1D:
			this->_registers.unused = data;
			break;
		case 0x2D:
			for (int i = 0; i < 8; i++)
				this->_voices[i].pmon |= data << i;
			break;
		case 0x3D:
			for (int i = 0; i < 8; i++)
				this->_voices[i].non |= data << i;
			break;
		case 0x4D:
			for (int i = 0; i < 8; i++)
				this->_voices[i].eon |= data << i;
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
			this->_voices[0].coeff = data;
			break;
		case 0x1F:
			this->_voices[1].coeff = data;
			break;
		case 0x2F:
			this->_voices[2].coeff = data;
			break;
		case 0x3F:
			this->_voices[3].coeff = data;
			break;
		case 0x4F:
			this->_voices[4].coeff = data;
			break;
		case 0x5F:
			this->_voices[5].coeff = data;
			break;
		case 0x6F:
			this->_voices[6].coeff = data;
			break;
		case 0x7F:
			this->_voices[7].coeff = data;
			break;
		default:
			throw InvalidAddress("DSP Registers write", addr);
		}
	}

	void DSP::update()
    {
	    this->_state.voice = (this->_state.voice + 1) % 32;
    }

	Registers DSP::getRegisters()
	{
		return this->_registers;
	}

	std::array<Voice, 8> DSP::getVoices()
	{
		return this->_voices;
	}

    int32_t DSP::getSamplesCount()
    {
        return this->_state.buffer - this->_state.bufferStart;
    }

    std::string DSP::getName()
	{
		return "DSP";
	}

	Component DSP::getComponent()
	{
		return Apu;
	}
}