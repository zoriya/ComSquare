//
// Created by anonymus-raccoon on 3/20/20.
//

#ifndef COMSQUARE_MEMORYBUSDEBUG_HPP
#define COMSQUARE_MEMORYBUSDEBUG_HPP

#include <QtWidgets/QMainWindow>
#include "../Memory/MemoryBus.hpp"
#include "../../ui/ui_busView.h"

namespace ComSquare::Debugger
{
	//! @brief window that allow the user to view all data going through the memory bus.
	class MemoryBusDebug : public Memory::MemoryBus, public QMainWindow {
	private:
		//! @brief A widget that contain the whole UI.
		Ui::BusView _ui;
	public:
		explicit MemoryBusDebug(const Memory::MemoryBus &bus);
		MemoryBusDebug(const MemoryBusDebug &) = delete;
		MemoryBusDebug &operator=(const MemoryBusDebug &) = delete;
		~MemoryBusDebug() override = default;
	};
}

#endif //COMSQUARE_MEMORYBUSDEBUG_HPP
