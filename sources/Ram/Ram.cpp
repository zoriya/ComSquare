//
// Created by anonymus-raccoon on 1/28/20.
//

#include <cstring>
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

	uint8_t Ram::read_internal(uint24_t addr)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Ram read", addr);
		return this->_data[addr];
	}

	void Ram::write_internal(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress("Ram write", addr);
		this->_data[addr] = data;
	}

	void Ram::memset(uint24_t start, uint24_t end, uint8_t value)
	{
		if (end >= this->_size)
			throw InvalidAddress("Ram memset end", end);
		if (start >= end)
			throw InvalidAddress("Ram memset start", start);
		std::memset(&this->_data[start], value, sizeof(uint8_t) * (end - start));
	}
}
