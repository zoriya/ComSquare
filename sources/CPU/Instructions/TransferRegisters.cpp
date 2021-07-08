//
// Created by anonymus-raccoon on 2/28/20.
//

#include <iostream>
#include "CPU/CPU.hpp"

namespace ComSquare::CPU
{
	int CPU::TAX(uint24_t, AddressingMode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.xl = this->_registers.al;
			this->_registers.p.z = this->_registers.xl == 0;
			this->_registers.p.n = this->_registers.x & 0x80u;
		} else {
			this->_registers.x = this->_registers.a;
			this->_registers.p.z = this->_registers.x == 0;
			this->_registers.p.n = this->_registers.x & 0x8000u;
		}
		return 0;
	}

	int CPU::TAY(uint24_t, AddressingMode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.yl = this->_registers.al;
			this->_registers.p.z = this->_registers.yl == 0;
			this->_registers.p.n = this->_registers.y & 0x80u;
		} else {
			this->_registers.y = this->_registers.a;
			this->_registers.p.z = this->_registers.y == 0;
			this->_registers.p.n = this->_registers.y & 0x8000u;
		}
		return 0;
	}

	int CPU::TXS(uint24_t, AddressingMode)
	{
		if (this->_registers.p.x_b) {
			this->_registers.sh = 0;
			this->_registers.sl = this->_registers.xl;
			this->_registers.p.z = this->_registers.sl == 0;
			this->_registers.p.n = this->_registers.s & 0x80u;
		} else {
			this->_registers.s = this->_registers.x;
			this->_registers.p.z = this->_registers.s == 0;
			this->_registers.p.n = this->_registers.s & 0x8000u;
		}
		return 0;
	}

	int CPU::TCD(uint24_t, AddressingMode)
	{
		this->_registers.d = this->_registers.a;
		this->_registers.p.n = this->_registers.d & 0x8000u;
		this->_registers.p.z = this->_registers.d == 0;
		return 0;
	}

	int CPU::TCS(uint24_t, AddressingMode)
	{
		this->_registers.s = this->_registers.a;
		if (this->_isEmulationMode)
			this->_registers.sh = 1;
		return 0;
	}

	int CPU::TDC(uint24_t, AddressingMode)
	{
		this->_registers.a = this->_registers.d;
		this->_registers.p.n = this->_registers.a & 0x8000u;
		this->_registers.p.z = this->_registers.a == 0;
		return 0;
	}

	int CPU::TSC(uint24_t, AddressingMode)
	{
		this->_registers.a = this->_registers.s;
		this->_registers.p.n = this->_registers.a & 0x8000u;
		this->_registers.p.z = this->_registers.a == 0;
		return 0;
	}

	int CPU::TSX(uint24_t, AddressingMode)
	{
		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;

		this->_registers.x = this->_registers.s;
		if (this->_registers.p.x_b)
			this->_registers.xh = 0;
		this->_registers.p.n = this->_registers.x & negativeFlag;
		this->_registers.p.z = this->_registers.x == 0;
		return 0;
	}

	int CPU::TXA(uint24_t, AddressingMode)
	{
		unsigned negativeFlag = this->_registers.p.m ? 0x80u : 0x8000u;

		if (this->_registers.p.m)
			this->_registers.al = this->_registers.xl;
		else {
			this->_registers.a = this->_registers.x;
			if (this->_registers.p.x_b)
				this->_registers.ah = 0;
		}
		this->_registers.p.n = this->_registers.a & negativeFlag;
		this->_registers.p.z = this->_registers.a == 0;
		return 0;
	}

	int CPU::TYA(uint24_t, AddressingMode)
	{
		unsigned negativeFlag = this->_registers.p.m ? 0x80u : 0x8000u;

		if (this->_registers.p.m)
			this->_registers.al = this->_registers.yl;
		else {
			this->_registers.a = this->_registers.y;
			if (this->_registers.p.x_b)
				this->_registers.ah = 0;
		}
		this->_registers.p.n = this->_registers.a & negativeFlag;
		this->_registers.p.z = this->_registers.a == 0;
		return 0;
	}

	int CPU::TXY(uint24_t, AddressingMode)
	{
		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;

		if (this->_registers.p.x_b)
			this->_registers.yl = this->_registers.xl;
		else
			this->_registers.y = this->_registers.x;
		this->_registers.p.n = this->_registers.y & negativeFlag;
		this->_registers.p.z = this->_registers.y == 0;
		return 0;
	}

	int CPU::TYX(uint24_t, AddressingMode)
	{
		unsigned negativeFlag = this->_registers.p.x_b ? 0x80u : 0x8000u;

		if (this->_registers.p.x_b)
			this->_registers.xl = this->_registers.yl;
		else
			this->_registers.x = this->_registers.y;
		this->_registers.p.n = this->_registers.y & negativeFlag;
		this->_registers.p.z = this->_registers.y == 0;
		return 0;
	}

	int CPU::MVN(uint24_t params, AddressingMode)
	{
		uint8_t srcBank = params;
		uint8_t destBank = params >> 8u;
		int length = this->_registers.a + 1;

		this->_registers.dbr = destBank;
		while (this->_registers.a != 0xFFFF) {
			uint8_t data = this->getBus().read(srcBank << 24u  | this->_registers.x);
			this->getBus().write(destBank << 24u | this->_registers.y, data);
			this->_registers.x++;
			this->_registers.y++;
			this->_registers.a--;
		}
		return 7 * length;
	}

	int CPU::MVP(uint24_t params, AddressingMode)
	{
		uint8_t srcBank = params;
		uint8_t destBank = params >> 8u;
		int length = this->_registers.a + 1;

		this->_registers.dbr = destBank;
		while (this->_registers.a != 0xFFFF) {
			uint8_t data = this->getBus().read(srcBank << 24u  | this->_registers.x);
			this->getBus().write(destBank << 24u | this->_registers.y, data);
			this->_registers.x--;
			this->_registers.y--;
			this->_registers.a--;
		}
		return 7 * length;
	}

	std::string CPU::getValueName(uint24_t) const
	{
		// TODO implement this method
		return "???";
	}
}