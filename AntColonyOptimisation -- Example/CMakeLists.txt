cmake_minimum_required(VERSION 3.17)
project(AntColonyOptimisation)

set(CMAKE_CXX_STANDARD 14)

add_executable(AntColonyOptimisation main.cpp ACO.cpp ACO.h Randoms.cpp)