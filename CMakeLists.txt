cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)

# set the project name
project(ComSquare)

# show compilation warnings
add_compile_options(-W -Wall -Wextra -Wshadow)

# make unit tests
add_executable(unit_tests sources/hello.cpp tests/hello.cpp)

# include criterion & coverage
target_link_libraries(unit_tests criterion -lgcov)
target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage)

# make app
add_executable(ComSquare main.cpp sources/hello.cpp sources/Memory/MemoryBus.cpp sources/Memory/MemoryBus.hpp sources/Memory/IMemory.hpp sources/Memory/IMemory.cpp)
