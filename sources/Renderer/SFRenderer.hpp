//
// Created by cbihan on 1/30/20.
//

#ifndef COMSQUARE_SFRENDERER_HPP
#define COMSQUARE_SFRENDERER_HPP

#include "IRenderer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics//RenderWindow.hpp>

namespace ComSquare::Renderer
{
	class SFRenderer : public IRenderer {
	public:
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param A new title for the window.
		void setWindowName(std::string) override;
		//! @brief Update the screen by printing the buffer.
		void drawScreen() override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param X : horizontal index.
		//! @param Y : vertical index.
		//! @param rgba : The color of the pixel.
		void putPixel(int x, int y, uint8_t rgba) override ;
	};
}

#endif //COMSQUARE_SFRENDERER_HPP
