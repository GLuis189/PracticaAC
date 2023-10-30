cmake_minimum_required(VERSION 3.26)
project(PracticaAC)

set(CMAKE_CXX_STANDARD 14)

include_directories(sim)

add_executable(PracticaAC
        fluid/fluid.cpp
        sim/block.cpp
        sim/block.hpp
        sim/grid.cpp
        sim/grid.hpp
        sim/progargs.cpp
        sim/progargs.hpp
        utest/block_test.cpp
        utest/grid_test.cpp
        utest/progargs_test.cpp)
