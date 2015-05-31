cmake_minimum_required(VERSION 3.0)
project(CI5652_MDP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(mdp LocalSearch.cpp)
add_executable(mdp2 Problem.cpp Problem.h main.h main.cpp)