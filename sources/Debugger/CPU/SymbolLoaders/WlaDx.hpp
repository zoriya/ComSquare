//
// Created by Zoe Roux on 2/6/21.
//

#pragma once

#include "Debugger/CPU/CPUDebug.hpp"
#include <fstream>

namespace ComSquare::Debugger
{
	//! @brief Class to parse WLA-DX symbol files.
	class WlaDx {
	public:
		//! @brief Parse the whole file
		//! @param symbolFile The file to parse (opened but not seeked)
		//! @return The list of symbol found.
		static std::vector<Label> parse(std::ifstream &symbolFile);
		//! @brief Parse labels name & addresses (the [labels] section)
		//! @param labels An list of labels. If one is already defined, the address will be set.
		//! @param symbolFile The file to parse, opened and seeked to the line just after the [labels] line.
		//! @return The line after the label section (the first line that doesn't match the section's regex).
		static std::string _parseLabels(std::map<std::string, Label> &labels, std::ifstream &symbolFile);
		//! @brief Parse definitions (and size of labels) (the [definitions] section)
		//! @param labels An list of labels. If one is already defined, the size will be set.
		//! @param symbolFile The file to parse, opened and seeked to the line just after the [definitions] line.
		//! @return The line after the definitions section (the first line that doesn't match the section's regex).
		static std::string _parseDefinitions(std::map<std::string, Label> &labels, std::ifstream &symbolFile);
		//! @brief Remove comments, indentation & trailing spaces from a line.
		//! @param line The line to clean
		//! @return The line cleaned up.
		static std::string _cleanLine(std::string line);
	};
}