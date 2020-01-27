//
// Created by anonymus-raccoon on 1/24/20.
//

#include <iostream>
#include <string>
#include "sources/SNES.hpp"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "ComSquare:" << std::endl << "\tUsage: " << argv[0] << "rom_path" << std::endl;
		return 1;
	}
	ComSquare::MemoryBus bus;
	ComSquare::SNES snes(std::make_shared<ComSquare::MemoryBus>(bus), std::string(argv[1]));
    return 0;
}