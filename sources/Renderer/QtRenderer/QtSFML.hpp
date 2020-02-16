//
// Created by anonymus-raccoon on 2/15/20.
//

#ifndef COMSQUARE_QTSFML_HPP
#define COMSQUARE_QTSFML_HPP

#include <QtWidgets/QWidget>
#include <SFML/Graphics/RenderWindow.hpp>
#include <QtCore/QTimer>
#include "../IRenderer.hpp"
#include "../SFRenderer.hpp"
#include "QtWidgetSFML.hpp"
#include "QtWindow.hpp"

namespace ComSquare::Renderer
{
	//! @brief The SFML window that manage the update.
	class QtFullSFML : public QtWidgetSFML {
	private:
		//! @brief The snes to update.
		SNES &_snes;
		void _onUpdate() override;
	public:
		QtFullSFML(SNES &snes, QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		QtFullSFML(const QtFullSFML &) = delete;
		QtFullSFML &operator=(const QtFullSFML &) = delete;
		~QtFullSFML() override = default;
	};

	//! @brief A SFML renderer inside a QT window.
	class QtSFML : public IRenderer, public QtWindow {
	private:
		//! @brief The SFML widget.
		std::unique_ptr<QtFullSFML> _sfWidget = nullptr;
	public:
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
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName) override;
		//! @brief Constructor that return a SFML renderer inside a QT window.
		//! @param height _height of the window.
		//! @param width _width of the window.
		QtSFML(unsigned int height, unsigned int width);
		QtSFML(const QtSFML &) = delete;
		QtSFML &operator=(const QtSFML &) = delete;
		~QtSFML() = default;
	};
}

#endif //COMSQUARE_QTSFML_HPP
