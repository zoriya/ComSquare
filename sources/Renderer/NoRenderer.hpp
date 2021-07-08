//
// Created by anonymus-raccoon on 2/5/20.
//

#pragma once

#include "IRenderer.hpp"

namespace ComSquare::Renderer
{
	//! @brief A renderer that discard everything you give. (Used for tests).
	class NoRenderer : public IRenderer
	{
	public:
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName) override;
		//! @brief Update the screen by printing the buffer.
		void drawScreen() override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param X horizontal index.
		//! @param Y vertical index.
		//! @param rgba The color of the pixel.
		void putPixel(unsigned y, unsigned x, uint32_t rgba) override;
		//! @brief Playing all samples from buffer
		//! @param samples Buffer containing samples
		//! @param sampleCount number of samples inside buffer
		void playAudio(std::span<int16_t> samples) override;
		//! @brief Use this function to create the window.
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Constructor that return the window component of the SFML.
		//! @param height _height of the window.
		//! @param width _width of the window.
		//! @param maxFPS the number of maximum FPS for the window.
		NoRenderer(unsigned int height, unsigned int width, int maxFPS);
		NoRenderer(const NoRenderer &) = default;
		NoRenderer &operator=(const NoRenderer &) = default;
		~NoRenderer() = default;
	};
}
