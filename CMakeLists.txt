cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

# set the project name
project(ComSquare)

# show compilation warnings
add_compile_options(-W -Wall -Wextra -Wshadow)

if (TEST)
    # make unit tests
    add_executable(unit_tests sources/hello.cpp tests/test_test.cpp)

    # include criterion & coverage
    target_link_libraries(unit_tests criterion -lgcov)
    target_compile_options(unit_tests PUBLIC -fprofile-arcs -ftest-coverage -g -O0)

else (TEST)
    # make app
    add_executable(ComSquare sources/main.cpp sources/hello.cpp)

endif (TEST)