//
// Created by anonymus-raccoon on 2/16/20.
//

#ifndef COMSQUARE_QTWIDGETSFML_H
#define COMSQUARE_QTWIDGETSFML_H

#include <QtWidgets/QWidget>
#include <SFML/Graphics/RenderWindow.hpp>
#include <QtCore/QTimer>

namespace ComSquare::Renderer
{
	//! @brief A widget that you can put inside a QT application that render using the SFML.
	class QtWidgetSFML : public QWidget, public sf::RenderWindow {
	private:
		virtual void _onInit();
		virtual void _onUpdate() = 0;
		QPaintEngine* paintEngine() const override;
		void showEvent(QShowEvent*) override;
		void paintEvent(QPaintEvent*) override;

		QTimer _timer;
		bool _isInitialized = false;
	public:
		QtWidgetSFML(QWidget* parent, const QPoint& position, const QSize& size, int frameRate = 0);
		QtWidgetSFML(const QtWidgetSFML &) = delete;
		QtWidgetSFML &operator=(const QtWidgetSFML &) = delete;
		~QtWidgetSFML() override = default;
	};
}

#endif //COMSQUARE_QTWIDGETSFML_H
