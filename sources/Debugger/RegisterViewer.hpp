//
// Created by anonymus-raccoon on 5/28/20.
//

#ifndef COMSQUARE_REGISTERVIEWER_HPP
#define COMSQUARE_REGISTERVIEWER_HPP

#include <QtCore/QObject>
#include "ClosableWindow.hpp"
#include "../../ui/ui_registersView.h"

namespace ComSquare
{
	class SNES;

	namespace Debugger
	{
		class RegisterViewer : public QObject {
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow<RegisterViewer> *_window;
			//! @brief A widget that contain the whole UI.
			Ui::RegistersView _ui;

			//! @brief The snes instance to read/write to DMA channels.
			SNES &_snes;
		public:
			//! @brief Called when the window is closed. Turn off the debugger.
			void disableDebugger();

			explicit RegisterViewer(SNES &snes);
			RegisterViewer(
			const RegisterViewer &) = delete;
			RegisterViewer &operator=(const RegisterViewer &) = delete;
			~RegisterViewer() =default;

			//! @brief Focus the debugger's window.
			void focus();
		};
	};
}

#endif //COMSQUARE_REGISTERVIEWER_HPP
