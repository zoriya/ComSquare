//
// Created by Melefo on 21/02/2020.
//

#include "APU/APU.hpp"
#include "Utility/Utility.hpp"

namespace ComSquare::APU
{
	int APU::CALL(uint24_t abs)
	{
		this->PUSH(this->_internalRegisters.pch);
		this->PUSH(this->_internalRegisters.pcl);
		this->_internalRegisters.pc = abs;
		return 8;
	}

	int APU::PCALL()
	{
		this->CALL(0xFF00u + this->_getImmediateData());
		return 6;
	}

	int APU::TCALL(uint8_t bit)
	{
		this->CALL(this->_internalRead(0xFFDE - bit * 2));
		return 8;
	}

	int APU::BRK()
	{
		this->_internalRegisters.b = true;
		this->PUSH(this->_internalRegisters.pch);
		this->PUSH(this->_internalRegisters.pcl);
		this->PUSH(this->_internalRegisters.psw);
		this->_internalRegisters.i = false;
		this->_internalRegisters.pch = this->_internalRead(0xFFDF);
		this->_internalRegisters.pcl = this->_internalRead(0xFFDE);
		return 8;
	}

	int APU::RET()
	{
		 this->POP(this->_internalRegisters.pch);
		 this->POP(this->_internalRegisters.pcl);
		 return 5;
	}

	int APU::RETI()
	{
		this->POP(this->_internalRegisters.psw);
		this->RET();
		return 6;
	}
}