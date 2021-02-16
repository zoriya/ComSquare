//
// Created by melefo on 2/3/21.
//

#include <algorithm>
#include <cstdint>
#include "DSP.hpp"

namespace ComSquare::APU::DSP
{
	int32_t DSP::interpolate(const Voice &voice)
	{
		int32_t interpolated;
		uint8_t offset = voice.gaussOffset >> 4;
		int forward = 255 - offset;
		int reverse = offset;

		offset = (voice.sampleOffset + (voice.gaussOffset >> 12)) % 12;
		interpolated = this->_gauss[forward] * voice.samples[offset++] >> 11;
		offset %= 12;
		interpolated += this->_gauss[forward + 256] * voice.samples[offset++] >> 11;
		offset %= 12;
		interpolated += this->_gauss[reverse + 256] * voice.samples[offset++] >> 11;
		offset %= 12;
		interpolated = static_cast<int16_t>(interpolated);
		interpolated += this->_gauss[reverse] * voice.samples[offset] >> 11;

		return std::clamp(interpolated, 0, 16) & ~1;
	}
}