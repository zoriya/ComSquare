//
// Created by Melefo on 05/02/2020.
//

#ifndef COMSQUARE_TESTS_HPP
#define COMSQUARE_TESTS_HPP

#define private public
#define protected public
#define class struct

#include "../sources/Memory/MemoryBus.hpp"
std::pair<std::shared_ptr<ComSquare::Memory::MemoryBus>, ComSquare::SNES> Init();

#endif //COMSQUARE_TESTS_HPP
