//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef COMSQUARE_DEBUGCPU_HPP
#define COMSQUARE_DEBUGCPU_HPP

#include "../CPU/CPU.hpp"
#include "../Renderer/SFRenderer.hpp"
#include "../SNES.hpp"
#include "../Renderer/QtRenderer/QtWindow.hpp"
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
		//! @brief Get a printable string representing an instruction.
		static std::string _getInstructionString(uint8_t opcode);
	public slots:
		//! @brief Pause/Resume the CPU.
		void pause();
		//! @brief Step - Execute a single instruction.
		void step();
	public:
		//! @brief Convert a basic CPU to a debugging CPU.
		explicit CPUDebug(ComSquare::CPU::CPU &cpu, SNES &snes);
		CPUDebug(const CPUDebug &) = delete;
		CPUDebug &operator=(const CPUDebug &) = delete;
		~CPUDebug() = default;

		//! @brief Override the basic cpu's update to allow pausing of the CPU only.
		unsigned update() override;
	};
}

#endif //COMSQUARE_DEBUGCPU_HPP
