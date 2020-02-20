//
// Created by anonymus-raccoon on 2/15/20.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QIcon>
#include <QMenuBar>
#include <iostream>
#include "QtSFML.hpp"

#ifdef Q_WS_X11
	#include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

namespace ComSquare::Renderer
{
	QtSFML::QtSFML(unsigned int h, unsigned int w) :
		_window(), _sfWidget(nullptr)
	{
		this->_window.resize(w, h);
		this->_window.setWindowIcon(QIcon(":/resources/Logo.png"));
	}

	void QtSFML::createWindow(SNES &snes, int maxFPS)
	{
		this->setWindowName(snes.cartridge->header.gameName);
		this->_sfWidget = std::make_unique<QtFullSFML>(snes, &_window, QPoint(0, 0), QSize(this->_window.width(), this->_window.height()), maxFPS);
		this->_window.setCentralWidget(this->_sfWidget.get());

		QMenu *file = this->_window.menuBar()->addMenu("&File");
		//TODO implement rom openning from this menu.
		(void)file;

		QMenu *game = this->_window.menuBar()->addMenu("Game");
		QAction *reset = new QAction("Reset", &this->_window);
		QMainWindow::connect(reset, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::reset);
		game->addAction(reset);

		QMenu *debugger = this->_window.menuBar()->addMenu("Debugger");
		QAction *cpuDebugger = new QAction("CPU's Debugger", &this->_window);
		cpuDebugger->setShortcut(Qt::Key_F1);
		QMainWindow::connect(cpuDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableDebugCPU);
		debugger->addAction(cpuDebugger);
		QAction *ramViewer = new QAction("Memory viewer", &this->_window);
		ramViewer->setShortcut(Qt::Key_F2);
		QMainWindow::connect(ramViewer, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableRamViewer);
		debugger->addAction(ramViewer);
		QAction *headerViewer = new QAction("Header viewer", &this->_window);
		headerViewer->setShortcut(Qt::Key_F3);
		QMainWindow::connect(headerViewer, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableHeaderViewer);
		debugger->addAction(headerViewer);
		QAction *apuDebugger = new QAction("APU's Debugger", &this->_window);
		apuDebugger->setShortcut(Qt::Key_F4);
		QMainWindow::connect(apuDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableDebugAPU);
		debugger->addAction(apuDebugger);

		this->_window.show();
	}

	void QtSFML::putPixel(unsigned y, unsigned x, uint32_t rgba)
	{
		this->_sfWidget->putPixel(y, x, rgba);
	}

	void QtSFML::drawScreen() { }

	void QtSFML::setWindowName(std::string &newWindowName)
	{
		this->_window.setWindowTitle((newWindowName + " - ComSquare").c_str());
	}

	QtFullSFML::QtFullSFML(SNES &snes, QWidget *parent, const QPoint &position, const QSize &size, int frameRate) :
		QtWidgetSFML(parent, position, size, frameRate),
		_snes(snes)
	{ }

	void QtFullSFML::_onUpdate()
	{
		try {
			this->_snes.update();
		} catch (std::exception &e) {
			std::cerr << "An error occurred: " << e.what() << std::endl;
			QApplication::quit();
		}
	}

	void QtFullSFML::enableDebugCPU()
	{
		this->_snes.enableCPUDebugging();
	}

	void QtFullSFML::reset()
	{
		this->_snes.cpu->RESB();
	}

	void QtFullSFML::enableRamViewer()
	{
		this->_snes.enableRamViewer();
	}

	void QtFullSFML::enableHeaderViewer()
	{
		this->_snes.enableHeaderViewer();
	}

	void QtFullSFML::enableDebugAPU()
	{
		this->_snes.enableAPUDebugging();
	}
}