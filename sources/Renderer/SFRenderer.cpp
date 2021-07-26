//
// Created by cbihan on 1/30/20.
//

#include "SNES.hpp"
#include "SFRenderer.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

namespace ComSquare::Renderer
{
	SFRenderer::SFRenderer(unsigned int height, unsigned int width)
		: _videoMode({width, height, 32}),
		  _pixelBuffer(new sf::Color[height * width])
	{
		this->_texture.create(width, height);
		this->_sprite.setTexture(this->_texture);
		this->_sound.setBuffer(this->_soundBuffer);
	}

	void SFRenderer::createWindow(SNES &snes, int maxFPS)
	{
		sf::Image icon;
		this->_renderWindow.create(this->_videoMode, "ComSquare Emulator", sf::Style::Default);
		if (icon.loadFromFile("resources/Logo.png"))
			this->_renderWindow.setIcon(314, 314, icon.getPixelsPtr());
		this->_renderWindow.setFramerateLimit(maxFPS);
		this->setWindowName(snes.cartridge.header.gameName);

		while (!this->shouldExit) {
			snes.update();
			this->getEvents();
		}
	}

	SFRenderer::~SFRenderer()
	{
		delete[] this->_pixelBuffer;
	}

	void SFRenderer::setWindowName(std::string &newWindowName)
	{
		this->_renderWindow.setTitle(newWindowName + " - ComSquare");
	}

	void SFRenderer::drawScreen()
	{
		this->_texture.update(reinterpret_cast<sf::Uint8 *>(this->_pixelBuffer));
		this->_sprite.setTexture(this->_texture);
		this->_renderWindow.draw(this->_sprite);
		this->_renderWindow.display();
	}

	void SFRenderer::playAudio(std::span<int16_t> samples)
	{
		this->_soundBuffer.loadFromSamples(samples.data(), samples.size(), 2, 32040);
		this->_sound.play();
	}

	void SFRenderer::putPixel(unsigned verticalPosition, unsigned horizontalPosition, uint32_t rgba)
	{
		if (horizontalPosition >= this->_videoMode.width)
			throw InvalidPixelPosition("Width", horizontalPosition, this->_videoMode.width);
		if (verticalPosition >= this->_videoMode.height)
			throw InvalidPixelPosition("Height", verticalPosition, this->_videoMode.height);

		this->_pixelBuffer[this->_videoMode.width * verticalPosition + horizontalPosition] = sf::Color(rgba);
	}

	void SFRenderer::getEvents()
	{
		sf::Event event{};
		while (this->_renderWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->shouldExit = true;
				break;
			}
		}
	}

	void SFRenderer::setSize(unsigned int height, unsigned int width)
	{
		this->_renderWindow.setSize({width, height});
		sf::FloatRect visibleArea(0, 0, width, height);
		this->_renderWindow.setView(sf::View(visibleArea));
		this->_videoMode.width = width;
		this->_videoMode.height = height;
		if (!this->_texture.create(width, height)) {
			throw std::runtime_error("sfml texture resize failed");
		}
		this->_sprite.setTexture(this->_texture);
		delete[] this->_pixelBuffer;
		this->_pixelBuffer = new sf::Color[height * width];
	}

	void SFRenderer::setScale(float height, float width)
	{
		this->_sprite.setScale(width, height);
	}
}
