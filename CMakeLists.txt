cmake_minimum_required(VERSION 3.0)
project(CI5652_MDP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(mdp main.cpp main.h Problem.cpp Solution.cpp Types.h)