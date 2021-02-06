//
// Created by Zoe Roux on 2/6/21.
//

#pragma once

#include "../CPUDebug.hpp"
#include <fstream>

namespace ComSquare::Debugger
{
	//! @brief Class to parse WLA-DX symbol files.
	class WlaDx {
	public:
		static std::vector<Label> parse(std::fstream symbolFile);
	};
}