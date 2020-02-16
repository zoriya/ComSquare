//
// Created by anonymus-raccoon on 2/17/20.
//

#include "RamViewer.hpp"
#include "../SNES.hpp"

namespace ComSquare::Debugger
{
	RamViewer::RamViewer(ComSquare::SNES &snes) :
		QMainWindow(), _snes(snes)
	{
		this->setContextMenuPolicy(Qt::NoContextMenu);
		this->setAttribute(Qt::WA_QuitOnClose, false);

//		this->_ui.setupUi(this);
		this->show();
	}
}