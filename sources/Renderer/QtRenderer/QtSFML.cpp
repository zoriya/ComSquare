//
// Created by anonymus-raccoon on 2/15/20.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QIcon>
#include <iostream>
#include "QtSFML.hpp"

#ifdef Q_WS_X11
	#include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

namespace ComSquare::Renderer
{
	QtSFML::QtSFML(unsigned int h, unsigned int w) :
		QtWindow(h, w), _sfWidget(nullptr)
	{ }

	void QtSFML::createWindow(SNES &snes, int maxFPS)
	{
		this->setWindowName(snes.cartridge->header.gameName);
		this->_sfWidget = std::make_unique<QtFullSFML>(snes, &_frame, QPoint(0, 0), QSize(this->_width, this->_height), maxFPS);
		this->_sfWidget->show();
	}

	void QtSFML::putPixel(unsigned y, unsigned x, uint32_t rgba)
	{
		this->_sfWidget->putPixel(y, x, rgba);
	}

	void QtSFML::drawScreen() { }

	void QtSFML::setWindowName(std::string &newWindowName)
	{
		QtWindow::setWindowName(newWindowName);
	}

	QtFullSFML::QtFullSFML(SNES &snes, QWidget *parent, const QPoint &position, const QSize &size, int frameRate) :
		QtWidgetSFML(parent, position, size, frameRate),
		_snes(snes)
	{ }

	void QtFullSFML::_onUpdate()
	{
		this->_snes.update();
	}
}