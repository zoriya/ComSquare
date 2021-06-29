//
// Created by Melefo on 19/02/2020.
//

#ifndef COMSQUARE_APUDEBUG_HPP
#define COMSQUARE_APUDEBUG_HPP

#include "../APU/APU.hpp"
#include "../SNES.hpp"
#include "../../ui/ui_apuView.h"

namespace ComSquare::Debugger
{
	//! @brief List of all types of operands used by the instructions
	enum Operand
	{
		None,
		A,
		X,
		Y,
		SP,
		PSW,
		ImmediateData,
		IndexXAddr,
		IndexYAddr,
		AbsoluteBit,
		AbsoluteAddr,
		AbsoluteAddrByX,
		AbsoluteAddrByY,
		AbsoluteByXAddr,
		AbsoluteDirectByXAddr,
		AbsoluteDirectAddrByY,
		DirectAddr,
		DirectAddrByX,
		DirectAddrByY
	};

	//! @brief Small structure to store some values on the instructions
	struct Instruction
	{
		std::string name;
		int size;
		std::tuple<Operand, Operand> operands;
	};

	class APUDebug : public APU::APU, public QObject
	{
	private:
		//! @brief List of instructions and their information
		std::array<Instruction, 0x100> _instructions {{
			{"NOP", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"OR", 2, {DirectAddr, None}},
			{"OR", 3, {AbsoluteAddr, None}},
			{"OR", 1, {IndexXAddr, None}},
			{"OR", 2, {AbsoluteDirectByXAddr, None}},
			{"OR", 2, {ImmediateData, None}},
			{"OR", 3, {DirectAddr, DirectAddr}},
			{"OR1", 3, {AbsoluteBit, None}},
			{"ASL", 2, {DirectAddr, None}},
			{"ASL", 3, {AbsoluteAddr, None}},
			{"PUSH", 1, {PSW, None}},
			{"TSET1", 3, {AbsoluteAddr, None}},
			{"BRK", 1, {None, None}},
			{"BPL", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"OR", 2, {DirectAddrByX, None}},
			{"OR", 3, {AbsoluteAddrByX, None}},
			{"OR", 3, {AbsoluteAddrByY, None}},
			{"OR", 2, {AbsoluteDirectAddrByY, None}},
			{"OR", 3, {DirectAddr, ImmediateData}},
			{"OR", 1, {IndexYAddr, IndexYAddr}},
			{"DECW", 2, {DirectAddr, None}},
			{"ASL", 2, {DirectAddrByX, None}},
			{"ASL", 1, {A, None}},
			{"DEC", 1, {X, None}},
			{"CMP", 3, {X, AbsoluteAddr}},
			{"JMP", 3, {AbsoluteByXAddr, None}},
			{"CLRP", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"AND", 2, {DirectAddr, None}},
			{"AND", 3, {AbsoluteAddr, None}},
			{"AND", 1, {IndexXAddr, None}},
			{"AND", 2, {AbsoluteDirectByXAddr, None}},
			{"AND", 2, {ImmediateData, None}},
			{"AND", 3, {DirectAddr, DirectAddr}},
			{"OR1", 3, {AbsoluteBit, None}},
			{"ROL", 2, {DirectAddr, None}},
			{"ROL", 3, {AbsoluteAddr, None}},
			{"PUSH", 1, {A, None}},
			{"CBNE", 3, {DirectAddrByX, ImmediateData}},
			{"BRA", 2, {ImmediateData, None}},
			{"BMI", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"AND", 2, {DirectAddrByX, None}},
			{"AND", 3, {AbsoluteAddrByX, None}},
			{"AND", 3, {AbsoluteAddrByY, None}},
			{"AND", 2, {AbsoluteDirectAddrByY, None}},
			{"AND", 3, {DirectAddr, ImmediateData}},
			{"AND", 1, {IndexXAddr, IndexYAddr}},
			{"INCW", 2, {DirectAddr, None}},
			{"ROL", 2, {AbsoluteAddrByX, None}},
			{"ROL", 1, {A, None}},
			{"INC", 1, {X, None}},
			{"CMP", 2, {X, DirectAddr}},
			{"CALL", 3, {AbsoluteAddr, None}},
			{"SETP", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"EOR", 2, {DirectAddr, None}},
			{"EOR", 3, {AbsoluteAddr, None}},
			{"EOR", 1, {IndexXAddr, None}},
			{"EOR", 2, {AbsoluteDirectByXAddr, None}},
			{"EOR", 2, {ImmediateData, None}},
			{"EOR", 3, {DirectAddr, DirectAddr}},
			{"AND1", 3, {AbsoluteBit, None}},
			{"LSR", 2, {DirectAddr, None}},
			{"LSR", 3, {AbsoluteAddr, None}},
			{"PUSH", 1, {X, None}},
			{"TCLR1", 3, {AbsoluteAddr, None}},
			{"PCALL", 3, {None, None}},
			{"BVC", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"EOR", 2, {DirectAddrByX, None}},
			{"EOR", 3, {AbsoluteAddrByX, None}},
			{"EOR", 3, {AbsoluteAddrByY, None}},
			{"EOR", 2, {AbsoluteDirectAddrByY, None}},
			{"EOR", 3, {DirectAddr, ImmediateData}},
			{"EOR", 1, {IndexXAddr, IndexYAddr}},
			{"CMPW", 2, {DirectAddr, None}},
			{"LSR", 2, {DirectAddrByX, None}},
			{"LSR", 1, {A, None}},
			{"MOV", 1, {A, X}},
			{"CMP", 3, {Y, AbsoluteAddr}},
			{"JMP", 3, {AbsoluteAddr, None}},
			{"CLRC", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"CMP", 2, {A, DirectAddr}},
			{"CMP", 3, {A, AbsoluteAddr}},
			{"CMP", 1, {A, IndexXAddr,}},
			{"CMP", 2, {A, AbsoluteDirectByXAddr}},
			{"CMP", 2, {A, ImmediateData}},
			{"CMP", 3, {DirectAddr, DirectAddr}},
			{"AND1", 3, {AbsoluteBit, None}},
			{"ROR", 2, {DirectAddr , None}},
			{"ROR", 3, {AbsoluteAddr, None}},
			{"PUSH", 1, {Y, None}},
			{"DBNZ", 3, {ImmediateData, None}},
			{"RET", 1, {None, None}},
			{"BVS", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"CMP", 2, {A, DirectAddrByX}},
			{"CMP", 3, {A, AbsoluteAddrByX}},
			{"CMP", 3, {A, AbsoluteAddrByY}},
			{"CMP", 2, {A, AbsoluteDirectAddrByY}},
			{"CMP", 3, {DirectAddr, ImmediateData}},
			{"CMP", 1, {IndexXAddr, IndexYAddr}},
			{"ADDW", 2, {DirectAddr, None}},
			{"ROR", 2, {DirectAddrByX, None}},
			{"ROR", 1, {A, None}},
			{"MOV", 1, {X, A}},
			{"CMP", 3, {Y, DirectAddr}},
			{"RETI", 1, {None, None}},
			{"SETC", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"ADC", 2, {DirectAddr, None}},
			{"ADC", 3, {AbsoluteAddr, None}},
			{"ADC", 1, {IndexXAddr, None}},
			{"ADC", 2, {AbsoluteDirectByXAddr, None}},
			{"ADC", 2, {ImmediateData, None}},
			{"ADC", 3, {DirectAddr, DirectAddr}},
			{"EOR1", 3, {AbsoluteBit, None}},
			{"DEC", 2, {DirectAddr, None}},
			{"DEC", 3, {AbsoluteAddr, None}},
			{"MOV", 2, {ImmediateData, Y}},
			{"POP", 1, {PSW, None}},
			{"MOV", 3, {DirectAddr, ImmediateData}},
			{"BCC", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"ADC", 2, {DirectAddrByX, None}},
			{"ADC", 3, {AbsoluteAddrByX, None}},
			{"ADC", 3, {AbsoluteAddrByY, None}},
			{"ADC", 2, {AbsoluteDirectAddrByY, None}},
			{"ADC", 3, {DirectAddr, ImmediateData}},
			{"ADC", 1, {IndexXAddr, IndexYAddr}},
			{"SUBW", 2, {DirectAddr, None}},
			{"DEC", 2, {DirectAddrByX, None}},
			{"DEC", 1, {A, None}},
			{"MOV", 1, {SP, X}},
			{"DIV", 1, {None, None}},
			{"XCN", 1, {None, None}},
			{"EI", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"SBC", 2, {DirectAddr, None}},
			{"SBC", 3, {AbsoluteAddr, None}},
			{"SBC", 1, {IndexXAddr, None}},
			{"SBC", 2, {AbsoluteDirectByXAddr, None}},
			{"SBC", 2, {ImmediateData, None}},
			{"SBC", 3, {DirectAddr, DirectAddr}},
			{"MOV1", 3, {AbsoluteBit, None}},
			{"INC", 2, {DirectAddr, None}},
			{"INC", 3, {AbsoluteAddr, None}},
			{"CMP", 2, {Y, ImmediateData}},
			{"POP", 1, {A, None}},
			{"MOV", 1, {A, IndexXAddr}},
			{"BCS", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"SBC", 2, {DirectAddrByX, None}},
			{"SBC", 3, {AbsoluteAddrByX, None}},
			{"SBC", 3, {AbsoluteAddrByY, None}},
			{"SBC", 2, {AbsoluteDirectAddrByY, None}},
			{"SBC", 2, {DirectAddr, ImmediateData}},
			{"SBC", 1, {IndexXAddr, IndexYAddr}},
			{"MOVW", 2, {DirectAddr, None}},
			{"INC", 2, {DirectAddrByX, None}},
			{"INC", 1, {A, None}},
			{"MOV", 1, {X, SP}},
			{"DAS", 1, {None, None}},
			{"MOV", 1, {IndexXAddr, A}},
			{"DI", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"MOV", 2, {A, DirectAddr}},
			{"MOV", 3, {A, AbsoluteAddr}},
			{"MOV", 1, {A, IndexXAddr}},
			{"MOV", 2, {A, AbsoluteDirectByXAddr}},
			{"CMP", 2, {X, ImmediateData}},
			{"MOV", 3, {X, AbsoluteAddr}},
			{"MOV1", 3, {AbsoluteBit, None}},
			{"MOV", 2, {Y, DirectAddr}},
			{"MOV", 3, {Y, AbsoluteAddr}},
			{"MOV", 2, {ImmediateData, X}},
			{"POP", 1, {X, None}},
			{"MUL", 1, {None, None}},
			{"BNE", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"MOV", 2, {A, DirectAddrByX}},
			{"MOV", 3, {A, AbsoluteAddrByX}},
			{"MOV", 3, {A, AbsoluteAddrByY}},
			{"MOV", 2, {A, AbsoluteDirectAddrByY}},
			{"MOV", 2, {X, DirectAddr}},
			{"MOV", 2, {X, DirectAddrByY}},
			{"MOVW", 2, {DirectAddr, None}},
			{"MOV", 2, {Y, DirectAddrByX}},
			{"DEC", 1, {Y, None}},
			{"MOV", 1, {Y, A}},
			{"CBNE", 3, {DirectAddrByX, ImmediateData}},
			{"DAA", 1, {None, None}},
			{"CLRV", 1, {None, None}},
			{"TCALL", 1, {None, None}},
			{"SET1", 2, {DirectAddr, None}},
			{"BBS", 3, {DirectAddr, ImmediateData}},
			{"MOV", 2, {DirectAddr, A}},
			{"MOV", 3, {AbsoluteAddrByX, A}},
			{"MOV", 1, {IndexXAddr, A}},
			{"MOV", 2, {AbsoluteDirectByXAddr, A}},
			{"MOV", 2, {ImmediateData, A}},
			{"MOV", 3, {AbsoluteAddr, X}},
			{"NOT1", 3, {AbsoluteBit, None}},
			{"MOV", 2, {DirectAddr, Y}},
			{"MOV", 3, {AbsoluteAddr, Y}},
			{"NOTC", 1, {None, None}},
			{"POP", 1, {Y, None}},
			{"SLEEP", 1, {None, None}},
			{"BEQ", 2, {ImmediateData, None}},
			{"TCALL", 1, {None, None}},
			{"CLR1", 2, {DirectAddr, None}},
			{"BBC", 3, {DirectAddr, ImmediateData}},
			{"MOV", 2, {DirectAddrByX, A}},
			{"MOV", 3, {AbsoluteAddrByX, A}},
			{"MOV", 3, {AbsoluteAddrByY, A}},
			{"MOV", 2, {AbsoluteDirectAddrByY, A}},
			{"MOV", 2, {DirectAddr, X}},
			{"MOV", 2, {DirectAddrByY, X}},
			{"MOV", 3, {DirectAddr, DirectAddr}},
			{"MOV", 2, {DirectAddrByX, Y}},
			{"INC", 1, {Y, None}},
			{"MOV", 1, {A, Y}},
			{"DBNZ", 3, {ImmediateData, None}},
			{"STOP", 1, {None, None}}
		}};

