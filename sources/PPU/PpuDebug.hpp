//
// Created by cbihan on 03/07/2021.
//

#pragma once

#include "Ram/Ram.hpp"

#define private public
#include "PPU.hpp"


namespace ComSquare::PPU::Utils::Debug
{
	void populateVram(std::shared_ptr<Ram::Ram> vram, int dumpNumber);
	void populateCgram(std::shared_ptr<Ram::Ram> cgram, int dumpNumber);

	void populateEnvironment(PPU &ppu, int dumpNumber);
}


