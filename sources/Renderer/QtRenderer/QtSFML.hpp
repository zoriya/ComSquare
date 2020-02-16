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

namespace ComSquare::Renderer
{
	//! @brief A SFML renderer inside a QT window.
	class QtSFML : public SFRenderer {
	private:
		QApplication &_app;
	public:
		//! @brief Use this function to create the window.
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Constructor that return a SFML renderer inside a QT window.
		//! @param height height of the window.
		//! @param width width of the window.
		QtSFML(QApplication &app, unsigned int height, unsigned int width);
		QtSFML(const QtSFML &) = delete;
		QtSFML &operator=(const QtSFML &) = delete;
		~QtSFML() = default;
	};

	//! @brief The SFML window that manage the update.
	class MainQTWidget : public QtWidgetSFML {
	private:
		//! @brief The snes to update.
		SNES &_snes;
		void _onUpdate() override;
	public:
		MainQTWidget(SNES &snes, QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		MainQTWidget(const MainQTWidget &) = delete;
		MainQTWidget &operator=(const MainQTWidget &) = delete;
		~MainQTWidget() override = default;
	};
}

#endif //COMSQUARE_QTSFML_HPP
