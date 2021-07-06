//
// Created by anonymus-raccoon on 2/5/20.
//

#include "NoRenderer.hpp"

namespace ComSquare::Renderer
{
	void NoRenderer::setWindowName(std::string &)
	{}

	void NoRenderer::drawScreen()
	{}

	void NoRenderer::putPixel(unsigned, unsigned, uint32_t)
	{}

	void NoRenderer::playAudio(std::span<int16_t>)
	{}

	NoRenderer::NoRenderer(unsigned int, unsigned int, int)
	{}

	void NoRenderer::createWindow(SNES &, int)
	{}
}