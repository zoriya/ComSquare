//
// Created by cbihan on 1/30/20.
//

#include "SFRenderer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics//RenderWindow.hpp>

namespace ComSquare::Renderer
{
	void SFRenderer::setWindowName(std::string newWindowName)
	{
		this->window.setTitle(newWindowName);
	}

	void SFRenderer::drawScreen()
	{
		this->texture.update(this->pixelBuffer);
		this->sprite.setTexture(this->texture, false);
		this->renderer.draw(this->sprite);
	}

	void SFRenderer::putPixel(int x, int y, uint8_t rgba)
	{
		this->pixelBuffer[this->videoMode.width * x + y] = rgba;
	}

	SFRenderer::SFRenderer(unsigned int height, unsigned int width, int maxFPS)
	{
		this->shouldExit = false;
		this->videoMode = {static_cast<unsigned int>(width), static_cast<unsigned int>(height), 32};
		// note 32 is the BitsPerPixel.
		this->pixelBuffer = new sf::Uint8[height * width * 4];
		// note the size of the buffer is multiplied by 4 due to rgba values
		this->window.create(this->videoMode, "ComSquare Emulator", sf::Style::Default);
		this->window.setFramerateLimit(maxFPS);
		this->texture.create(width, height);
	}

}
