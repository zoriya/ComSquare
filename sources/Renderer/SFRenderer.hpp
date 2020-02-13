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
	class InvalidPixelPosition : public std::exception {
	private:
		std::string _msg;
	public:
		explicit InvalidPixelPosition(const std::string &name, unsigned int x, unsigned int width)
			: _msg("Trying to place a pixel at an invalid " + name + " (" + std::to_string(x) + ">=" + std::to_string(width) + ")") {}
		const char *what() const noexcept override { return this->_msg.c_str(); }
	};

	class SFRenderer : public IRenderer {
	private:
		//! @brief The Renderer for the window.
		sf::RenderWindow window;
		//! @brief Video Mode containing the height and width of the window.
		sf::VideoMode videoMode;
		//! @brief The image that contain all of the pixels
		sf::Color *pixelBuffer;
		//! @brief The sprite to render the array of pixels
		sf::Sprite sprite;
		//! @brief The texture to render the array of pixels
		sf::Texture texture;
	public:
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string newWindowName) override;
		//! @brief Update the screen by printing the buffer.
		void drawScreen() override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param X horizontal index.
		//! @param Y vertical index.
		//! @param rgba The color of the pixel.
		void putPixel(unsigned y, unsigned x, uint32_t rgba) override ;
		//! @brief Get the inputs from the Window
		void getEvents();
		//! @brief Constructor that return the window component of the SFML.
		//! @param height height of the window.
		//! @param width width of the window.
		//! @param maxFPS the number of maximum FPS for the window.
		SFRenderer(unsigned int height, unsigned int width, int maxFPS);
		SFRenderer(const SFRenderer &) = delete;
		SFRenderer &operator=(const SFRenderer &) = delete;
		~SFRenderer() = default;
	};
}

#endif //COMSQUARE_SFRENDERER_HPP
