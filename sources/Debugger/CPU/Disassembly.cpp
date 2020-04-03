//
// Created by anonymus-raccoon on 4/3/20.
//

#include <sstream>
#include "CPUDebug.hpp"
#include "../../Utility/Utility.hpp"

using namespace ComSquare::CPU;

namespace ComSquare::Debugger
{
	DisassembledInstruction::DisassembledInstruction(const CPU::Instruction &instruction, uint24_t addr, std::string arg, uint8_t op)
		: CPU::Instruction(instruction), address(addr), argument(std::move(arg)), opcode(op), level(Safe) {}

	std::string DisassembledInstruction::toString()
	{
		return this->name + " " + this->argument;
	}

	std::vector<DisassembledInstruction> CPUDebug::_disassemble(uint24_t pc, uint24_t length, DisassemblyContext &ctx)
	{
		std::vector<DisassembledInstruction> map;
		uint24_t endAddr = pc + length;

		while (pc < endAddr) {
			DisassembledInstruction instruction = this->_parseInstruction(pc, ctx);
			instruction.level = ctx.level;
			map.push_back(instruction);
			pc += instruction.size;
			if (instruction.addressingMode == ImmediateForA && !ctx.mFlag)
				pc++;
			if (instruction.addressingMode == ImmediateForX && !ctx.xFlag)
				pc++;

			if (instruction.opcode == 0x40 && ctx.isEmulationMode) { // RTI
				ctx.mFlag = true;
				ctx.xFlag = true;
			}
			if (instruction.opcode == 0xC2) { // REP
				if (ctx.isEmulationMode) {
					ctx.mFlag = true;
					ctx.xFlag = true;
				} else {
					uint8_t m = this->_bus->read(pc - 1);
					ctx.mFlag &= ~m & 0b00100000u;
					ctx.xFlag &= ~m & 0b00010000u;
				}
			}
			if (instruction.opcode == 0xE2) { // SEP
				uint8_t m = this->_bus->read(pc - 1);
				ctx.mFlag |= m & 0b00100000u;
				ctx.xFlag |= m & 0b00010000u;
			}
			if (instruction.opcode == 0x28) { // PLP
				if (ctx.isEmulationMode) {
					ctx.mFlag = true;
					ctx.xFlag = true;
				} else
					ctx.level = Compromised;
			}
			if (instruction.opcode == 0xFB) {// XCE
				ctx.level = Unsafe;
				ctx.isEmulationMode = false; // The most common use of the XCE is to enable native mode at the start of the ROM so we guess that it has done that.
			}
		}
		return map;
	}

	DisassembledInstruction CPUDebug::_parseInstruction(uint24_t pc, DisassemblyContext &ctx)
	{
		uint24_t opcode = this->_bus->read(pc, true);
		Instruction instruction = this->_instructions[opcode];
		std::string argument = this->_getInstructionParameter(instruction, pc + 1, ctx);
		return DisassembledInstruction(instruction, pc, argument, opcode);
	}

	std::string CPUDebug::_getInstructionParameter(Instruction &instruction, uint24_t pc, DisassemblyContext &ctx)
	{
		switch (instruction.addressingMode) {
		case Implied:
			return "";

		case Immediate8bits:
			return this->_getImmediateValue(pc, false);
		case ImmediateForA:
			return this->_getImmediateValue(pc, !ctx.mFlag);
		case ImmediateForX:
			return this->_getImmediateValue(pc, !ctx.xFlag);

		case Absolute:
			return this->_getAbsoluteValue(pc);
		case AbsoluteLong:
			return this->_getAbsoluteLongValue(pc);

		case DirectPage:
			return this->_getDirectValue(pc);
		case DirectPageIndexedByX:
			return this->_getDirectIndexedByXValue(pc);

		case DirectPageIndirect:
			return this->_getDirectIndirectValue(pc);
		case DirectPageIndirectLong:
			return this->_getDirectIndirectLongValue(pc);
		case AbsoluteIndexedByX:
			return this->_getAbsoluteIndexByXValue(pc);
		case AbsoluteIndexedByXLong:
			return this->_getAbsoluteIndexByXLongValue(pc);
		case AbsoluteIndexedByY:
			return this->_getAbsoluteIndexByYValue(pc);
		case DirectPageIndexedByY:
			return this->_getDirectIndexedByYValue(pc);
		case DirectPageIndirectIndexedByX:
			return this->_getDirectIndexedByXIndirectValue(pc);
		case DirectPageIndirectIndexedByY:
			return this->_getDirectIndirectIndexedByYValue(pc);
		case DirectPageIndirectIndexedByYLong:
			return this->_getDirectIndirectIndexedByYLongValue(pc);
		case StackRelative:
			return this->_getStackRelativeValue(pc);
		case StackRelativeIndirectIndexedByY:
			return this->_getStackRelativeIndiretIndexdeByYValue(pc);
		case AbsoluteIndirect:
			return this->_getAbsoluteIndirectValue(pc);
		case AbsoluteIndirectIndexedByX:
			return this->_getAbsoluteIndirectIndexedByXValue(pc);
		case AbsoluteIndirectLong:
			return this->_getAbsoluteIndirectLongValue(pc);

		default:
			return "???";
		}
	}

