//
// Created by anonymus-raccoon on 5/28/20.
//

#ifndef COMSQUARE_DMADEBUG_HPP
#define COMSQUARE_DMADEBUG_HPP

#include <QtCore/QObject>
#include "../../ClosableWindow.hpp"
#include "../../../../ui/ui_dmaView.h"

namespace ComSquare
{
	class SNES;

	namespace Debugger
	{
		class DMADebug : public QObject {
		private:
			//! @brief The QT window for this debugger.
			ClosableWindow <DMADebug> *_window;
			//! @brief A widget that contain the whole UI.
			Ui::DMAView _ui;

			//! @brief The snes instance to read/write to DMA channels.
			SNES &_snes;
		public:
			//! @brief Called when the window is closed. Turn off the debugger.
			void disableDebugger();

			explicit DMADebug(SNES &snes);
			DMADebug(
			const DMADebug &) = delete;
			DMADebug &operator=(const DMADebug &) = delete;
			~DMADebug() =default;

			//! @brief Focus the debugger's window.
			void focus();
		};
	};
}

#endif //COMSQUARE_DMADEBUG_HPP
