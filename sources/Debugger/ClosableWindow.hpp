//
// Created by anonymus-raccoon on 3/20/20.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include <utility>

namespace ComSquare::Debugger
{
	class ClosableWindow : public QMainWindow
	{
	protected:
		void closeEvent(QCloseEvent *) override
		{
			this->_onClose();
		}

	private:
		std::function<void ()> _onClose;

	public:
		explicit ClosableWindow(std::function<void ()> onClose)
			: _onClose(std::move(onClose))
		{
			this->setContextMenuPolicy(Qt::NoContextMenu);
			this->setAttribute(Qt::WA_QuitOnClose, false);
			this->setAttribute(Qt::WA_DeleteOnClose);
		}
		ClosableWindow(const ClosableWindow &) = delete;
		ClosableWindow &operator=(const ClosableWindow &) = delete;
		~ClosableWindow() override = default;
	};
}// namespace ComSquare::Debugger
