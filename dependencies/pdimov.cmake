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

  function(pdimov_add_compile_benchmark dataset repeat range)
    metabench_add_dataset(${dataset} compile.pdimov.cpp.erb ${range}
                          NAME pdimov ENV "{repeat: ${repeat}}")
    target_include_directories(${dataset} PUBLIC ${pdimov_INCLUDE_DIRS})
    target_link_libraries(${dataset} PUBLIC Boost::boost)
    add_dependencies(${dataset} pdimov)
  endfunction()

  function(pdimov_add_execute_benchmark benchmark)
    add_executable(${benchmark} execute.pdimov.cpp)
    target_include_directories(${benchmark} PUBLIC ${pdimov_INCLUDE_DIRS})
    target_link_libraries(${benchmark} PUBLIC Boost::boost benchmark_main)
    add_test(${benchmark} ${benchmark} --benchmark_color=true)
    add_dependencies(${benchmark} pdimov)
  endfunction()
else()
  function(pdimov_add_compile_benchmark)
  endfunction()

  function(pdimov_add_execute_benchmark)
  endfunction()
endif()
