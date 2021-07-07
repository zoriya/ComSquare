//
// Created by anonymus-raccoon on 5/26/20.
//

#include "DMA.hpp"
#include "Exceptions/InvalidAddress.hpp"

namespace ComSquare::CPU
{
	DMA::DMA(Memory::IMemoryBus &bus)
	    : _bus(bus),
	      enabled(false)
	{}

	void DMA::setBus(Memory::IMemoryBus &bus)
	{
		this->_bus = bus;
	}

	uint8_t DMA::read(uint8_t addr) const
	{
		switch (addr) {
		case 0x0:
			return this->_controlRegister.raw;
		case 0x1:
			return this->_port;
		case 0x2:
			return this->_aAddress.bytes[0];
		case 0x3:
			return this->_aAddress.bytes[1];
		case 0x4:
			return this->_aAddress.bytes[2];
		case 0x5:
			return this->_count.bytes[0];
		case 0x6:
			return this->_count.bytes[1];
		default:
			throw InvalidAddress("DMA read", addr);
		}
	}

	void DMA::write(uint8_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0:
			this->_controlRegister.raw = data;
			break;
		case 0x1:
			this->_port = data;
			break;
		case 0x2:
			this->_aAddress.bytes[0] = data;
			break;
		case 0x3:
			this->_aAddress.bytes[1] = data;
			break;
		case 0x4:
			this->_aAddress.bytes[2] = data;
			break;
		case 0x5:
			this->_count.bytes[0] = data;
			break;
		case 0x6:
			this->_count.bytes[1] = data;
			break;
		default:
			throw InvalidAddress("DMA read", addr);
		}
	}

	unsigned DMA::_writeOneByte(uint24_t aAddress, uint24_t bAddress)
	{
		// Address $2180 refers to the WRam data register.
		// Write to/Read from this port when the a address is on the vram cause different behaviors.
		if (this->_port == 0x80) {
			auto accessor = this->getBus().getAccessor(aAddress);
			if (accessor && accessor->getComponent() == WRam) {
				// WRAM->$2180 The write is not performed but the time is consumed anyway.
				if (this->_controlRegister.direction == AtoB)
					return 8;
				// $2180->WRAM No read is performed (so only 4 master cycles are needed) but the value written is invalid.
				this->getBus().write(aAddress, 0xFF);
				return 4;
			}
		}
		if (this->_controlRegister.direction == AtoB) {
			uint8_t data = this->getBus().read(aAddress);
			this->getBus().write(bAddress, data);
		} else {
			uint8_t data = this->getBus().read(bAddress);
			this->getBus().write(aAddress, data);
		}
		return 8;
	}

	unsigned DMA::run(unsigned int maxCycles)
	{
		unsigned cycles = 8;
		int i = 0;

		do {
			cycles += this->_writeOneByte(this->_aAddress.raw, 0x2100 | (this->_port + this->_getModeOffset(i)));
			if (!this->_controlRegister.fixed)
				this->_aAddress.page += this->_controlRegister.increment ? -1 : 1;
			this->_count.raw--;
			i++;
		} while (this->_count.raw > 0 && this->enabled);
		this->enabled = false;
		return cycles;
	}

	int DMA::_getModeOffset(int index) const
	{
		switch (this->_controlRegister.mode) {
		case OneToOne:
		case TwoToOne:
		case TwoToOneBis:
			return 0;

		case TwoToTwo:
		case TwoToTwoBis:
			return index % 2;

		case FourToTwo:
		case FourToTwoBis:
			return (index & 0b11) > 1;

		case FourToFour:
			return (index & 0b11);
		}
		return 0;
	}
}// namespace ComSquare::CPU