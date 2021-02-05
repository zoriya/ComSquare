//
// Created by melefo on 2/3/21.
//

#include "DSP.hpp"

namespace ComSquare::APU::DSP
{
	void DSP::runEnvelope(Voice &voice)
	{
		int32_t envelope = voice.envelope;

		if (voice.envelopeMode == Envelope::Release) {
			envelope -= 0x08;
			if (envelope < 0)
				envelope = 0;
			voice.envelope = envelope;
			return;
		}

		int32_t rate;
		int32_t mode;
		int32_t data = voice.adsr2;
		if (this->_latch.adsr1 & 0b10000000) {
			if (voice.envelopeMode >= Envelope::Decay) {
				envelope -= 1;
				envelope -= envelope >> 8;
				rate = data & 0b11111;
				if (voice.envelopeMode == Envelope::Decay)
					rate = ((this->_latch.adsr1 >> 3) & 0x0E) + 0x10;
			}
			else {
				rate = ((this->_latch.adsr1 & 0b1111) << 1) + 1;
				if (rate < 0b11111)
					envelope += 0x20;
				else
					envelope += 0x400;
			}
		}
		else {
			data = voice.gain;
			mode = data >> 5;
			if (mode < 4) {
				envelope = data << 4;
				rate = 0b11111;
			}
			else {
				rate = data & 0b11111;
				if (mode == 4)
					envelope -= 0x20;
				else if (mode < 6) {
					envelope -= 1;
					envelope -= envelope >> 8;
				}
				else {
					envelope += 0x20;
					if (mode > 6 && voice.hiddenEnvelope >= 0x600)
						envelope += 0x08 - 0x20;
				}
			}
		}

		if (envelope >> 8 == (data >> 5) && voice.envelopeMode == Envelope::Decay)
			voice.envelopeMode = Envelope::Sustain;
		voice.hiddenEnvelope = envelope;
		if (static_cast<uint32_t>(envelope) > 0x7FF) {
			if (envelope < 0)
				envelope = 0;
			else
				envelope = 0x7FF;
			if (voice.envelopeMode == Envelope::Attack)
				voice.envelopeMode = Envelope::Decay;
		}

		if (this->timerPoll(rate))
			voice.envelope = envelope;
	}
}