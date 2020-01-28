//
// Created by anonymus-raccoon on 1/28/20.
//

#include "Ram.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	Ram::Ram(size_t size)
		: _size(size)
	{
		this->_data = new uint8_t[size];
	}

	Ram::~Ram()
	{
		delete[] this->_data;
	}

	uint8_t Ram::read(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Ram read", addr);
		return this->_data[addr];
	}

	void Ram::write(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Ram write", addr);
		this->_data[addr] = data;
	}
}
