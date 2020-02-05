//
// Created by anonymus-raccoon on 1/24/20.
//

#include <iostream>
#include <string>
#include "sources/Renderer/IRenderer.hpp"
#include "sources/SNES.hpp"
#include "sources/Renderer/SFRenderer.hpp"

using namespace ComSquare;

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "ComSquare:" << std::endl << "\tUsage: " << argv[0] << " rom_path" << std::endl;
		return 1;
	}
	Memory::MemoryBus bus;
	Renderer::SFRenderer renderer(600, 800, 60);
	SNES snes(std::make_shared<Memory::MemoryBus>(bus), argv[1], renderer);
	bus.mapComponents(snes);
	int incx = 0;
	int incy = 0;
	uint32_t pixel = 0x000000FF;

	while (!renderer.shouldExit) {
		renderer.putPixel(incy, incx++, pixel);
		if (incx >= 800) {
			incx = 0;
			incy++;
		}
		if (incy >= 600) {
			incy = 0;
		}
		if (incx == 0) {
			renderer.drawScreen();
			pixel += 0xFF00FF00;
		}
		renderer.getEvents();
	}

	return 0;
}