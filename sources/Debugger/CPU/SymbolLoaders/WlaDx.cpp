//
// Created by Zoe Roux on 2/6/21.
//

#include <regex>
#include "Utility/Utility.hpp"
#include "WlaDx.hpp"

namespace ComSquare::Debugger
{
	std::vector<Label> WlaDx::parse(std::ifstream &symbolFile)
	{
		std::vector<Label> ret;
		std::map<std::string, Label> labels;
		std::string line;
		std::smatch match;
		std::regex re(R"(\[(\S+)\])");

		while (symbolFile) {
			if (line.empty()) {
				std::getline(symbolFile, line);
				line = WlaDx::_cleanLine(line);
			}
			if (!std::regex_match(line, match, re))
				continue;

			if (match[1] == "labels")
				line = WlaDx::_parseLabels(labels, symbolFile);
			else if (match[1] == "definitions")
				line = WlaDx::_parseDefinitions(labels, symbolFile);
		}
		std::transform(labels.begin(), labels.end(), std::back_inserter(ret), [](auto &i){return i.second;});
		return ret;
	}

	std::string WlaDx::_parseLabels(std::map<std::string, Label> &labels, std::ifstream &f)
	{
		std::string line;
		std::smatch match;
		std::regex re(R"(([0-9a-fA-F]{2}):([0-9a-fA-F]{4}) (.*))");

		while (std::getline(f, line)) {
			line = WlaDx::_cleanLine(line);
			if (!std::regex_match(line, match, re))
				return line;
			labels[match[3]].name = match[3];
			labels[match[3]].address = std::stoi(match[1], nullptr, 16) << 16
			                           | std::stoi(match[2], nullptr, 16);
		}
		return "";
	}

	std::string WlaDx::_parseDefinitions(std::map<std::string, Label> &labels, std::ifstream &f)
	{
		std::string line;
		std::smatch match;
		std::regex re(R"(([0-9a-fA-F]{8}) (.*))");
		std::string name;

		while (std::getline(f, line)) {
			line = WlaDx::_cleanLine(line);
			if (!std::regex_match(line, match, re))
				return line;
			// TODO support other definitions.
			if (!match[2].str().starts_with("_sizeof_"))
				continue;
			name = match[2].str().substr(std::string("_sizeof_").size());
			labels[name].size = std::stoi(match[1], nullptr, 16);
		}
		return "";
	}

	std::string WlaDx::_cleanLine(std::string line)
	{
		line = line.substr(0, line.rfind(';'));
		line.erase(0, line.find_first_not_of(Utility::WHITESPACES));
		line.erase(line.find_last_not_of(Utility::WHITESPACES) + 1);
		return line;
	}
}


