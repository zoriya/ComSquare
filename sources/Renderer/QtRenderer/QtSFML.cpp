//
// Created by anonymus-raccoon on 2/15/20.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QCloseEvent>
#include <iostream>
#include "QtSFML.h"

#ifdef Q_WS_X11
	#include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

namespace ComSquare::Renderer
{
	QtSFML::QtSFML(QApplication &app, unsigned int height, unsigned int width) :
		SFRenderer(height, width), _app(app)
	{ }

	void QtSFML::createWindow(SNES &snes, int maxFPS)
	{
		QFrame *frame = new QFrame();
		frame->show();
		MainQTWidget *sfView = new MainQTWidget(snes, frame, QPoint(0, 0), QSize(this->_videoMode.width, this->_videoMode.height), maxFPS);
		sfView->show();
	}

	MainQTWidget::MainQTWidget(SNES &snes, QWidget *parent, const QPoint &position, const QSize &size, int frameRate) :
		QtWidgetSFML(parent, position, size, frameRate),
		_snes(snes)
	{ }

	void MainQTWidget::_onUpdate()
	{
		this->_snes.update();
	}
}