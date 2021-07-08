//
// Created by Melefo on 28/01/2020.
//

#include "DSP.hpp"
#include "APU/APU.hpp"
#include "Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::DSP
{
	DSP::DSP(Renderer::IRenderer &renderer, MemoryMap &map)
		: _state(this->_soundBuffer, this->_soundBuffer.size() / 2),
		  _map(map),
		  _renderer(renderer)
	{}

	uint8_t DSP::read(uint24_t addr) const
	{
		switch (addr) {
		case 0x00:
			return this->_voices[0].volume[0];
		case 0x10:
			return this->_voices[1].volume[0];
		case 0x20:
			return this->_voices[2].volume[0];
		case 0x30:
			return this->_voices[3].volume[0];
		case 0x40:
			return this->_voices[4].volume[0];
		case 0x50:
			return this->_voices[5].volume[0];
		case 0x60:
			return this->_voices[6].volume[0];
		case 0x70:
			return this->_voices[7].volume[0];
		case 0x01:
			return this->_voices[0].volume[1];
		case 0x11:
			return this->_voices[1].volume[1];
		case 0x21:
			return this->_voices[2].volume[1];
		case 0x31:
			return this->_voices[3].volume[1];
		case 0x41:
			return this->_voices[4].volume[1];
		case 0x51:
			return this->_voices[5].volume[1];
		case 0x61:
			return this->_voices[6].volume[1];
		case 0x71:
			return this->_voices[7].volume[1];
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
		case 0x18:
		case 0x28:
		case 0x38:
		case 0x48:
		case 0x58:
		case 0x68:
		case 0x78:
			return this->_latch.envx;
		case 0x09:
		case 0x19:
		case 0x29:
		case 0x39:
		case 0x49:
		case 0x59:
		case 0x69:
		case 0x79:
			return this->_latch.outx;
		case 0x0C:
			return this->_master.volume[0];
		case 0x1C:
			return this->_master.volume[1];
		case 0x2C:
			return this->_echo.volume[0];
		case 0x3C:
			return this->_echo.volume[1];
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
		case 0x6C: {
			uint8_t flg = 0;
			flg += this->_master.reset << 7;
			flg += this->_master.mute << 6;
			flg += this->_echo.enabled << 5;
			flg += this->_noise.clock;
			return flg;
		}
		case 0x7C: {
			uint8_t endx = 0;

			for (int i = 0; i < 8; i++)
				endx |= this->_voices[i].endx << i;
			return endx;
		}
		case 0x0D:
			return this->_echo.feedback;
		case 0x1D:
			return this->_master.unused;
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
			return this->_brr.offset;
		case 0x6D:
			return this->_echo.data;
		case 0x7D:
			return this->_echo.delay;
		case 0x0F:
			return this->_echo.FIR[0];
		case 0x1F:
			return this->_echo.FIR[1];
		case 0x2F:
			return this->_echo.FIR[2];
		case 0x3F:
			return this->_echo.FIR[3];
		case 0x4F:
			return this->_echo.FIR[4];
		case 0x5F:
			return this->_echo.FIR[5];
		case 0x6F:
			return this->_echo.FIR[6];
		case 0x7F:
			return this->_echo.FIR[7];
		default:
			throw InvalidAddress("DSP Registers read", addr);
		}
	}

	void DSP::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x00:
			this->_voices[0].volume[0] = data;
			break;
		case 0x10:
			this->_voices[1].volume[0] = data;
			break;
		case 0x20:
			this->_voices[2].volume[0] = data;
			break;
		case 0x30:
			this->_voices[3].volume[0] = data;
			break;
		case 0x40:
			this->_voices[4].volume[0] = data;
			break;
		case 0x50:
			this->_voices[5].volume[0] = data;
			break;
		case 0x60:
			this->_voices[6].volume[0] = data;
			break;
		case 0x70:
			this->_voices[7].volume[0] = data;
			break;
		case 0x01:
			this->_voices[0].volume[1] = data;
			break;
		case 0x11:
			this->_voices[1].volume[1] = data;
			break;
		case 0x21:
			this->_voices[2].volume[1] = data;
			break;
		case 0x31:
			this->_voices[3].volume[1] = data;
			break;
		case 0x41:
			this->_voices[4].volume[1] = data;
			break;
		case 0x51:
			this->_voices[5].volume[1] = data;
			break;
		case 0x61:
			this->_voices[6].volume[1] = data;
			break;
		case 0x71:
			this->_voices[7].volume[1] = data;
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
		case 0x19:
		case 0x29:
		case 0x39:
		case 0x49:
		case 0x59:
		case 0x69:
		case 0x79:
			this->_latch.outx = data;
			break;
		case 0x0C:
			this->_master.volume[0] = data;
			break;
		case 0x1C:
			this->_master.volume[1] = data;
			break;
		case 0x2C:
			this->_echo.volume[0] = data;
			break;
		case 0x3C:
			this->_echo.volume[1] = data;
			break;
		case 0x4C:
			for (int i = 0; i < 8; i++) {
				this->_voices[i].kon |= data << i;
			}
			break;
		case 0x5C:
			for (int i = 0; i < 8; i++)
				this->_voices[i].kof |= data << i;
			break;
		case 0x6C:
			this->_master.reset = data >> 7;
			this->_master.mute = (data >> 6) & 0b1;
			this->_echo.enabled = (data >> 5) & 0b1;
			this->_noise.clock = data & 0b1111;
			break;
		case 0x7C:
			for (int i = 0; i < 8; i++)
				this->_voices[i].endx |= data << i;
			break;
		case 0x0D:
			this->_echo.feedback = data;
			break;
		case 0x1D:
			this->_master.unused = data;
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
			this->_brr.offset = data;
			break;
		case 0x6D:
			this->_echo.data = data;
			break;
		case 0x7D:
			this->_echo.delay = data;
			break;
		case 0x0F:
			this->_echo.FIR[0] = data;
			break;
		case 0x1F:
			this->_echo.FIR[1] = data;
			break;
		case 0x2F:
			this->_echo.FIR[2] = data;
			break;
		case 0x3F:
			this->_echo.FIR[3] = data;
			break;
		case 0x4F:
			this->_echo.FIR[4] = data;
			break;
		case 0x5F:
			this->_echo.FIR[5] = data;
			break;
		case 0x6F:
			this->_echo.FIR[6] = data;
			break;
		case 0x7F:
			this->_echo.FIR[7] = data;
			break;
		default:
			throw InvalidAddress("DSP Registers write", addr);
		}
	}

	uint8_t DSP::_readRAM(uint24_t addr)
	{
		switch (addr) {
		case 0x0000 ... 0x00EF:
			return this->_map.Page0.read(addr);
		case 0x0100 ... 0x01FF:
			return this->_map.Page1.read(addr - 0x0100);
		case 0x0200 ... 0xFFBF:
			return this->_map.Memory.read(addr - 0x200);
		case 0xFFC0 ... 0xFFFF:
			return this->_map.IPL.read(addr - 0xFFC0);
		default:
			throw InvalidAddress("DSP read", addr);
		}
	}

	void DSP::_writeRAM(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0000 ... 0x00EF:
			this->_map.Page0.write(addr, data);
			break;
		case 0x0100 ... 0x01FF:
			this->_map.Page1.write(addr - 0x0100, data);
			break;
		case 0x0200 ... 0xFFBF:
			this->_map.Memory.write(addr - 0x200, data);
			break;
		case 0xFFC0 ... 0xFFFF:
			this->_map.IPL.write(addr - 0xFFC0, data);
			break;
		default:
			throw InvalidAddress("DSP write", addr);
		}
	}

	void DSP::update()
	{
		switch (this->_state.voice) {
		case 0:
			this->voice5(this->_voices[0]);
			this->voice2(this->_voices[1]);
			break;
		case 1:
			this->voice6(this->_voices[0]);
			this->voice3(this->_voices[1]);
			break;
		case 2:
			this->voice7(this->_voices[0]);
			this->voice4(this->_voices[1]);
			this->voice1(this->_voices[3]);
			break;
		case 3:
			this->voice8(this->_voices[0]);
			this->voice5(this->_voices[1]);
			this->voice2(this->_voices[2]);
			break;
		case 4:
			this->voice9(this->_voices[0]);
			this->voice6(this->_voices[1]);
			this->voice3(this->_voices[2]);
			break;
		case 5:
			this->voice7(this->_voices[1]);
			this->voice4(this->_voices[2]);
			this->voice1(this->_voices[4]);
			break;
		case 6:
			this->voice8(this->_voices[1]);
			this->voice5(this->_voices[2]);
			this->voice2(this->_voices[3]);
			break;
		case 7:
			this->voice9(this->_voices[1]);
			this->voice6(this->_voices[2]);
			this->voice3(this->_voices[3]);
			break;
		case 8:
			this->voice7(this->_voices[2]);
			this->voice4(this->_voices[3]);
			this->voice1(this->_voices[5]);
			break;
		case 9:
			this->voice8(this->_voices[2]);
			this->voice5(this->_voices[3]);
			this->voice2(this->_voices[4]);
			break;
		case 10:
			this->voice9(this->_voices[2]);
			this->voice6(this->_voices[3]);
			this->voice3(this->_voices[4]);
			break;
		case 11:
			this->voice7(this->_voices[3]);
			this->voice4(this->_voices[4]);
			this->voice1(this->_voices[6]);
			break;
		case 12:
			this->voice8(this->_voices[3]);
			this->voice5(this->_voices[4]);
			this->voice2(this->_voices[5]);
			break;
		case 13:
			this->voice9(this->_voices[3]);
			this->voice6(this->_voices[4]);
			this->voice3(this->_voices[5]);
			break;
		case 14:
			this->voice7(this->_voices[4]);
			this->voice4(this->_voices[5]);
			this->voice1(this->_voices[7]);
			break;
		case 15:
			this->voice8(this->_voices[4]);
			this->voice5(this->_voices[5]);
			this->voice2(this->_voices[6]);
			break;
		case 16:
			this->voice9(this->_voices[4]);
			this->voice6(this->_voices[5]);
			this->voice3(this->_voices[6]);
			break;
		case 17:
			this->voice1(this->_voices[0]);
			this->voice7(this->_voices[5]);
			this->voice4(this->_voices[6]);
			break;
		case 18:
			this->voice8(this->_voices[5]);
			this->voice5(this->_voices[6]);
			this->voice2(this->_voices[7]);
			break;
		case 19:
			this->voice9(this->_voices[5]);
			this->voice6(this->_voices[6]);
			this->voice3(this->_voices[7]);
			break;
		case 20:
			this->voice1(this->_voices[1]);
			this->voice7(this->_voices[6]);
			this->voice4(this->_voices[7]);
			break;
		case 21:
			this->voice8(this->_voices[6]);
			this->voice5(this->_voices[7]);
			this->voice2(this->_voices[0]);
			break;
		case 22:
			this->voice3a(this->_voices[0]);
			this->voice9(this->_voices[6]);
			this->voice6(this->_voices[7]);
			echo22();
			break;
		case 23:
			this->voice7(this->_voices[7]);
			echo23();
			break;
		case 24:
			this->voice8(this->_voices[7]);
			echo24();
			break;
		case 25:
			this->voice3b(this->_voices[0]);
			this->voice9(this->_voices[7]);
			echo25();
			break;
		case 26:
			echo26();
			break;
		case 27:
			this->misc27();
			this->echo27();
			break;
		case 28:
			this->misc28();
			this->echo28();
			break;
		case 29:
			this->misc29();
			this->echo29();
			break;
		case 30:
			this->misc30();
			this->voice3c(this->_voices[0]);
			this->echo30();
			break;
		case 31:
			this->voice4(this->_voices[0]);
			this->voice1(this->_voices[2]);
			break;
		}
		this->_state.voice = (this->_state.voice + 1) % 32;
		int32_t samples = this->getSamplesCount();
		if (samples > 0)
			this->_renderer.playAudio(std::span(this->_soundBuffer.begin(), samples / 2));
	}

	uint24_t DSP::getSize() const
	{
		return 0x7F;
	}

	const std::array<Voice, 8> &DSP::getVoices() const
	{
		return this->_voices;
	}

	const Master &DSP::getMaster() const
	{
		return this->_master;
	}

	const Echo &DSP::getEcho() const
	{
		return this->_echo;
	}

	const Noise &DSP::getNoise() const
	{
		return this->_noise;
	}

	const BRR &DSP::getBrr() const
	{
		return this->_brr;
	}

	const Latch &DSP::getLatch() const
	{
		return this->_latch;
	}

	int32_t DSP::getSamplesCount() const
	{
		return this->_state.bufferOffset;
	}
}