cmake_minimum_required(VERSION 3.17)
project(TestSampleCode)

set(CMAKE_CXX_STANDARD 14)

add_executable(TestSampleCode main.cpp EVRP.cpp EVRP.hpp heuristic.cpp heuristic.hpp stats.cpp stats.hpp randomHeuristic.cpp randomHeuristic.h greedyEuclideanHeuristic.cpp greedyEuclideanHeuristic.h ACOHeuristic.cpp ACOHeuristic.h AStarHeuristic.cpp AStarHeuristic.h DijkstrasHeuristic.cpp DijkstrasHeuristic.h ACOv1.cpp ACOv1.h Randoms.cpp ACO.cpp ACO.h)