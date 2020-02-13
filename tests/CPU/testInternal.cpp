//
// Created by anonymus-raccoon on 2/13/20.
//

#include <criterion/criterion.h>
#include <iostream>
#include <bitset>
#include "../tests.hpp"
#include "../../sources/SNES.hpp"
#include "../../sources/Memory/MemoryBus.hpp"
using namespace ComSquare;

Test(SEP, setall)
{
	auto pair = Init();
	pair.second.cpu.SEP()
}