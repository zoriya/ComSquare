//
// Created by anonymus-raccoon on 2/16/20.
//

#include "QtWindow.hpp"
#include <QIcon>

namespace ComSquare::Renderer
{
	QtWindow::QtWindow(unsigned int height, unsigned int width) :
		_frame(), _width(width), _height(height)
	{
		this->_frame.setWindowIcon(QIcon(":/resources/Logo.png"));
		this->_frame.show();
	}

	void QtWindow::setWindowName(std::string &newWindowName)
	{
		this->_frame.setWindowTitle((newWindowName + " - ComSquare").c_str());
	}
}