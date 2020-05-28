//
// Created by anonymus-raccoon on 5/28/20.
//

#include "RegisterViewer.hpp"
#include "../SNES.hpp"

namespace ComSquare::Debugger
{
	RegisterViewer::RegisterViewer(SNES &snes)
		: _window(new ClosableWindow<RegisterViewer>(*this, &RegisterViewer::disableDebugger)),
		_ui(),
		_snes(snes)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		this->_window->show();
	}

	void RegisterViewer::focus()
	{
		this->_window->activateWindow();
	}

	void RegisterViewer::disableDebugger()
	{
		this->_snes.disableRegisterDebugging();
	}
}