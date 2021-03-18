cmake_minimum_required(VERSION 3.17)
project(parallel_processing)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(parallel_processing lab1/main.cpp)
