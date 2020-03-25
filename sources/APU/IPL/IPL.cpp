//
// Created by Melefo on 27/02/2020.
//

#include "IPL.hpp"

#include <utility>
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::APU::IPL
{
	IPL::IPL(Component type, std::string iplName)
		: _iplType(type),
		_iplName(std::move(iplName))
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

	std::string IPL::getName()
	{
		return this->_iplName;
	}

	Component IPL::getComponent()
	{
		return this->_iplType;
	}
}