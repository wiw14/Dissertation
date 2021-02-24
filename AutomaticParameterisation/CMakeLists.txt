cmake_minimum_required(VERSION 3.17)
project(AutomaticParameterisation)

set(CMAKE_CXX_STANDARD 14)

add_executable(AutomaticParameterisation parameteriser.cpp ACO.cpp ACO.h LocalSearches.cpp LocalSearches.h EVRP.cpp EVRP.hpp heuristic.cpp heuristic.hpp ACOHeuristic.cpp ACOHeuristic.h stats.cpp stats.hpp)