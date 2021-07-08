//
// Created by melefo on 2/1/21.
//

#include "DSP.hpp"
#include "APU/APU.hpp"
#include <algorithm>

namespace ComSquare::APU::DSP
{
	void DSP::voiceOutput(Voice &voice, bool channel)
	{
		int out = this->_latch.output * voice.volume[channel] >> 7;

		this->_master.output[channel] += out;
		if (!voice.echo)
			return;
		this->_echo.volume[channel] += out;
	}

	void DSP::voice1(Voice &voice)
	{
		this->_brr.address = (this->_brr.offsetAddr << 8) + (this->_brr.source << 2);
		this->_brr.source = voice.srcn;
	}

	void DSP::voice2(Voice &voice)
	{
		uint16_t addr = this->_brr.address;

		if (!voice.konDelay)
			addr += 2;
		this->_brr.nextAddress = this->_readRAM(addr++);
		this->_brr.nextAddress += this->_readRAM(addr++) << 8;
		this->_latch.adsr1 = voice.adsr1;
		this->_latch.pitch = voice.pitch & 0xFF;
	}

	void DSP::voice3(Voice &voice)
	{
		this->voice3a(voice);
		this->voice3b(voice);
		this->voice3c(voice);
	}

	void DSP::voice3a(Voice &voice)
	{
		this->_latch.pitch |= (voice.pitchH & 0x3F) << 8;
	}

	void DSP::voice3b(Voice &voice)
	{
		this->_brr.header = this->_readRAM(this->_brr.address);
		this->_brr.value = this->_readRAM(this->_brr.address + voice.brrOffset);
	}

	void DSP::voice3c(Voice &voice)
	{
		if (voice.prevPmon)
			this->_latch.pitch += (this->_latch.output >> 5) * this->_latch.pitch >> 10;

		if (voice.konDelay) {
			if (voice.konDelay == 5) {
				voice.brrAddress = this->_brr.nextAddress;
				voice.brrOffset = 1;
				voice.sampleOffset = 0;
				this->_brr.header = 0;
			}

			voice.envelope = 0;
			voice.hiddenEnvelope = 0;
			voice.gaussOffset = 0;
			voice.konDelay -= 1;
			if (voice.konDelay & 3)
				voice.gaussOffset = 0x4000;
			this->_latch.pitch = 0;
		}

		int32_t interpolated = this->interpolate(voice);

		if (voice.tempNon)
			interpolated = this->_noise.lfsr << 1;

		this->_latch.output = interpolated * voice.envelope >> 11 & ~1;
		voice.envx = voice.envelope >> 4;

		if (this->_master.reset || (this->_brr.header & 3) == 1) {
			voice.envelope = 0;
			voice.envelopeMode = Envelope::Release;
		}

		if (this->_timer.sample)
		{
			if (voice.tempKof)
				voice.envelopeMode = Envelope::Release;
			if (voice.tempKon) {
				voice.konDelay = 5;
				voice.envelopeMode = Envelope::Attack;
			}
		}

		if (!voice.konDelay)
			this->runEnvelope(voice);
	}

	void DSP::voice4(Voice &voice)
	{
		voice.loop = false;
		if (voice.gaussOffset >= 0x4000) {
			this->decodeBRR(voice);
			voice.brrOffset += 2;
			if (voice.brrOffset >= 9) {
				voice.brrOffset = voice.brrAddress + 9;
				if (this->_brr.header & 0b1) {
					voice.brrAddress = this->_brr.nextAddress;
					voice.loop = true;
				}
				voice.brrOffset = 1;
			}
		}

		voice.gaussOffset = (voice.gaussOffset & 0x3FFF) + this->_latch.pitch;
		if (voice.gaussOffset > 0x7FFF)
			voice.gaussOffset = 0x7FFF;
		this->voiceOutput(voice, 0);
	}

	void DSP::voice5(Voice &voice)
	{
		this->voiceOutput(voice, 1);

		voice.endx |= voice.loop;
		if (voice.konDelay == 5)
			voice.endx = 0x00;
	}

	void DSP::voice6(Voice &)
	{
		this->_latch.outx = this->_latch.output >> 8;
	}

	void DSP::voice7(Voice &voice)
	{
		this->_latch.envx = voice.envx;
	}

	void DSP::voice8(Voice &voice)
	{
		voice.outx = this->_latch.outx;
	}

	void DSP::voice9(Voice &voice)
	{
		voice.envx = this->_latch.envx;
	}
}