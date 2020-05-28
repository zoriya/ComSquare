//
// Created by anonymus-raccoon on 5/28/20.
//

#include "DMADebug.hpp"
#include "../../../SNES.hpp"

namespace ComSquare::Debugger
{
	DMADebug::DMADebug(SNES &snes)
		: _window(new ClosableWindow<DMADebug>(*this, &DMADebug::disableDebugger)),
		_ui(),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_window->show();
	}

	void DMADebug::focus()
	{
		this->_window->activateWindow();
	}

	void DMADebug::disableDebugger()
	{
		this->_snes.disableDMADebugging();
	}
}