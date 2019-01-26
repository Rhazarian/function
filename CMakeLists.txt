cmake_minimum_required(VERSION 3.12)
project(function)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES function.h function_testing.cpp gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc function_testing.cpp)
add_executable(function ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")