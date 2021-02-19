cmake_minimum_required(VERSION 3.16)
project(GeneticAlgorithmExample)

set(CMAKE_CXX_STANDARD 14)

add_executable(GeneticAlgorithmExample Chromosome.hpp Converter.hpp Evolution.hpp example.cpp Galgo.hpp GeneticAlgorithm.hpp Parameter.hpp Population.hpp Randomize.hpp)