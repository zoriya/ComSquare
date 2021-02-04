//
// Created by anonymus-raccoon on 1/28/20.
//

#include <cstring>
#include <utility>
#include "Ram.hpp"
#include "../Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	Ram::Ram(size_t size, Component type, std::string ramName)
		: _size(size),
		_ramType(type),
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

	uint8_t Ram::read(uint24_t addr) const
	{
		// TODO read/write after the size of the rom should noop or behave like a mirror. I don't really know.
		if (addr >= this->_size)
			throw InvalidAddress(this->getName() + " read", addr);
		return this->_data[addr];
	}

	void Ram::write(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_size)
			throw InvalidAddress(this->getName() + " write", addr);
		this->_data[addr] = data;
	}

	uint24_t Ram::getSize() const
	{
		return this->_size;
	}

	std::string Ram::getName() const
	{
		return this->_ramName;
	}

	Component Ram::getComponent() const
	{
		return this->_ramType;
	}
}
