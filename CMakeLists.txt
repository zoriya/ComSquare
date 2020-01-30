cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

# set the project name
project(ComSquare)

# show compilation warnings
add_compile_options(-W -Wall -Wextra -Wshadow)

# make unit tests
add_executable(unit_tests
        tests/testCPU.cpp
        sources/PPU/PPU.cpp
        sources/PPU/PPU.hpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/IMemory.hpp
        sources/Memory/IMemory.cpp
        sources/PPU/PPU.cpp
        sources/PPU/PPU.hpp
        sources/CPU/CPU.cpp
        sources/CPU/CPU.hpp
        sources/Cartridge/Cartridge.cpp
        sources/Cartridge/Cartridge.hpp
        sources/Exceptions/NotImplementedException.hpp
        sources/APU/APU.hpp
        sources/APU/APU.cpp
        sources/Exceptions/InvalidAddress.hpp
        sources/Exceptions/InvalidRom.hpp
        sources/Models/Ints.hpp
        sources/Models/Ints.hpp
        sources/Ram/Ram.cpp
        sources/Ram/Ram.hpp
        sources/Memory/MemoryShadow.cpp
        sources/Memory/MemoryShadow.hpp
        sources/Memory/IRectangleMemory.cpp
        sources/Memory/IRectangleMemory.hpp
        sources/DSP/DSP.cpp
        sources/DSP/DSP.hpp
)

# include criterion & coverage
target_link_libraries(unit_tests criterion -lgcov)
target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage)

# make app
add_executable(ComSquare
        main.cpp
        sources/SNES.cpp
        sources/SNES.hpp
        sources/Memory/MemoryBus.cpp
        sources/Memory/MemoryBus.hpp
        sources/Memory/IMemory.hpp
        sources/Memory/IMemory.cpp
        sources/PPU/PPU.cpp
        sources/PPU/PPU.hpp
        sources/CPU/CPU.cpp
        sources/CPU/CPU.hpp
        sources/Cartridge/Cartridge.cpp
        sources/Cartridge/Cartridge.hpp
        sources/Exceptions/NotImplementedException.hpp
        sources/APU/APU.hpp
        sources/APU/APU.cpp
        sources/Exceptions/InvalidAddress.hpp
        sources/Exceptions/InvalidRom.hpp
        sources/Models/Ints.hpp
        sources/Models/Ints.hpp
        sources/Ram/Ram.cpp
        sources/Ram/Ram.hpp
        sources/Memory/MemoryShadow.cpp
        sources/Memory/MemoryShadow.hpp
        sources/Memory/IRectangleMemory.cpp
        sources/Memory/IRectangleMemory.hpp
        sources/DSP/DSP.cpp
        sources/DSP/DSP.hpp
        sources/Renderer/IRenderer.hpp
        sources/Renderer/SFRenderer.hpp
        sources/Renderer/SFRenderer.cpp
        sources/Exceptions/InvalidAcction.hpp
)
