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
	QtSFML::QtSFML(QApplication &app, unsigned int h, unsigned int w) :
		_app(app), _sfWidget(nullptr), width(w), height(h)
	{ }

	void QtSFML::createWindow(SNES &snes, int maxFPS)
	{
		this->frame = new QFrame();
		this->setWindowName(snes.cartridge->header.gameName);
		this->frame->show();
		this->_sfWidget = new QtFullSFML(snes, frame, QPoint(0, 0), QSize(this->width, this->height), maxFPS);
		this->_sfWidget->show();
	}

	void QtSFML::setWindowName(std::string newWindowName)
	{
		this->frame->setWindowTitle((newWindowName + " - ComSquare").c_str());
	}

	void QtSFML::putPixel(unsigned y, unsigned x, uint32_t rgba)
	{
		this->_sfWidget->putPixel(y, x, rgba);
	}

	void QtSFML::drawScreen() { }

	QtFullSFML::QtFullSFML(SNES &snes, QWidget *parent, const QPoint &position, const QSize &size, int frameRate) :
		QtWidgetSFML(parent, position, size, frameRate),
		_snes(snes)
	{ }

	void QtFullSFML::_onUpdate()
	{
		this->_snes.update();
	}
}