//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef COMSQUARE_DEBUGCPU_HPP
#define COMSQUARE_DEBUGCPU_HPP

#include "../CPU/CPU.hpp"
#include "../Renderer/SFRenderer.hpp"
#include "../SNES.hpp"

namespace ComSquare::Debugger
{
	//! @brief A custom CPU with a window that show it's registers and the disassembly.
	class CPUDebug : public CPU::CPU {
	private:
		//! @brief The debug window.
		Renderer::SFRenderer _renderer;
		//! @brief If this is set to true, the execution of the CPU will be paused.
		bool _isPaused = true;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
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
