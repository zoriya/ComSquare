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
		sf::Sprite sprite;
		sf::Image image;
		image.loadFromMemory(this->pixelBuffer, sizeof(sf::Uint8 *) * this->videoMode.height * this->videoMode.width * 4);

		//image.LoadFromPixels(800, 600, this->pixelBuffer);
		//sprite.SetImage(image);
		//window.Draw(sprite);
		//window.Display();
	}

	void SFRenderer::putPixel(int x, int y, uint8_t rgba)
	{
		(void) x;
		(void) y;
		(void) rgba;
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
	}

}
