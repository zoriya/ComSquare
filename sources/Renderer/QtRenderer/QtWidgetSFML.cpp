//
// Created by anonymus-raccoon on 2/16/20.
//

#include "QtWidgetSFML.hpp"

namespace ComSquare::Renderer
{
	QtWidgetSFML::QtWidgetSFML(QWidget *parent, const QPoint &position, const QSize &size, int frameRate)
		: QWidget(parent),
		  SFRenderer(size.height(), size.width())
	{
		this->setAttribute(Qt::WA_PaintOnScreen);
		this->setAttribute(Qt::WA_OpaquePaintEvent);
		this->setAttribute(Qt::WA_NoSystemBackground);

		this->setFocusPolicy(Qt::StrongFocus);

		this->move(position);
		this->resize(size);
		this->_timer.setInterval(1000 / frameRate);
	}

	void QtWidgetSFML::showEvent(QShowEvent *)
	{
		if (!this->_isInitialized) {
			// Under X11, we need to flush the commands sent to the server to ensure that
			// SFML will get an updated view of the windows
#ifdef Q_WS_X11
			XFlush(QX11Info::display());
#endif
			this->_renderWindow.create(static_cast<sf::WindowHandle>(this->winId()));
			this->_renderWindow.setFramerateLimit(60);
			this->_onInit();

			this->_timer.setSingleShot(false);
			connect(&_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
			this->_timer.start();
			this->_isInitialized = true;
		}
	}

	QPaintEngine *QtWidgetSFML::paintEngine() const
	{
		return nullptr;
	}

	void QtWidgetSFML::paintEvent(QPaintEvent *)
	{
		this->drawScreen();
	}

	void QtWidgetSFML::_onInit()
	{}

	void QtWidgetSFML::setSize(unsigned int width, unsigned int height)
	{
		SFRenderer::setSize(width, height);
		QWidget::resize(static_cast<int>(width), static_cast<int>(height));
	}

	void QtWidgetSFML::onUpdate()
	{
		this->drawScreen();
	}
}