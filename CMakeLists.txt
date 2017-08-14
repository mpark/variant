# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.6.3)

project(MPark.Variant VERSION 1.2.0 LANGUAGES CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-std=c++1z)
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
  add_compile_options(-pedantic)
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-ftemplate-depth=5000 HAS_FTEMPLATE_DEPTH)
if(HAS_FTEMPLATE_DEPTH)
  add_compile_options(-ftemplate-depth=5000)
endif()

# Option.
set(MPARK_VARIANT_INCLUDE_BENCHMARKS "" CACHE STRING
    "Semicolon-separated list of benchmarks to build.")

list(REMOVE_DUPLICATES MPARK_VARIANT_INCLUDE_BENCHMARKS)

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "anthonyw" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_ANTHONYW_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "boost" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_BOOST_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "eggs" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_EGGS_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "juice" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_JUICE_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "mapbox" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_MAPBOX_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "mpark" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_MPARK_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "pdimov" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_PDIMOV_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(FIND MPARK_VARIANT_INCLUDE_BENCHMARKS "std" MPARK_VARIANT_INDEX)
if(NOT MPARK_VARIANT_INDEX EQUAL -1)
  set(MPARK_VARIANT_INCLUDE_STD_BENCHMARKS ON)
  list(REMOVE_AT MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_INDEX)
endif()

list(LENGTH MPARK_VARIANT_INCLUDE_BENCHMARKS MPARK_VARIANT_LENGTH)
if(MPARK_VARIANT_LENGTH GREATER 0)
  message(FATAL_ERROR
    "The following libraries in `MPARK_VARIANT_INCLUDE_BENCHMARKS`"
    "are not supported: ${MPARK_VARIANT_INCLUDE_BENCHMARKS}")
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

macro(add_dataset datasets lib name repeat range)
  set(dataset "${lib}.${name}")

  if ("${lib}" STREQUAL "anthonyw")
    anthonyw_add_dataset(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "boost")
    boost_add_dataset(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "eggs")
    eggs_add_dataset(${dataset} ${repeat} ${range})
  elseif ("${lib}" STREQUAL "juice")
    juice_add_dataset(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "mapbox")
    mapbox_add_dataset(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "mpark")
    mpark_add_dataset(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "pdimov")
    pdimov_add_dataset(${dataset} ${repeat} ${range})
  elseif("${lib}" STREQUAL "std")
    std_add_dataset(${dataset} ${repeat} ${range})
  else()
    message(FATAL_ERROR "The library '${lib}' is currently not supported.")
  endif()

  if (TARGET ${dataset})
    list(APPEND ${datasets} ${dataset})
  endif()
endmacro()

add_subdirectory(ctor.fwd)
add_subdirectory(visit.1)
add_subdirectory(visit.2.equal)
add_subdirectory(visit.2.mixed)
