//
// Created by cbihan on 08/06/2021.
//

#include <iostream>
#include "QtSfmlTileRenderer.hpp"

namespace ComSquare::Renderer
{

	QtSFMLTileRenderer::QtSFMLTileRenderer(QWidget *parent,
	                                       int frameRate)
		: QtWidgetSFML(parent, {0, 0}, {1025, 1025}, frameRate)
	{
		// todo the size of the sfml renderwindow should fill the parent
		std::cout << "size: " << parent->width() << " " << parent->height()  << std::endl;
	}

	void QtSFMLTileRenderer::_onUpdate()
	{
		this->_window.clear(sf::Color::Black);
		for (unsigned long i = 0; i < this->buffer.size(); i++) {
			for (unsigned long j = 0; j < this->buffer[i].size(); j++) {
				this->putPixel(j, i, this->buffer[i][j]);
			}
		}
		this->drawScreen();
	}
}