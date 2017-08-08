# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.6.3)

project(MPark.Variant VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
   "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
  add_compile_options(-pedantic)
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-ftemplate-depth=5000 HAS_FTEMPLATE_DEPTH)
if(HAS_FTEMPLATE_DEPTH)
  add_compile_options(-ftemplate-depth=5000)
endif()

include(dependencies/anthonyw.cmake)
include(dependencies/boost.cmake)
include(dependencies/eggs.cmake)
include(dependencies/mapbox.cmake)
include(dependencies/mpark.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdparty/metabench)
include(metabench)

macro(add_dataset datasets lib name range)
  set(dataset "${lib}.${name}")

  if ("${lib}" STREQUAL "anthonyw")
    anthonyw_add_dataset(${dataset} ${range})
  elseif ("${lib}" STREQUAL "boost")
    boost_add_dataset(${dataset} ${range})
  elseif ("${lib}" STREQUAL "eggs")
    eggs_add_dataset(${dataset} ${range})
  elseif("${lib}" STREQUAL "mapbox")
    mapbox_add_dataset(${dataset} ${range})
  elseif("${lib}" STREQUAL "mpark")
    mpark_add_dataset(${dataset} ${range})
  else()
    message(FATAL_ERROR "The library '${lib}' is currently not supported.")
  endif()

  if (TARGET ${dataset})
    list(APPEND ${datasets} ${dataset})
  endif()
endmacro()

add_subdirectory(ctor.fwd.alternatives)
add_subdirectory(ctor.fwd.variants)
