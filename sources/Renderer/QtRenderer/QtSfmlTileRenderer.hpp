//
// Created by cbihan on 08/06/2021.
//

#pragma once

#include <QtWidgets/QWidget>
#include "QtWidgetSFML.hpp"

namespace ComSquare::Renderer
{
	class QtSFMLTileRenderer : public QtWidgetSFML
	{
	private:
		//! @brief internal buffer
		//sf::Image _image;
		//! @brief sprite
		//sf::Sprite _sprite;
	public:

		//! @brief Function called to update this widget.
		void _onUpdate() override;

		//! @brief ctor
		explicit QtSFMLTileRenderer(QWidget* parent, int frameRate = 0);
		//! @brief copy ctor
		QtSFMLTileRenderer(const QtSFMLTileRenderer &) = delete;
		//! @brief default ctor
		~QtSFMLTileRenderer() override = default;
		//! @brief assignment operator
		QtSFMLTileRenderer &operator=(const QtSFMLTileRenderer &) = delete;
	};
}