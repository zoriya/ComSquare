//
// Created by anonymus-raccoon on 1/24/20.
//

#include "CPU.hpp"

#include <utility>
#include "../Exceptions/NotImplementedException.hpp"
#include "../Exceptions/InvalidAddress.hpp"
#include "../Exceptions/InvalidOpcode.hpp"

namespace ComSquare::CPU
{
	CPU::CPU(std::shared_ptr<Memory::MemoryBus> bus, Cartridge::Header &cartridgeHeader)
		: _bus(std::move(bus)), _cartridgeHeader(cartridgeHeader)
	{
		this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.reset;
	}

	//! @bref The CPU's internal registers starts at $4200	and finish at $421F.
	uint8_t CPU::read(uint24_t addr)
	{
		switch (addr) {
		case 0x0:
			return this->_internalRegisters.nmitimen;
		case 0x1:
			return this->_internalRegisters.wrio;
		case 0x2:
			return this->_internalRegisters.wrmpya;
		case 0x3:
			return this->_internalRegisters.wrmpyb;
		case 0x4:
			return this->_internalRegisters.wrdivl;
		case 0x5:
			return this->_internalRegisters.wrdivh;
		case 0x6:
			return this->_internalRegisters.wrdivb;
		case 0x7:
			return this->_internalRegisters.htimel;
		case 0x8:
			return this->_internalRegisters.htimeh;
		case 0x9:
			return this->_internalRegisters.vtimel;
		case 0xA:
			return this->_internalRegisters.vtimeh;
		case 0xB:
			return this->_internalRegisters.mdmaen;
		case 0xC:
			return this->_internalRegisters.hdmaen;
		case 0xD:
			return this->_internalRegisters.memsel;
		case 0x10:
			return this->_internalRegisters.rdnmi;
		case 0x11:
			return this->_internalRegisters.timeup;
		case 0x12:
			return this->_internalRegisters.hvbjoy;
		case 0x13:
			return this->_internalRegisters.rdio;
		case 0x14:
			return this->_internalRegisters.rddivl;
		case 0x15:
			return this->_internalRegisters.rddivh;
		case 0x16:
			return this->_internalRegisters.rdmpyl;
		case 0x17:
			return this->_internalRegisters.rdmpyh;
		case 0x18:
			return this->_internalRegisters.joy1l;
		case 0x19:
			return this->_internalRegisters.joy1h;
		case 0x1A:
			return this->_internalRegisters.joy2l;
		case 0x1B:
			return this->_internalRegisters.joy2h;
		case 0x1C:
			return this->_internalRegisters.joy3l;
		case 0x1D:
			return this->_internalRegisters.joy3h;
		case 0x1E:
			return this->_internalRegisters.joy4l;
		case 0x1F:
			return this->_internalRegisters.joy4h;
		default:
			throw InvalidAddress("CPU Internal Registers read", addr);
		}
	}

	void CPU::write(uint24_t addr, uint8_t data)
	{
		switch (addr) {
		case 0x0:
			this->_internalRegisters.nmitimen = data;
			break;
		case 0x1:
			this->_internalRegisters.wrio = data;
			break;
		case 0x2:
			this->_internalRegisters.wrmpya = data;
			break;
		case 0x3:
			this->_internalRegisters.wrmpyb = data;
			break;
		case 0x4:
			this->_internalRegisters.wrdivl = data;
			break;
		case 0x5:
			this->_internalRegisters.wrdivh = data;
			break;
		case 0x6:
			this->_internalRegisters.wrdivb = data;
			break;
		case 0x7:
			this->_internalRegisters.htimel = data;
			break;
		case 0x8:
			this->_internalRegisters.htimeh = data;
			break;
		case 0x9:
			this->_internalRegisters.vtimel = data;
			break;
		case 0xA:
			this->_internalRegisters.vtimeh = data;
			break;
		case 0xB:
			this->_internalRegisters.mdmaen = data;
			break;
		case 0xC:
			this->_internalRegisters.hdmaen = data;
			break;
		case 0xD:
			this->_internalRegisters.memsel = data;
			break;
		case 0x10:
			this->_internalRegisters.rdnmi = data;
			break;
		case 0x11:
			this->_internalRegisters.timeup = data;
			break;
		case 0x12:
			this->_internalRegisters.hvbjoy = data;
			break;
		case 0x13:
			this->_internalRegisters.rdio = data;
			break;
		case 0x14:
			this->_internalRegisters.rddivl = data;
			break;
		case 0x15:
			this->_internalRegisters.rddivh = data;
			break;
		case 0x16:
			this->_internalRegisters.rdmpyl = data;
			break;
		case 0x17:
			this->_internalRegisters.rdmpyh = data;
			break;
		case 0x18:
			this->_internalRegisters.joy1l = data;
			break;
		case 0x19:
			this->_internalRegisters.joy1h = data;
			break;
		case 0x1A:
			this->_internalRegisters.joy2l = data;
			break;
		case 0x1B:
			this->_internalRegisters.joy2h = data;
			break;
		case 0x1C:
			this->_internalRegisters.joy3l = data;
			break;
		case 0x1D:
			this->_internalRegisters.joy3h = data;
			break;
		case 0x1E:
			this->_internalRegisters.joy4l = data;
			break;
		case 0x1F:
			this->_internalRegisters.joy4h = data;
			break;
		default:
			throw InvalidAddress("CPU Internal Registers write", addr);
		}
	}

	int CPU::update()
	{
		int cycles = 0;

		for (int i = 0; i < 0xFF; i++)
			cycles += this->executeInstruction();
		return cycles;
	}

	int CPU::executeInstruction()
	{
		uint8_t opcode = this->_bus->read(this->_registers.pc);

		switch (opcode) {
		case 0x0:
			return this->BRK();
		case 0x61:
		case 0x63:
		case 0x65:
		case 0x67:
		case 0x69:
		case 0x6D:
		case 0x6F:
		case 0x71:
		case 0x72:
		case 0x73:
		case 0x75:
		case 0x77:
		case 0x79:
		case 0x7D:
		case 0x7F:
			return this->ADC();
		default:
			throw InvalidOpcode("CPU", opcode);
		}
	}

	////////////////////////////////////////////////////////////////////
	/// Addressing modes
	////////////////////////////////////////////////////////////////////

	uint24_t CPU::_GetImmediateAddr()
	{
		return this->_registers.pc++;
	}

	uint24_t CPU::_GetDirectAddr()
	{
		uint8_t addr = this->_bus->read(this->_registers.pc++);
		return this->_registers.d + addr;
	}

	uint24_t CPU::_GetAbsoluteAddr()
	{
		uint24_t addr = this->_registers.dbr << 16u;
		addr += this->_bus->read(this->_registers.pc++) << 8u;
		addr += this->_bus->read(this->_registers.pc++);
		return addr;
	}

	uint24_t CPU::_GetAbsoluteLongAddr()
	{
		return 0;
	}



	int CPU::BRK()
	{
		this->_registers.pc += 2;

		this->_registers.p.i = true;
		if (this->_isEmulationMode)
			this->_registers.pc = this->_cartridgeHeader.emulationInterrupts.brk;
		else
			this->_registers.pc = this->_cartridgeHeader.nativeInterrupts.brk;
		this->_registers.p.d = false;
		return 7 + !this->_isEmulationMode;
	}


	////////////////////////////////////////////////////////////////////
	/// Mathematical operations
	////////////////////////////////////////////////////////////////////

	int CPU::ADC()
	{
//		this->_registers.a +=
	}
}