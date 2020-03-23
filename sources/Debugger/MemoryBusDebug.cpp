//
// Created by anonymus-raccoon on 3/20/20.
//

#include "MemoryBusDebug.hpp"
#include "../SNES.hpp"

namespace ComSquare::Debugger
{
	MemoryBusDebug::MemoryBusDebug(SNES &snes, const Memory::MemoryBus &bus)
		: MemoryBus(bus),
		_window(new ClosableWindow(*this, &MemoryBusDebug::disableViewer)),
		_snes(snes),
		_ui()
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);

		this->_ui.setupUi(this->_window);
		this->_window->show();
	}

	void MemoryBusDebug::disableViewer()
	{
		this->_snes.disableMemoryBusDebugging();
	}

	void MemoryBusDebug::focus()
	{
		this->_window->activateWindow();
	}

	bool MemoryBusDebug::isDebugger()
	{
		return true;
	}
}