		//! @brief Position of the last instruction executed
		uint16_t _pc;

		//! @brief Add instruction to disassembly
		int _appendInstruction(int row);

		//! @brief The QT window for this debugger.
		ClosableWindow<APUDebug> *_window;

		//! @brief A widget that contain the whole UI.
		Ui::APUView _ui;

		//! @brief If this is set to true, the execution of the APU will be paused.
		bool _isPaused = true;
		//! @brief If this is set to true, the APU will execute one instruction and pause itself.
		bool _isStepping = false;

		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;

		//! @brief Update the debugger panel values
		void _updatePanel();

		//! @brief Updates the object that serves as the disassembly
		void _updateLogger();

		//! @brief Replace original _executeInstruction to write to the logger.
		int _executeInstruction() override;

		//! @brief Retrieves the instruction from the SP location
		Instruction &_getInstruction();

		//! @brief Returns an operand in text format
		std::string _getOperand(Operand ope);

	public slots:
		//! @brief Pause/Resume the APU.
		void pause();
		//! @brief Step - Execute a single instruction.
		void step();
		//! @brief Called when the window is closed. Turn off the debugger and revert to a basic APU.
		void disableDebugger();
	public:
		//! @brief Convert a basic APU to a debugging APU.
		explicit APUDebug(ComSquare::APU::APU &apu, SNES &snes);
		APUDebug(const APUDebug &) = delete;
		APUDebug &operator=(const APUDebug &) = delete;
		~APUDebug() override = default;

		//! @brief Override the apu's update to disable debugging.
		void update(unsigned cycles) override;

		//! @brief Return true if the CPU is overloaded with debugging features.
		bool isDebugger() const override;

		//! @brief Focus the debugger's window.
		void focus();
	};
}

#endif //COMSQUARE_APUDEBUG_HPP