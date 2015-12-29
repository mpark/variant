cmake_minimum_required(VERSION 2.8)

project(variant CXX)

include(CheckCXXCompilerFlag)

macro(add_compiler_flag testname flag)
  check_cxx_compiler_flag(${flag} ${testname})
  if (${testname})
    add_compile_options(${flag})
  endif()
endmacro()

add_compiler_flag(STD_CXX14 -std=c++14)

include_directories(include)

add_subdirectory(3rdparty/googletest/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
enable_testing()

add_subdirectory(test)
