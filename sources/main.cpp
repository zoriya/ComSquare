//
// Created by anonymus-raccoon on 1/24/20.
//

#include <iostream>
#include <QtWidgets/QApplication>
#include <getopt.h>
#include "SNES.hpp"
#include "Renderer/QtRenderer/QtSFML.hpp"

using namespace ComSquare;

void usage(char *bin)
{
	std::cout << "ComSquare:" << std::endl
		<< "\tUsage: " << bin << " rom_path [options]" << std::endl
		<< "Options:" << std::endl
		<< "\t-h, --help:      \tDisplay this help message and exit." << std::endl
#ifdef DEBUGGER_ENABLED
		<< "\t-c, --cpu:       \tEnable the debugger of the CPU." << std::endl
		<< "\t-m, --memory:    \tEnable the memory viewer panel." << std::endl
		<< "\t-H, --header:    \tShow the header of the cartridge." << std::endl
		<< "\t-b, --bus:       \tShow the memory bus's log." << std::endl
		<< "\t-g, --cgram:     \tShow the palette viewer." << std::endl
		<< "\t-r, --registers: \tShow the registers viewer." << std::endl
#endif
	    ;
}

void parseArguments(int argc, char **argv, SNES &snes)
{
	int option_index = 0;
	static struct option long_options[] = {
		{"help",      no_argument, 0, 'h'},
#ifdef DEBUGGER_ENABLED
		{"cpu",       no_argument, 0, 'c'},
		{"apu",       no_argument, 0, 'a'},
		{"memory",    no_argument, 0, 'm'},
		{"header",    no_argument, 0, 'H'},
		{"bus",       no_argument, 0, 'b'},
		{"cgram",     no_argument, 0, 'g'},
		{"registers", no_argument, 0, 'r'},
#endif
		{0,           0,           0, 0}
	};
#ifdef DEBUGGER_ENABLED
	char short_options[] = "hcamHbgr";
#else
	char short_options[] = "h";
#endif

	while (true) {
		int c = getopt_long(argc, argv, short_options, long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			usage(argv[0]);
			exit(2);
		case 'h':
			usage(argv[0]);
			exit(0);
#ifdef DEBUGGER_ENABLED
		case 'c':
			snes.enableCPUDebugging();
			break;
//		case 'a':
//			snes.enableAPUDebugging();
//			break;
		case 'm':
			snes.enableRamViewer();
			break;
		case 'H':
			snes.enableHeaderViewer();
			break;
//		case 'b':
//			snes.enableMemoryBusDebugging();
//			break;
//		case 'g':
//			snes.enableCgramDebugging();
//			break;
		case 'r':
			snes.enableRegisterViewer();
			break;
#endif
		default:
			exit(2);
		}
	}

	if (optind != argc - 1) {
		usage(argv[0]);
		exit(2);
	}
	snes.loadRom(argv[optind]);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}
	QApplication app(argc, argv);
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
	Renderer::QtSFMLWindow renderer(1100, 1100);
	try {
		auto snes = std::make_unique<SNES>(renderer);
		parseArguments(argc, argv, *snes);
		renderer.createWindow(*snes, 60);
		return QApplication::exec();
	}
	catch(std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}