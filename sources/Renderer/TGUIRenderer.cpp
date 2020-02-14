//
// Created by anonymus-raccoon on 2/14/20.
//

#include "TGUIRenderer.h"
#include <TGUI/TGUI.hpp>

namespace ComSquare::Renderer
{
	TGUIRenderer::TGUIRenderer(unsigned int height, unsigned int width, int maxFPS) :
		SFRenderer(height, width, maxFPS),
		_gui(this->_window)
	{
		_gui.loadWidgetsFromFile("../ressources/form.txt");
	}

	void TGUIRenderer::drawScreen()
	{
		this->_gui.draw();
		this->_window.display();
	}

	void TGUIRenderer::getEvents()
	{
		sf::Event event;
		while (this->_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->shouldExit = true;
				break;
			}
			this->_gui.handleEvent(event);
		}
	}
}