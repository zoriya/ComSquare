//
// Created by cbihan on 18/05/2021.
//

#pragma once

#include <QtWidgets/QWidget>
#include <SFML/Graphics/RenderWindow.hpp>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include "../IRenderer.hpp"
#include "../SFRenderer.hpp"
#include "QtWidgetSFML.hpp"
#include "QtSFML.hpp"

namespace ComSquare::Renderer
{
	class QtRenderSFML : public QtWidgetSFML
	{
	private:
		//! @brief The main window that the app reside on.
		//QMainWindow _window;
		QWidget _window;
		//! @brief The SFML widget.
		std::unique_ptr<QtFullSFML> _sfWidget = nullptr;
	public:

		void setParentWidget(QWidget &widget);
		//! @brief Use this function to create the window.
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param X horizontal index.
		//! @param Y vertical index.
		//! @param rgba The color of the pixel.
		void putPixel(unsigned y, unsigned x, uint32_t rgba) override;
		//! @brief This function doesn't do anything because QT internally handle drawing to the screen.
		void drawScreen() override;
		//! @brief Playing all samples from buffer
		//! @param samples Buffer containing samples
		//! @param sampleCount number of samples inside buffer
		void playAudio(std::span<int16_t> samples, uint64_t sampleCount) override;
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName) override;
		//! @brief Constructor that return a SFML renderer inside a QT window.
		//! @param height _height of the window.
		//! @param width _width of the window.
		QtRenderSFML(SNES &snes, QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		QtRenderSFML(const QtRenderSFML &) = delete;
		QtRenderSFML &operator=(const QtRenderSFML &) = delete;
		~QtRenderSFML() override = default;
	};
}
