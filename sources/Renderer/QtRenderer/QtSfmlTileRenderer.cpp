//
// Created by cbihan on 08/06/2021.
//

#include "PPU/Tile.hpp"
#include "QtSfmlTileRenderer.hpp"


namespace ComSquare::Renderer
{

	QtSFMLTileRenderer::QtSFMLTileRenderer(QWidget *parent,
	                                       int frameRate)
		: QtWidgetSFML(parent, {0, 0}, {16 * PPU::Tile::NbPixelsWidth, 18 * PPU::Tile::NbPixelsHeight}, frameRate)
	{
		// todo the size of the sfml renderwindow should fill the parent
	}

	void QtSFMLTileRenderer::onUpdate()
	{
		int i = 0;
		int j = 0;
		this->_renderWindow.clear(sf::Color::Black);
		for (const auto &row : this->buffer) {
			for (const auto &pixel : row) {
				this->putPixel(j++, i, pixel);
			}
			i++;
			j = 0;
		}
		this->drawScreen();
	}
}