	std::string CPUDebug::_getImmediateValue(uint24_t pc, bool dual)
	{
		unsigned value = this->_bus->read(pc, true);

		if (dual)
			value += this->_bus->read(pc + 1, true) << 8u;
		std::stringstream ss;
		ss << "#$" << std::hex << value;
		return ss.str();
	}

	std::string CPUDebug::_getDirectValue(uint24_t pc)
	{
		return Utility::to_hex(this->_bus->read(pc, true), Utility::HexString::AsmPrefix);
	}

	std::string CPUDebug::_getAbsoluteValue(uint24_t pc)
	{
		uint24_t value = this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u);
		return Utility::to_hex(value, Utility::HexString::AsmPrefix);
	}

	std::string CPUDebug::_getAbsoluteLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc++, true);
		value += this->_bus->read(pc++, true) << 8u;
		value += this->_bus->read(pc, true) << 16u;

		return Utility::to_hex(value, Utility::HexString::AsmPrefix);
	}

	std::string CPUDebug::_getDirectIndexedByXValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "$" << std::hex << value << ", x";
		return ss.str();
	}

	std::string CPUDebug::_getDirectIndexedByYValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "$" << std::hex << value << ", y";
		return ss.str();
	}

	std::string CPUDebug::_getDirectIndirectValue(uint24_t pc)
	{
		return "(" + Utility::to_hex(this->_bus->read(pc, true), Utility::AsmPrefix) + ")";
	}

	std::string CPUDebug::_getDirectIndirectLongValue(uint24_t pc)
	{
		return "[" + Utility::to_hex(this->_bus->read(pc, true), Utility::AsmPrefix) + "]";
	}

	std::string CPUDebug::_getAbsoluteIndexByXValue(uint24_t pc)
	{
		uint24_t value = this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u);
		return Utility::to_hex(value, Utility::HexString::AsmPrefix) + ", x";
	}

	std::string CPUDebug::_getAbsoluteIndexByYValue(uint24_t pc)
	{
		uint24_t value = this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u);
		return Utility::to_hex(value, Utility::HexString::AsmPrefix) + ", y";
	}

	std::string CPUDebug::_getAbsoluteIndexByXLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc++, true);
		value += this->_bus->read(pc++, true) << 8u;
		value += this->_bus->read(pc, true) << 16u;

		return Utility::to_hex(value, Utility::HexString::AsmPrefix) + ", x";
	}

	std::string CPUDebug::_getDirectIndexedByXIndirectValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "($" << std::hex << value << ", x)";
		return ss.str();
	}

	std::string CPUDebug::_getDirectIndirectIndexedByYValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "($" << std::hex << value << "), y";
		return ss.str();
	}

	std::string CPUDebug::_getDirectIndirectIndexedByYLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc, true);

		std::stringstream ss;
		ss << "[$" << std::hex << value << "], y";
		return ss.str();
	}

	std::string CPUDebug::_getStackRelativeValue(uint24_t pc)
	{
		return Utility::to_hex(this->_bus->read(pc, true), Utility::AsmPrefix) + ", s";
	}

	std::string CPUDebug::_getStackRelativeIndiretIndexdeByYValue(uint24_t pc)
	{
		return "(" + Utility::to_hex(this->_bus->read(pc, true), Utility::AsmPrefix) + ", s), y";
	}

	std::string CPUDebug::_getAbsoluteIndirectValue(uint24_t pc)
	{
		uint24_t value = this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u);
		return "(" + Utility::to_hex(value, Utility::HexString::AsmPrefix) + ")";
	}

	std::string CPUDebug::_getAbsoluteIndirectLongValue(uint24_t pc)
	{
		unsigned value = this->_bus->read(pc++, true);
		value += this->_bus->read(pc++, true) << 8u;
		value += this->_bus->read(pc, true) << 16u;

		return "(" + Utility::to_hex(value, Utility::HexString::AsmPrefix) + ")";
	}

	std::string CPUDebug::_getAbsoluteIndirectIndexedByXValue(uint24_t pc)
	{
		uint24_t value = this->_bus->read(pc) + (this->_bus->read(pc + 1, true) << 8u);
		return "(" + Utility::to_hex(value, Utility::HexString::AsmPrefix) + ", x)";
	}
}