//
// Created by cbihan on 08/06/2021.
//

#include "PPU/Tile.hpp"
#include "QtSfmlTileRenderer.hpp"


namespace ComSquare::Renderer
{

	QtSFMLTileRenderer::QtSFMLTileRenderer(QWidget *parent,
	                                       int frameRate)
		: QtWidgetSFML(parent, {0, 0}, {160 * PPU::Tile::NbPixelsWidth, 180 * PPU::Tile::NbPixelsHeight}, frameRate)
	{
		// todo the size of the sfml renderwindow should fill the parent
	}

	void QtSFMLTileRenderer::onUpdate()
	{
		this->_renderWindow.clear(sf::Color::Black);
		this->drawScreen();
	}
}