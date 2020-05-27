//
// Created by anonymus-raccoon on 5/26/20.
//

#include "DMA.hpp"
#include "../../Exceptions/InvalidAddress.hpp"

namespace ComSquare::CPU
{
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

	uint8_t DMA::run(unsigned int cycles)
	{
		return 0;
	}
}