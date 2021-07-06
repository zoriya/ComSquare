//
// Created by cbihan on 1/30/20.
//

#pragma once

#include <string>
#include <span>

namespace ComSquare
{
	class SNES;
	namespace Renderer
	{
		class IRenderer {
		public:
			//! @brief Set a new name to the window, if there is already a name it will be overwrite
			virtual void setWindowName(std::string &) = 0;

			//! @brief Render the buffer to the window
			virtual void drawScreen() = 0;

			//! @brief Set a pixel to the coordinates x, y with the color rgba
			//! @param x The x position of the window (0, 0 is the top left corner).
			//! @param y The y position of the window (0, 0 is the top left corner).
			//! @param rgba The color of the pixel (red, green, blue, alpha).
			virtual void putPixel(unsigned x, unsigned y, uint32_t rgba) = 0;

			//! @brief Use this function to create the window.
			//! @param snes The snes game object (to call the update method).
			//! @param maxFPS The number of FPS you aim to run on.
			virtual void createWindow(SNES &snes, int maxFPS) = 0;

			//! @brief Playing all samples from buffer
			//! @param samples Buffer containing samples
			virtual void playAudio(std::span<int16_t> samples) = 0;
		};
	}
}
