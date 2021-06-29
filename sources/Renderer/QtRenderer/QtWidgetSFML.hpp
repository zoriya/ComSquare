//
// Created by anonymus-raccoon on 2/16/20.
//

#ifndef COMSQUARE_QTWIDGETSFML_HPP
#define COMSQUARE_QTWIDGETSFML_HPP

#include <QtWidgets/QWidget>
#include <SFML/Graphics/RenderWindow.hpp>
#include <QtCore/QTimer>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../SFRenderer.hpp"

namespace ComSquare::Renderer
{
	//! @brief A widget that you can put inside a QT application that render using the SFML.
	class QtWidgetSFML : public QWidget, public SFRenderer {
	private:
		//! @brief Function called when this widget is created.
		virtual void _onInit();
		//! @brief Function called to update this widget.
		virtual void _onUpdate() = 0;
		//! @brief Qt internal paint engine (always null since we use a custom one)
		QPaintEngine* paintEngine() const override;
		//! @brief Used to create the SF window and bind it to the window manager of the user.
		void showEvent(QShowEvent*) override;
		//! @brief QT event that refresh the widget. (A draw screen)
		void paintEvent(QPaintEvent*) override;

		//! @brief Internal timer used for update intervals.
		QTimer _timer;
		//! @brief Has the SF window been created yet.
		bool _isInitialized = false;
	public:
		QtWidgetSFML(QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		QtWidgetSFML(const QtWidgetSFML &) = delete;
		QtWidgetSFML &operator=(const QtWidgetSFML &) = delete;
		~QtWidgetSFML() override = default;
	};
}

#endif //COMSQUARE_QTWIDGETSFML_HPP
