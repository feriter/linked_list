cmake_minimum_required(VERSION 3.15)
project(Task1)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(gtest)
include_directories(gtest/googletest/include)

add_executable(Task1 main.cpp linked_list.h)