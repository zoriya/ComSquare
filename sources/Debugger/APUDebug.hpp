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
	class APUDebug : public APU::APU, public QObject {
	private:
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

		//! @brief Convert CPU APU flags to a string.
		std::string _getPSWString();

		//! @brief Replace original _executeInstruction to write to the logger.
		int _executeInstruction() override;

		//! @brief return the mnemonic of the current instruction done.
		std::string _getInstructionString();
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