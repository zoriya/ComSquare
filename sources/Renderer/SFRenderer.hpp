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
#include <SFML/Graphics/RenderWindow.hpp>

namespace ComSquare::Renderer
{
	class InvalidPixelPosition : public std::exception {
	private:
		std::string _msg;
	public:
		explicit InvalidPixelPosition(const std::string &name, unsigned int x, unsigned int width)
			: _msg("Trying to place a pixel at an invalid " + name + " (" + std::to_string(x) + ">=" + std::to_string(width) + ")") {}
		[[nodiscard]] const char *what() const noexcept override { return this->_msg.c_str(); }
	};

	class SFRenderer : public IRenderer {
	protected:
		//! @brief The Renderer for the window.
		sf::RenderWindow _renderWindow;
		//! @brief Video Mode containing the _height and _width of the window.
		sf::VideoMode _videoMode;
		//! @brief The image that contain all of the pixels
		sf::Color *_pixelBuffer;
		//! @brief The sprite to render the array of pixels
		sf::Sprite _sprite;
		//! @brief The texture to render the array of pixels
		sf::Texture _texture;

		//! @brief The buffer containing samples to be played
		sf::SoundBuffer _soundBuffer;
		//! @brief the sound played
		sf::Sound _sound;
	public:
		//! @brief Tells to the program if the window has been closed, and therefore if he should stop
		bool shouldExit = false;
		//! @brief Set a new name to the window, if there is already a name it will be overwrite.
		//! @param newWindowName new title for the window.
		void setWindowName(std::string &newWindowName) override;
		//! @brief Update the screen by printing the buffer.
		void drawScreen() override;
		//! @brief Set a size or resize the Renderer drawing size
		//! @param height The new height of the renderer in pixels
		//! @param width The new width of the renderer in pixels
		void setSize(unsigned width, unsigned height) override;
		//! @brief Add a pixel to the buffer to the coordinates x, y with the color rgba.
		//! @param horizontalPosition horizontal index.
		//! @param verticalPosition vertical index.
		//! @param rgba The color of the pixel.
		void putPixel(unsigned verticalPosition, unsigned horizontalPosition, uint32_t rgba) override;
		//! @brief Playing all samples from buffer
		//! @param samples Buffer containing samples
		//! @param sampleCount number of samples inside buffer
		void playAudio(std::span<int16_t> samples) override;
		//! @brief Get the inputs from the Window
		void getEvents();
		//! @brief Use this function to create the window.
		//! @param snes The snes for the update (not implemented here).
		//! @param maxFPS The number of FPS you aim to run on.
		void createWindow(SNES &snes, int maxFPS) override;
		//! @brief Constructor that return the window component of the SFML.
		//! @param height _height of the window.
		//! @param width _width of the window.
		SFRenderer(unsigned int height, unsigned int width);
		SFRenderer(const SFRenderer &) = delete;
		SFRenderer &operator=(const SFRenderer &) = delete;
		~SFRenderer();
	};
}

#endif //COMSQUARE_SFRENDERER_HPP
