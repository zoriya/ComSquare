//
// Created by anonymus-raccoon on 3/20/20.
//

#include "MemoryBusDebug.hpp"

namespace ComSquare::Debugger
{
	MemoryBusDebug::MemoryBusDebug(const ComSquare::Memory::MemoryBus &bus)
		: MemoryBus(bus), QMainWindow(), _ui()
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this);
//		QMainWindow::connect(this->_ui.actionPause, &QAction::triggered, this, &CPUDebug::pause);
		this->show();
	}
}