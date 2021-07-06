//
// Created by Zoe Roux on 2021-07-06.
//


#pragma once

#include <iostream>

#ifdef TESTS
#include <catch2/catch.hpp>
#endif

namespace ComSquare
{
	enum LogLevel
	{
		INFO,
		WARNING,
		ERROR
	};

	constexpr LogLevel GlobalLevel = INFO;

#ifndef TESTS
#define log(level, message) \
	if constexpr((level) >= GlobalLevel) \
		std::cout << message << std::endl // NOLINT(bugprone-macro-parentheses)
#else
#define log(_, msg) INFO(msg)
#endif
}