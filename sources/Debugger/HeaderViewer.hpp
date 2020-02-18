//
// Created by anonymus-raccoon on 2/18/20.
//

#ifndef COMSQUARE_HEADERVIEWER_HPP
#define COMSQUARE_HEADERVIEWER_HPP

#include <QtWidgets/QMainWindow>
#include "../Cartridge/Cartridge.hpp"
#include "../../ui/ui_cartridgeView.h"

namespace ComSquare::Debugger
{
	//! @brief Window that show the header of the currently running game.
	class HeaderViewer  : public QMainWindow {
	private:
		//! @brief The cartrdige containing the header.
		Cartridge::Cartridge &_cartridge;
		//! @brief The layout of the viewer.
		Ui::CatridgeView _ui;
	public:
		explicit HeaderViewer(Cartridge::Cartridge &cartridge);
		HeaderViewer(const HeaderViewer &) = delete;
		HeaderViewer &operator=(const HeaderViewer &) = delete;
		~HeaderViewer() override = default;
	};
}

#endif //COMSQUARE_HEADERVIEWER_HPP
