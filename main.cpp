//
// Created by anonymus-raccoon on 1/24/20.
//

#include <iostream>
#include <string>
#include "sources/SNES.hpp"
#include "sources/Renderer/SFRenderer.hpp"

using namespace ComSquare;

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "ComSquare:" << std::endl << "\tUsage: " << argv[0] << " rom_path" << std::endl;
		return 1;
	}
	try {
		Renderer::SFRenderer renderer(600, 800, 60);
		SNES snes(std::make_shared<Memory::MemoryBus>(), argv[1], renderer);
		while (!renderer.shouldExit) {
			unsigned cycleCount = snes.cpu->update();
			snes.ppu->update(cycleCount);
			snes.apu->update(cycleCount);

			renderer.getEvents();
		}
	} catch (std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
	}
	return 0;
}