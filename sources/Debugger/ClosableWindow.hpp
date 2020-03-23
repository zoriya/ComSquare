//
// Created by anonymus-raccoon on 3/20/20.
//

#ifndef COMSQUARE_CLOSABLEWINDOW_HPP
#define COMSQUARE_CLOSABLEWINDOW_HPP

#include <QtWidgets/QMainWindow>

namespace ComSquare::Debugger
{
	template <typename T>
	class ClosableWindow : public QMainWindow {
	protected:
		void closeEvent(QCloseEvent *) override
		{
			(this->_obj.*this->_onClose)();
		}

	private:
		T &_obj;
		void (T::*_onClose)();

	public:
		explicit ClosableWindow(T &obj, void (T::*onClose)())
			: _obj(obj), _onClose(onClose)
		{ }
		ClosableWindow(const ClosableWindow &) = delete;
		ClosableWindow &operator=(const ClosableWindow &) = delete;
		~ClosableWindow() override = default;
	};
}

#endif //COMSQUARE_CLOSABLEWINDOW_HPP
