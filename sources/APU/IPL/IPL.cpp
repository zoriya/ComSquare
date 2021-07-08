//
// Created by Melefo on 27/02/2020.
//

#include "IPL.hpp"
#include "Exceptions/InvalidAddress.hpp"
#include <utility>

namespace ComSquare::APU::IPL
{
	IPL::IPL(Component type, std::string iplName)
	    : _iplType(type),
	      _iplName(std::move(iplName))
	{}

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

	uint24_t IPL::getSize() const
	{
		return this->_size;
	}

	std::string IPL::getName() const
	{
		return this->_iplName;
	}

	Component IPL::getComponent() const
	{
		return this->_iplType;
	}

	std::string IPL::getValueName(uint24_t) const
	{
		// TODO implement this
		return "???";
	}

	uint8_t &IPL::operator[](uint24_t addr)
	{
		return this->_data[addr];
	}

	const uint8_t &IPL::operator[](uint24_t addr) const
	{
		return this->_data[addr];
	}
}// namespace ComSquare::APU::IPL