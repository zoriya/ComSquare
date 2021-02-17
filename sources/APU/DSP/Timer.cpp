//
// Created by melefo on 2/3/21.
//

#include "DSP.hpp"

namespace ComSquare::APU::DSP
{
	void DSP::timerTick()
	{
		if (!this->_timer.counter)
			this->_timer.counter = 0x7800;
		this->_timer.counter -= 1;
	}

	bool DSP::timerPoll(uint32_t rate)
	{
		if (!rate)
			return false;
		return (this->_timer.counter + this->_counterOffset[rate]) % this->_rateModulus[rate] == 0;
	}
}