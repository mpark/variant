# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_PDIMOV_BENCHMARKS)
  find_package(Boost 1.68.0 REQUIRED)

  include(ExternalProject)

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

  ExternalProject_Get_Property(pdimov SOURCE_DIR)
  set(pdimov_INCLUDE_DIRS ${SOURCE_DIR}/include)

  function(pdimov_add_dataset dataset repeat range)
    metabench_add_dataset(${dataset} pdimov.cpp.erb ${range}
                          NAME pdimov ENV "{repeat: ${repeat}}")
    add_dependencies(${dataset} pdimov)
    target_include_directories(${dataset} PUBLIC ${pdimov_INCLUDE_DIRS})
    target_link_libraries(${dataset} PUBLIC Boost::boost)
  endfunction()
else()
  function(pdimov_add_dataset)
  endfunction()
endif()
