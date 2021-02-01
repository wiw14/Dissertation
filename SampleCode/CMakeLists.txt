cmake_minimum_required(VERSION 3.17)
project(TestSampleCode)

set(CMAKE_CXX_STANDARD 14)

add_executable(TestSampleCode main.cpp EVRP.cpp EVRP.hpp heuristic.cpp heuristic.hpp stats.cpp stats.hpp)