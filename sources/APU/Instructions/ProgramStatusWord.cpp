//
// Created by Melefo on 11/02/2020.
//

#include "APU/APU.hpp"

namespace ComSquare::APU
{
	int APU::CLRC()
	{
		this->_internalRegisters.c = false;
		return 2;
	}

	int APU::SETC()
	{
		this->_internalRegisters.c = true;
		return 2;
	}

	int APU::NOTC()
	{
		this->_internalRegisters.c = !this->_internalRegisters.c;
		return 3;
	}

	int APU::CLRV()
	{
		this->_internalRegisters.v = false;
		this->_internalRegisters.h = false;
		return 2;
	}

	int APU::CLRP()
	{
		this->_internalRegisters.p = false;
		return 2;
	}

	int APU::SETP()
	{
		this->_internalRegisters.p = true;
		return 2;
	}

	int APU::EI()
	{
		this->_internalRegisters.i = true;
		return 3;
	}

	int APU::DI()
	{
		this->_internalRegisters.i = false;
		return 3;
	}
}