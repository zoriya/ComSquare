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

			//! @brief Set a size or resize the Renderer drawing size
			//! @param height The new height of the renderer in pixels
			//! @param width The new width of the renderer in pixels
			virtual void setSize(unsigned height, unsigned width) = 0;

			//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
			//! @param horizontalPosition horizontal index.
			//! @param verticalPosition vertical index.
			//! @param rgba The color of the pixel.
			virtual void putPixel(unsigned verticalPosition, unsigned horizontalPosition, uint32_t rgba) = 0;

			//! @brief Use this function to create the window.
			//! @param snes The snes game object (to call the update method).
			//! @param maxFPS The number of FPS you aim to run on.
			virtual void createWindow(SNES &snes, int maxFPS) = 0;

			//! @brief Set the scale of the renderer (it will scale all pixels)
			//! @param width The scale factor for width
			//! @param height The scale factor for height
			virtual void setScale(float height, float width) = 0;

			//! @brief Playing all samples from buffer
			//! @param samples Buffer containing samples
			virtual void playAudio(std::span<int16_t> samples) = 0;
		};
	}
}
