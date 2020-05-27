//
// Created by anonymus-raccoon on 5/26/20.
//

#include "DMA.hpp"
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::CPU
{
	DMA::DMA(std::shared_ptr<Memory::MemoryBus> bus) : _bus(std::move(bus)) {}

	void DMA::setBus(std::shared_ptr<Memory::MemoryBus> bus)
	{
		this->_bus = std::move(bus);
	}

	uint8_t DMA::read(uint8_t addr)
	{
		switch (addr) {
		case 0x0:
			return this->controlRegister.raw;
		case 0x1:
			return this->port;
		case 0x2:
			return this->aAddress.bytes[0];
		case 0x3:
			return this->aAddress.bytes[1];
		case 0x4:
			return this->aAddress.bytes[2];
		case 0x5:
			return this->count.bytes[0];
		case 0x6:
			return this->count.bytes[1];
		default:
			throw InvalidAddress("DMA read", addr);
		}
	}

	void DMA::write(uint8_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0:
			this->controlRegister.raw = data;
			break;
		case 0x1:
			this->port = data;
			break;
		case 0x2:
			this->aAddress.bytes[0] = data;
			break;
		case 0x3:
			this->aAddress.bytes[1] = data;
			break;
		case 0x4:
			this->aAddress.bytes[2] = data;
			break;
		case 0x5:
			this->count.bytes[0] = data;
			break;
		case 0x6:
			this->count.bytes[1] = data;
			break;
		default:
			throw InvalidAddress("DMA read", addr);
		}
	}

	unsigned DMA::_writeOneByte(uint24_t aAddress, uint24_t bAddress)
	{
		// Address $2180 refers to the WRam data register. Write to/Read from this port when the a address is on the vram cause different behaviors.
		if (this->port == 0x80) {
			auto accessor = this->_bus->getAccessor(aAddress);
			if (accessor && accessor->getComponent() == WRam) {
				if (this->controlRegister.direction == AToB)
					return 8;
				this->_bus->write(aAddress, 0xFF);
				return 4;
			}
		}
		if (this->controlRegister.direction == AToB) {
			uint8_t data = this->_bus->read(aAddress);
			this->_bus->write(bAddress, data);
		} else {
			uint8_t data = this->_bus->read(bAddress);
			this->_bus->write(aAddress, data);
		}
		return 8;
	}

	uint8_t DMA::run(unsigned int maxCycles)
	{
		unsigned cycles = 8;
		int i = 0;

		do {
			cycles += this->_writeOneByte(this->aAddress.raw, 0x2100 | this->port  + this->getModeOffset(i));
			if (!this->controlRegister.fixed)
				this->aAddress.page += this->controlRegister.increment ? -1 : 1;
			this->count.raw--;
			i++;
		} while (this->count.raw > 0 && cycles < maxCycles);
		this->enabled = false;
		return cycles;
	}

	int DMA::getModeOffset(int index)
	{
		switch (this->controlRegister.mode) {
		case OneToOne:
			return 0;
		case TwoToTwo:
			return index % 2;
		case TwoToOne:
			return 0;
		case FourToTwo:
			return (index & 0b11) > 1;
		case FourToFour:
			return (index & 0b11);
		}
		return 0;
	}
}