//
// Created by anonymus-raccoon on 2/13/20.
//

#include <cstring>
#include "ExtendedRam.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	ExtendedRam::ExtendedRam(size_t size)
		: _size(size)
	{
		this->_data = new uint16_t[size];
		std::memset(this->_data, 0, size * sizeof(uint16_t));
	}

	ExtendedRam::~ExtendedRam()
	{
		delete [] this->_data;
	}

	uint16_t ExtendedRam::read(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("ExtendedRam Read", addr);
		return this->_data[addr];
	}

	void ExtendedRam::write(uint24_t addr, uint16_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress("ExtendedRam Write", addr);
		this->_data[addr] = data;
	}
}