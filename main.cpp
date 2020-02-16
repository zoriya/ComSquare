//
// Created by anonymus-raccoon on 1/24/20.
//

#include <iostream>
#include <string>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include "sources/SNES.hpp"
#include "sources/Renderer/SFRenderer.hpp"
#include "sources/Renderer/QtRenderer/QtSFML.hpp"

using namespace ComSquare;

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "ComSquare:" << std::endl << "\tUsage: " << argv[0] << " rom_path" << std::endl;
		return 1;
	}
	QApplication app(argc, argv);
	Renderer::QtSFML renderer(600, 800);
	SNES snes(std::make_shared<Memory::MemoryBus>(), argv[1], renderer);
	renderer.createWindow(snes, 60);
	snes.enableCPUDebugging();
	return QApplication::exec();
}