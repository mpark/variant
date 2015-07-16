cmake_minimum_required(VERSION 2.8)

project(variant)

include_directories(include)

add_subdirectory(3rdparty/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
enable_testing()

add_subdirectory(test)
