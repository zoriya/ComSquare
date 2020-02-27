//
// Created by Melefo on 27/02/2020.
//

#include "IPL.hpp"
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::IPL
{
	IPL::IPL()
	{ }

	IPL::~IPL()
	{ }

	uint8_t IPL::read(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("IPL read", addr);
		return this->_data[addr];
	}

	void IPL::write(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress("IPL write", addr);
		this->_data[addr] = data;
	}
}