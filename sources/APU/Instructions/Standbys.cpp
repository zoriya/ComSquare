//
// Created by Melefo on 11/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::NOP()
	{
		return 2;
	}

	int APU::SLEEP()
	{
		this->_state = Sleeping;
		return 3;
	}

	int APU::STOP()
	{
		this->_state = Stopped;
		return 3;
	}
}