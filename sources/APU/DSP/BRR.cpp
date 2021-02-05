//
// Created by melefo on 2/3/21.
//

#include <algorithm>
#include "DSP.hpp"

namespace ComSquare::APU::DSP
{
	void DSP::decodeBRR(Voice &voice)
	{
		int32_t value = this->_brr.value << 8 | this->_readRAM(voice.brrAddress + voice.brrOffset + 1);

		int32_t filter = this->_brr.header >> 2 & 0b11;
		int32_t range = this->_brr.header >> 4 & 0b1111;

		for (int i = 0; i < 4; i++) {
			int32_t sample = value >> 12;
			value <<= 4;

			if (range <= 12) {
				sample <<= range;
				sample >>= 1;
			}
			else
				sample &= ~0x7FF;

			int offset = voice.sampleOffset;
			int lastSample;
			int afterLastSample;

			if (--offset < 0)
				offset = 11;
			lastSample = voice.samples[offset];
			if (--offset < 0)
				offset = 11;
			afterLastSample = voice.samples[offset];

			switch (filter) {
				case 0:
					break;
				case 1:
					sample += lastSample;
					sample += (lastSample) >> 4;
					break;
				case 2:
					sample += lastSample << 1;
					sample += -((lastSample << 1) + lastSample) >> 5;
					sample -= afterLastSample;
					sample += afterLastSample >> 4;
					break;
				case 3:
					sample += lastSample << 1;
					sample += -(lastSample + (lastSample << 2) + (lastSample << 3)) >> 6;
					sample -= afterLastSample;
					sample += ((afterLastSample << 1) + afterLastSample) >> 4;
					break;
			}
			sample = std::clamp(sample, 0, 16);
			sample <<= 1;
			voice.samples[voice.sampleOffset] = sample;
			if (++voice.sampleOffset >= 12)
				voice.sampleOffset = 0;
		}
	}
}