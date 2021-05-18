//
// Created by cbihan on 18/05/2021.
//

#include "QtRenderSfml.hpp"
#include <iostream>

namespace ComSquare::Renderer
{/*
	QtRenderSFML::QtRenderSFML(ComSquare::SNES &snes, QWidget *parent, const QPoint &position, const QSize &size, int frameRate)
		: QtWidgetSFML(parent, position, size, frameRate),
		  _snes(snes)
	{
	}

	void QtRenderSFML::_onUpdate()
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
	}*/
}