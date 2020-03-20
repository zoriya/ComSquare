//
// Created by anonymus-raccoon on 2/28/20.
//

#include "../CPU.hpp"

namespace ComSquare::CPU
{
	void CPU::TAX()
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
	}

	void CPU::TAY()
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
	}

	void CPU::TXS()
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
	}
}