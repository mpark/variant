# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

include(ExternalProject)
ExternalProject_Add(anthonyw
  HG_REPOSITORY https://bitbucket.org/anthonyw/variant
  TIMEOUT 120
  SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/anthonyw"
  CONFIGURE_COMMAND "" # Disable configure step
  BUILD_COMMAND ""     # Disable build step
  INSTALL_COMMAND ""   # Disable install step
  TEST_COMMAND ""      # Disable test step
  UPDATE_COMMAND ""    # Disable source work-tree update
)

ExternalProject_Get_Property(anthonyw SOURCE_DIR)
set(anthonyw_variant_INCLUDE_DIRS ${SOURCE_DIR})

function(anthonyw_add_dataset dataset range)
  metabench_add_dataset(${dataset} anthonyw.cpp.erb ${range} NAME anthonyw)
  target_include_directories(${dataset} PUBLIC ${anthonyw_variant_INCLUDE_DIRS})
  add_dependencies(${dataset} anthonyw)
endfunction()
