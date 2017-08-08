# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_PDIMOV_BENCHMARKS)
  include(ExternalProject)

  ExternalProject_Add(mp11
    GIT_REPOSITORY https://github.com/boostorg/mp11.git
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/mp11"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Add(pdimov
    GIT_REPOSITORY https://github.com/pdimov/variant2.git
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/pdimov"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Get_Property(mp11 SOURCE_DIR)
  set(mp11_INCLUDE_DIRS ${SOURCE_DIR}/include)

  ExternalProject_Get_Property(pdimov SOURCE_DIR)
  set(pdimov_INCLUDE_DIRS ${SOURCE_DIR}/include)

  function(pdimov_add_dataset dataset range)
    metabench_add_dataset(${dataset} pdimov.cpp.erb ${range} NAME pdimov)
    target_include_directories(${dataset} PUBLIC ${mp11_INCLUDE_DIRS}
                                                 ${pdimov_INCLUDE_DIRS})
    add_dependencies(${dataset} mp11 pdimov)
  endfunction()
else()
  function(pdimov_add_dataset)
  endfunction()
endif()
