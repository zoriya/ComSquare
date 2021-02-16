//
// Created by melefo on 2/1/21.
//

#include "DSP.hpp"

namespace ComSquare::APU::DSP
{
	int32_t DSP::loadFIR(bool channel, int fir)
	{
		int32_t sample = this->_echo.history[channel][this->_echo.historyOffset + fir + 1];

		return (sample * this->_echo.FIR[fir]) >> 6;
	}

	void DSP::loadEcho(bool channel)
	{
		uint16_t address = this->_echo.address + channel * 2;
		uint8_t low = this->_readRAM(address++);
		uint8_t high = this->_readRAM(address);
		int16_t echo = (high << 8) + low;

		this->_echo.history[channel][this->_echo.historyOffset] = echo >> 1;
	}

	void DSP::writeEcho(bool channel)
	{
		if (!this->_echo.toggle) {
			uint16_t address = this->_echo.address + channel * 2;
			int16_t sample = this->_echo.output[channel];

			this->_writeRAM(address++, sample);
			this->_writeRAM(address, sample >> 8);
		}
		this->_echo.output[channel] = 0;
	}

	int16_t DSP::outputEcho(bool channel)
	{
		int16_t master = this->_master.output[channel] * this->_master.volume[channel] >> 7;
		int16_t echo = this->_echo.input[channel] * this->_echo.input[channel] >> 7;

		return master + echo;
	}

	void DSP::echo22()
	{
		this->_echo.historyOffset += 1;
		this->_echo.address = (this->_echo.value << 8) + this->_echo.offset;

		this->loadEcho(0);

		this->_echo.input[0] = this->loadFIR(0, 0);
		this->_echo.input[1] = this->loadFIR(1, 0);
	}

	void DSP::echo23()
	{
		this->loadEcho(1);

		this->_echo.input[0] += this->loadFIR(0, 1) + this->loadFIR(0, 2);
		this->_echo.input[1] += this->loadFIR(1, 1) + this->loadFIR(1, 2);
	}

	void DSP::echo24()
	{
		this->_echo.input[0] += this->loadFIR(0, 3) + this->loadFIR(0, 4) + this->loadFIR(0, 5);
		this->_echo.input[1] += this->loadFIR(1, 3) + this->loadFIR(1, 4) + this->loadFIR(1, 5);
	}

	void DSP::echo25()
	{
		this->_echo.input[0] += this->loadFIR(0, 6) + this->loadFIR(0, 7);
		this->_echo.input[1] += this->loadFIR(1, 6) + this->loadFIR(1, 7);
	}

	void DSP::echo26()
	{
		this->_master.output[0] = this->outputEcho(0);

		this->_echo.output[0] += this->_echo.input[0] * this->_echo.feedback >> 7;
		this->_echo.output[1] += this->_echo.input[1] * this->_echo.feedback >> 7;
	}

	void DSP::echo27()
	{
		int16_t outputLeft = this->_master.output[0];
		int16_t outputRight = this->outputEcho(1);

		this->_master.output[0] = 0;
		this->_master.output[1] = 0;

		if (this->_master.mute) {
			outputLeft = 0;
			outputRight = 0;
		}

		this->_state.buffer[this->_state.bufferOffset] = outputLeft;
		this->_state.buffer[this->_state.bufferOffset + 1] = outputRight;
		this->_state.bufferOffset += 2;
		if (this->_state.bufferOffset >= this->_state.bufferSize)
			this->_state.bufferOffset = 0;
	}

	void DSP::echo28()
	{
		this->_echo.toggle = this->_echo.enabled;
	}

	void DSP::echo29()
	{
		this->_echo.value = this->_echo.data;

		if (!this->_echo.offset)
			this->_echo.length = this->_echo.delay << 11;

		this->_echo.offset += 4;
		if (this->_echo.offset >= this->_echo.length)
			this->_echo.offset = 0;

		this->writeEcho(0);

		echo28();
	}

	void DSP::echo30()
	{
		this->writeEcho(1);
	}

	void DSP::misc27()
	{
		for (int i = 0; i < 8; i++)
			this->_voices[i].prevPmon = this->_voices[i].pmon;
	}

	void DSP::misc28()
	{
		for (int i = 0; i < 8; i++)
			this->_voices[i].tempNon = this->_voices[i].non;
		this->_brr.offsetAddr = this->_brr.offset;
	}

	void DSP::misc29()
	{
		this->_timer.sample = !this->_timer.sample;
		if (this->_timer.sample) {
			for (int i = 0; i < 8; i++)
				this->_voices[i].kon = 0;
		}
	}

	void DSP::misc30()
	{
		if (this->_timer.sample) {
			for (int i = 0; i < 8; i++)
				this->_voices[i].kof = 0;
		}

		this->timerTick();

		if (!this->timerPoll(this->_noise.clock))
			return;
		int32_t feedback = this->_noise.lfsr << 13 ^ this->_noise.lfsr << 14;

		this->_noise.lfsr = (feedback & 0x4000) ^ this->_noise.lfsr >> 1;
	}
}