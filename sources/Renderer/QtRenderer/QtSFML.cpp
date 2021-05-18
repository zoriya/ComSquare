//
// Created by anonymus-raccoon on 2/15/20.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QIcon>
#include <QMenuBar>
#include <iostream>
#include "../../Exceptions/DebuggableError.hpp"
#include "QtSFML.hpp"

#ifdef Q_WS_X11
	#include <Qt/qx11info_x11.h>
	#include <X11/Xlib.h>
#endif

namespace ComSquare::Renderer
{
	QtSFML::QtSFML(QWidget *parentWidget)
		: _window(parentWidget), _sfWidget(nullptr)
	{
		this->_window->resize(parentWidget->width(), parentWidget->height());
	}

	void QtSFML::createWindow(SNES &snes, int maxFPS)
	{
		this->_sfWidget = std::make_unique<QtFullSFML>(snes, &_window, QPoint(0, 0), QSize(this->_window.width(), this->_window.height()), maxFPS);
	}

	void QtSFML::putPixel(unsigned y, unsigned x, uint32_t rgba)
	{
		this->_sfWidget->putPixel(y, x, rgba);
	}

	void QtSFML::playAudio(std::span<int16_t> samples, uint64_t sampleCount)
	{
		this->_sfWidget->playAudio(samples, sampleCount);
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
		} catch (const DebuggableError &e) {
			std::cout << "Invalid rom's instruction: " << e.what() << std::endl;
			this->_snes.enableCPUDebuggingWithError(e);
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

	void QtFullSFML::enableDebugBus()
	{
		this->_snes.enableMemoryBusDebugging();
	}

	void QtFullSFML::enableCgramViewer()
	{
		this->_snes.enableCgramDebugging();
	}

	void QtFullSFML::enableRegisterViewer()
	{
		this->_snes.enableRegisterDebugging();
	}

	void QtFullSFML::enableTileViewer()
	{
		this->_snes.enableTileViewerDebugging();
	}

	QtSFMLWindow::QtSFMLWindow(unsigned int height, unsigned int width)
		: QtSFML(this->_window)
	{
		this->_window.resize(width, height);
		this->_window.setWindowIcon(QIcon(":/resources/Logo.png"));
	}

	void QtSFMLWindow::createWindow(SNES &snes, int maxFPS)
	{
		QtSFML::createWindow(snes, maxFPS);
		this->setWindowName(snes.cartridge->header.gameName);
		this->_window.setCentralWidget(this->_sfWidget.get());

		QMenu *file = this->_window.menuBar()->addMenu("&File");
		//TODO implement rom opening from this menu.
		(void)file;

		QMenu *game = this->_window.menuBar()->addMenu("&Game");
		QAction *reset = new QAction("Reset", &this->_window);
		QMainWindow::connect(reset, &QAction::triggered, this->window.get(), &QtFullSFML::reset);
		game->addAction(reset);



		QMenu *debugger = this->_window.menuBar()->addMenu("&Debugger");
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

		QAction *busDebugger = new QAction("Memory bus Viewer", &this->_window);
		busDebugger->setShortcut(Qt::Key_F5);
		QMainWindow::connect(busDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableDebugBus);
		debugger->addAction(busDebugger);

		QAction *cgramDebugger = new QAction("Palette Viewer", &this->_window);
		cgramDebugger->setShortcut(Qt::Key_F6);
		QMainWindow::connect(cgramDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableCgramViewer);
		debugger->addAction(cgramDebugger);

		QAction *registerDebugger = new QAction("Registers Viewer", &this->_window);
		registerDebugger->setShortcut(Qt::Key_F7);
		QMainWindow::connect(registerDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableRegisterViewer);
		debugger->addAction(registerDebugger);

		QAction *tileDebugger = new QAction("Tile Viewer", &this->_window);
		tileDebugger->setShortcut(Qt::Key_F8);
		QMainWindow::connect(tileDebugger, &QAction::triggered, this->_sfWidget.get(), &QtFullSFML::enableTileViewer);
		debugger->addAction(tileDebugger);

		this->_window.show();
	}
}