//
// Created by anonymus-raccoon on 1/28/20.
//

#include <cstring>
#include <utility>
#include "Ram.hpp"
#include "Exceptions/InvalidAddress.hpp"

namespace ComSquare::Ram
{
	Ram::Ram(size_t size, Component type, std::string ramName)
		: _data(size),
		  _ramType(type),
		  _ramName(std::move(ramName))
	{ }

	uint8_t &Ram::operator[](uint24_t addr)
	{
		return this->_data[addr];
	}

	const uint8_t &Ram::operator[](uint24_t addr) const
	{
		return this->_data[addr];
	}

	uint8_t Ram::read(uint24_t addr)
	{
		// TODO read/write after the size of the rom should noop or behave like a mirror. I don't really know.
		if (addr >= this->_data.size())
			throw InvalidAddress(this->getName() + " read", addr);
		return this->_data[addr];
	}

	void Ram::write(uint24_t addr, uint8_t data)
	{
		if (addr >= this->_data.size())
			throw InvalidAddress(this->getName() + " write", addr);
		this->_data[addr] = data;
	}

	uint24_t Ram::getSize() const
	{
		return this->_data.size();
	}

	void Ram::setSize(uint24_t size)
	{
		this->_data.resize(size);
	}

	std::string Ram::getName() const
	{
		return this->_ramName;
	}

	Component Ram::getComponent() const
	{
		return this->_ramType;
	}

	std::span<uint8_t> Ram::getData()
	{
		return std::span(this->_data);
	}

	std::span<const uint8_t> Ram::getData() const
	{
		return std::span(this->_data);
	}
}
