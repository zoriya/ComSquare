//
// Created by anonymus-raccoon on 2/5/20.
//

#include "NoRenderer.hpp"

namespace ComSquare::Renderer
{
	void NoRenderer::setWindowName(std::string newWindowName)
	{
		(void)newWindowName;
	}

	void NoRenderer::drawScreen() { }

	void NoRenderer::putPixel(unsigned y, unsigned x, uint32_t rgba)
	{
		(void)x;
		(void)y;
		(void)rgba;
	}

	void NoRenderer::getEvents() { }

	NoRenderer::NoRenderer(unsigned int height, unsigned int width, int maxFPS)
	{
		(void)height;
		(void)width;
		(void)maxFPS;
	}

	void NoRenderer::createWindow(SNES &snes, int maxFPS)
	{
		(void)snes;
		(void)maxFPS;
	}
}