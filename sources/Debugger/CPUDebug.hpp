//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef COMSQUARE_CPUDEBUG_HPP
#define COMSQUARE_CPUDEBUG_HPP

#include "../CPU/CPU.hpp"
#include "../Renderer/SFRenderer.hpp"
#include "../SNES.hpp"
#include "../../ui/ui_cpu.h"

namespace ComSquare::Debugger
{
	//! @brief A custom CPU with a window that show it's registers and the disassembly.
	class CPUDebug : public CPU::CPU, public QMainWindow {
	private:
		//! @brief A widget that contain the whole UI.
		Ui::CPUView _ui;
		//! @brief If this is set to true, the execution of the CPU will be paused.
		bool _isPaused = true;
		//! @brief If this is set to true, the CPU will execute one instruction and pause itself.
		bool _isStepping = false;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief Reimplement the basic instruction execution method to log instructions inside the logger view.
		unsigned _executeInstruction(uint8_t opcode) override;
		//! @brief Get a printable string representing an instruction at the program counter given as parameter.
		std::string _getInstructionString(uint24_t pc);
		//! @brief Get a printable string representing the flags.
		std::string _getFlagsString();
		//! @brief Update the register's panel (accumulator, stack pointer...)
		void _updateRegistersPanel();

		//! @brief Return a printable string corresponding to the value of an immediate addressing mode for a.
		std::string _getImmediateValueForA(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an immediate addressing mode for x.
		std::string _getImmediateValueForX(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a 8bits immediate addressing mode (used only with SEP and REP).
		std::string _getImmediateValue8Bits(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of a direct addressing mode.
		std::string _getDirectValue(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an absolute addressing mode.
		std::string _getAbsoluteValue(uint24_t pc);
		//! @brief Return a printable string corresponding to the value of an absolute long addressing mode.
		std::string _getAbsoluteLongValue(uint24_t pc);

	public slots:
		//! @brief Pause/Resume the CPU.
		void pause();
		//! @brief Step - Execute a single instruction.
		void step();
		//! @brief Clear the history panel.
		void clearHistory();
	public:
		//! @brief Update the UI when reseting the CPU.
		void RESB() override;
		//! @brief Convert a basic CPU to a debugging CPU.
		explicit CPUDebug(ComSquare::CPU::CPU &cpu, SNES &snes);
		CPUDebug(const CPUDebug &) = delete;
		CPUDebug &operator=(const CPUDebug &) = delete;
		~CPUDebug() override = default;

		//! @brief Override the basic cpu's update to allow pausing of the CPU only.
		unsigned update() override;
	};
}

#endif //COMSQUARE_CPUDEBUG_HPP
