//
// Created by anonymus-raccoon on 2/17/20.
//

#ifndef COMSQUARE_RAMVIEWER_HPP
#define COMSQUARE_RAMVIEWER_HPP

#include <QtWidgets/QMainWindow>

namespace ComSquare
{
	class SNES;
	namespace Debugger
	{
		class RamViewer : public QMainWindow {
		private:
			//! @brief SNES containing all rams to view.
			SNES &_snes;
			//! @brief The layout of the viewer.
//			Ui::RamView _ui;
		public:
			explicit RamViewer(SNES &snes);
			RamViewer(const RamViewer &) = delete;
			RamViewer &operator=(const RamViewer &) = delete;
			~RamViewer() override = default;
		};
	}
}

#endif //COMSQUARE_RAMVIEWER_HPP
