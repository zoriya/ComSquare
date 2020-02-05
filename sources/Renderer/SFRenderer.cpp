//
// Created by cbihan on 1/30/20.
//

#include "SFRenderer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics//RenderWindow.hpp>
#include <iostream>

namespace ComSquare::Renderer
{
	void SFRenderer::setWindowName(std::string newWindowName)
	{
		this->window.setTitle(newWindowName + " - ComSquare");
	}

	void SFRenderer::drawScreen()
	{
		this->texture.update(reinterpret_cast<sf::Uint8 *>(this->pixelBuffer));
		this->sprite.setTexture(this->texture);
		this->window.draw(this->sprite);
		this->window.display();
	}

	void SFRenderer::putPixel(int y, int x, uint32_t rgba)
	{
		sf::Color pixels;
		pixels.r = rgba >> 24U;
		pixels.g = rgba >> 16U;
		pixels.b = rgba >> 8U;
		pixels.a = rgba >> 0U;
		this->pixelBuffer[this->videoMode.width * y + x] = pixels;
	}

	SFRenderer::SFRenderer(unsigned int height, unsigned int width, int maxFPS)
	{
		this->shouldExit = false;
		this->videoMode = {width, height, 32};
		this->window.create(this->videoMode, "ComSquare Emulator", sf::Style::Default);
		this->window.setFramerateLimit(maxFPS);
		this->texture.create(width, height);
		this->sprite.setTexture(this->texture);
		this->pixelBuffer = new sf::Color[height * width];
	}

	void SFRenderer::getEvents()
	{
		sf::Event event;
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->shouldExit = true;
				break;
			}
		}
	}

}
