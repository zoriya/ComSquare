//
// Created by anonymus-raccoon on 2/18/20.
//

#pragma once

#include <QtWidgets/QMainWindow>
#include "Cartridge/Cartridge.hpp"
#include "ui/ui_cartridgeView.h"
#include "ClosableWindow.hpp"

namespace ComSquare
{
	class SNES;
	namespace Debugger
	{
		//! @brief Window that show the header of the currently running game.
		class HeaderViewer
		{
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow *_window;
			//! @brief A reference to the snes (to disable the debugger).
			SNES &_snes;
			//! @brief The cartridge containing the header.
			Cartridge::Cartridge &_cartridge;
			//! @brief The layout of the viewer.
			Ui::CatridgeView _ui;
		public:
			//! @brief Focus the debugger's window.
			void focus();

			explicit HeaderViewer(SNES &snes);
			HeaderViewer(const HeaderViewer &) = delete;
			HeaderViewer &operator=(const HeaderViewer &) = delete;
			~HeaderViewer() = default;
		};
	}
}