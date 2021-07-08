//
// Created by cbihan on 08/06/2021.
//

#pragma once

#include <array>
#include <QtWidgets/QWidget>
#include "QtWidgetSFML.hpp"
#include "Debugger/TileViewer/RAMTileRenderer.hpp"

namespace ComSquare::Renderer
{
	class QtSFMLTileRenderer : public QtWidgetSFML
	{
	public:
		std::array<std::array<uint32_t, 1024>, 1024> buffer;

		//! @brief Function called to update this widget.
		void onUpdate() override;

		//! @brief ctor
		explicit QtSFMLTileRenderer(QWidget* parent, int frameRate = 60);
		//! @brief copy ctor
		QtSFMLTileRenderer(const QtSFMLTileRenderer &) = delete;
		//! @brief default ctor
		~QtSFMLTileRenderer() override = default;
		//! @brief assignment operator
		QtSFMLTileRenderer &operator=(const QtSFMLTileRenderer &) = delete;
	};
}