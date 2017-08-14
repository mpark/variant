# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_JUICE_BENCHMARKS)
  include(ExternalProject)
  ExternalProject_Add(juice
    GIT_REPOSITORY https://github.com/jarro2783/thenewcpp.git
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/juice"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Get_Property(juice SOURCE_DIR)
  set(juice_INCLUDE_DIRS ${SOURCE_DIR})

  function(juice_add_dataset dataset repeat range)
    metabench_add_dataset(${dataset} juice.cpp.erb ${range}
                          NAME juice ENV "{repeat: ${repeat}}")
    target_include_directories(${dataset} PUBLIC ${juice_INCLUDE_DIRS})
    add_dependencies(${dataset} juice)
  endfunction()
else()
  function(juice_add_dataset)
  endfunction()
endif()
