//
// Created by cbihan on 08/06/2021.
//

#include <iostream>
#include "QtSfmlTileRenderer.hpp"

namespace ComSquare::Renderer
{

	QtSFMLTileRenderer::QtSFMLTileRenderer(QWidget *parent,
	                                       int frameRate)
		: QtWidgetSFML(parent, {0, 0}, {500, 1000}, frameRate)
	{
		// todo the size of the sfml renderwindow should fill the parent
		std::cout << "size: " << parent->width() << " " << parent->height()  << std::endl;
	}

	void QtSFMLTileRenderer::_onUpdate()
	{
		this->_window.clear(sf::Color::Blue);
		this->_window.display();
	}
}