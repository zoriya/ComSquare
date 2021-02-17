//
// Created by Zoe Roux on 2/6/21.
//

#include <regex>
#include "WlaDx.hpp"

namespace ComSquare::Debugger
{
	std::vector<Label> WlaDx::parse(std::fstream symbolFile)
	{
		std::vector<Label> labels;
		std::string line;

		while (std::getline(symbolFile, line)) {
			line = line.substr(0, line.rfind(';'));
			std::smatch match;
			std::regex re("\\[\\S+\\]");

			if (!std::regex_search(line, match, re)
		}
		return labels;
	}
}


