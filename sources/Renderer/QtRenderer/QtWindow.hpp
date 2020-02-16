//
// Created by anonymus-raccoon on 2/16/20.
//

#ifndef COMSQUARE_QTWINDOW_HPP
#define COMSQUARE_QTWINDOW_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>

namespace ComSquare::Renderer
{
	class QtWindow {
	protected:
		//! @brief The SFML frame.
		QFrame _frame;
		//! @brief The _width of the window.
		unsigned int _width;
		//! @brief The _height of the window.
		unsigned int _height;
	public:
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName);
		//! @brief Constructor that return a SFML renderer inside a QT window.
		//! @param height _height of the window.
		//! @param width _width of the window.
		QtWindow(unsigned int height, unsigned int width);
		QtWindow(const QtWindow &) = delete;
		QtWindow &operator=(const QtWindow &) = delete;
		~QtWindow() = default;
	};
}

#endif //COMSQUARE_QTWINDOW_HPP
