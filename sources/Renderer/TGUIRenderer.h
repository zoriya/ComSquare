//
// Created by anonymus-raccoon on 2/14/20.
//

#ifndef _TGUIRENDERER_H_
#define _TGUIRENDERER_H_

#include <TGUI/Gui.hpp>
#include "SFRenderer.hpp"

namespace ComSquare::Renderer
{
	class TGUIRenderer : public SFRenderer {
	private:
		tgui::Gui _gui;
	public:
		//! @brief Constructor that return the window component of the SFML.
		//! @param height height of the window.
		//! @param width width of the window.
		//! @param maxFPS the number of maximum FPS for the window.
		TGUIRenderer(unsigned int height, unsigned int width, int maxFPS);
		TGUIRenderer(const TGUIRenderer &) = delete;
		TGUIRenderer &operator=(const TGUIRenderer &) = delete;
		~TGUIRenderer() = default;

		//! @brief Draw the screen and renderer ui elements.
		void drawScreen() override;
		//! @brief Get closing events and input for buttons...
		void getEvents() override;
	};
}

#endif //_TGUIRENDERER_H_
