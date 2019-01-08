# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.6.3)

project(MPark.Variant/Benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING
    "The C++ standard with which to build the benchmarks.")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
  add_compile_options(-pedantic-errors)
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-ftemplate-depth=5000 HAS_FTEMPLATE_DEPTH)
if(HAS_FTEMPLATE_DEPTH)
  add_compile_options(-ftemplate-depth=5000)
endif()

# Option.
set(MPARK_VARIANT_INCLUDE_LIBRARIES "" CACHE STRING
    "Semicolon-separated list of libraries to include.")

list(REMOVE_DUPLICATES MPARK_VARIANT_INCLUDE_LIBRARIES)

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "anthonyw" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_ANTHONYW_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "boost" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_BOOST_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "eggs" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_EGGS_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "juice" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_JUICE_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "mapbox" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_MAPBOX_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "mpark" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_MPARK_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "pdimov" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_PDIMOV_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_LIBRARIES "std" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_STD_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_INDEX)
endif()

list(LENGTH MPARK_VARIANT_INCLUDE_LIBRARIES MPARK_VARIANT_LENGTH)
if(MPARK_VARIANT_LENGTH GREATER 0)
  message(FATAL_ERROR
    "The following libraries in `MPARK_VARIANT_INCLUDE_LIBRARIES`"
    "are not supported: ${MPARK_VARIANT_INCLUDE_LIBRARIES}")
endif()

include(dependencies/anthonyw.cmake)
include(dependencies/boost.cmake)
include(dependencies/eggs.cmake)
include(dependencies/juice.cmake)
include(dependencies/mapbox.cmake)
include(dependencies/mpark.cmake)
include(dependencies/pdimov.cmake)
include(dependencies/std.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdparty/metabench)
include(metabench)

macro(add_compile_benchmark datasets lib name repeat range)
  set(dataset "compile.${name}.${lib}")

  if ("${lib}" STREQUAL "anthonyw")
    anthonyw_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "boost")
    boost_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "eggs")
    eggs_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "juice")
    juice_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "mapbox")
    mapbox_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "mpark")
    mpark_add_compile_benchmark(${dataset} ${repeat} ${range})
    if (TARGET ${dataset}-dev)
      list(APPEND ${datasets} ${dataset}-dev)
    endif()
    if (TARGET ${dataset}-v1.3.0)
      list(APPEND ${datasets} ${dataset}-v1.3.0)
    endif()
  elseif("${lib}" STREQUAL "pdimov")
    pdimov_add_compile_benchmark(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "std")
    std_add_compile_benchmark(${dataset} ${repeat} ${range})
  else()
    message(FATAL_ERROR "The library '${lib}' is currently not supported.")
  endif()

  if (TARGET ${dataset})
    list(APPEND ${datasets} ${dataset})
  endif()
endmacro()

set(BENCHMARK_ENABLE_TESTING CACHE BOOL OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES CACHE BOOL ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/benchmark
                 ${CMAKE_BINARY_DIR}/3rdparty/benchmark)

macro(add_execute_benchmark lib name)
  set(benchmark "execute.${name}.${lib}")
  if ("${lib}" STREQUAL "anthonyw")
    anthonyw_add_execute_benchmark(${benchmark})
  elseif ("${lib}" STREQUAL "boost")
    boost_add_execute_benchmark(${benchmark})
  elseif ("${lib}" STREQUAL "eggs")
    eggs_add_execute_benchmark(${benchmark})
  elseif ("${lib}" STREQUAL "juice")
    juice_add_execute_benchmark(${benchmark})
  elseif("${lib}" STREQUAL "mapbox")
    mapbox_add_execute_benchmark(${benchmark})
  elseif("${lib}" STREQUAL "mpark")
    mpark_add_execute_benchmark(${benchmark})
  elseif("${lib}" STREQUAL "pdimov")
    pdimov_add_execute_benchmark(${benchmark})
  elseif("${lib}" STREQUAL "std")
    std_add_execute_benchmark(${benchmark})
  else()
    message(FATAL_ERROR "The library '${lib}' is currently not supported.")
  endif()
endmacro()

enable_testing()

add_subdirectory(ctor.fwd)
add_subdirectory(visit.1)
add_subdirectory(visit.2.equal)
add_subdirectory(visit.2.mixed)
