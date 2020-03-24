//
// Created by anonymus-raccoon on 1/28/20.
//

#include <cstring>
#include <utility>
#include "Ram.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	Ram::Ram(size_t size, std::string ramName)
		: _size(size),
		_ramName(std::move(ramName))
	{
		if (size == 0)
			this->_data = nullptr;
		else {
			this->_data = new uint8_t[size];
			std::memset(this->_data, 0, size * sizeof(uint8_t));
		}
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

	size_t Ram::getSize()
	{
		return this->_size;
	}

	std::string Ram::getName()
	{
		return this->_ramName;
	}
}
