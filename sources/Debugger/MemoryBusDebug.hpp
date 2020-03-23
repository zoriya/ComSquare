//
// Created by anonymus-raccoon on 3/20/20.
//

#ifndef COMSQUARE_MEMORYBUSDEBUG_HPP
#define COMSQUARE_MEMORYBUSDEBUG_HPP

#include <QtWidgets/QMainWindow>
#include "../Memory/MemoryBus.hpp"
#include "../../ui/ui_busView.h"
#include "ClosableWindow.hpp"

namespace ComSquare::Debugger
{
	//! @brief window that allow the user to view all data going through the memory bus.
	class MemoryBusDebug : public Memory::MemoryBus {
	private:
		//! @brief The QT window for this debugger.
		ClosableWindow<MemoryBusDebug> *_window;
		//! @brief A reference to the snes (to disable the debugger).
		SNES &_snes;
		//! @brief A widget that contain the whole UI.
		Ui::BusView _ui;
	public slots:
		//! @brief Called when the window is closed. Turn off the debugger and revert to a basic CPU.
		void disableViewer();
	public:
		explicit MemoryBusDebug(SNES &snes, const Memory::MemoryBus &bus);
		MemoryBusDebug(const MemoryBusDebug &) = delete;
		MemoryBusDebug &operator=(const MemoryBusDebug &) = delete;
		~MemoryBusDebug() = default;

		//! @brief Focus the debugger's window.
		void focus();

		//! @brief Return true if the Bus is overloaded with debugging features.
		bool isDebugger() override;
	};
}

#endif //COMSQUARE_MEMORYBUSDEBUG_HPP
