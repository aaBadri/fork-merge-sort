cmake_minimum_required(VERSION 3.6)
project(fork_merge_sort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
add_executable(fork_merge_sort ${SOURCE_FILES})