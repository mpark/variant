# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_MPARK_BENCHMARKS)
  include(ExternalProject)
  ExternalProject_Add(mpark
    GIT_REPOSITORY https://github.com/mpark/variant.git
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/mpark"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Add(mpark-dev
    GIT_REPOSITORY https://github.com/mpark/variant.git
    GIT_TAG dev
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/mpark-dev"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Add(mpark-v1.3.0
    GIT_REPOSITORY https://github.com/mpark/variant.git
    GIT_TAG v1.3.0
    GIT_SHALLOW 1
    TIMEOUT 120
    SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies/mpark-v1.3.0"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND ""     # Disable build step
    INSTALL_COMMAND ""   # Disable install step
    TEST_COMMAND ""      # Disable test step
    UPDATE_COMMAND ""    # Disable source work-tree update
  )

  ExternalProject_Get_Property(mpark SOURCE_DIR)
  set(mpark_INCLUDE_DIRS ${SOURCE_DIR}/include)

  ExternalProject_Get_Property(mpark-dev SOURCE_DIR)
  set(mpark_dev_INCLUDE_DIRS ${SOURCE_DIR}/include)

  ExternalProject_Get_Property(mpark-v1.3.0 SOURCE_DIR)
  set(mpark_v1_3_0_INCLUDE_DIRS ${SOURCE_DIR}/include)

  function(mpark_add_compile_benchmark dataset repeat range)
    metabench_add_dataset(${dataset} compile.mpark.cpp.erb ${range}
                          NAME mpark ENV "{repeat: ${repeat}}")
    target_include_directories(${dataset} PUBLIC ${mpark_INCLUDE_DIRS})
    add_dependencies(${dataset} mpark)

    metabench_add_dataset(${dataset}-dev compile.mpark.cpp.erb ${range}
                          NAME mpark-dev ENV "{repeat: ${repeat}}")
    target_include_directories(${dataset}-dev PUBLIC ${mpark_dev_INCLUDE_DIRS})
    add_dependencies(${dataset}-dev mpark-dev)

    metabench_add_dataset(${dataset}-v1.3.0 compile.mpark.cpp.erb ${range}
                          NAME mpark-v1.3.0 ENV "{repeat: ${repeat}}")
    target_include_directories(${dataset}-v1.3.0 PUBLIC ${mpark_v1_3_0_INCLUDE_DIRS})
    add_dependencies(${dataset}-v1.3.0 mpark-v1.3.0)
  endfunction()

  function(mpark_add_execute_benchmark benchmark)
    add_executable(${benchmark} execute.mpark.cpp)
    target_include_directories(${benchmark} PUBLIC ${mpark_INCLUDE_DIRS})
    target_link_libraries(${benchmark} PUBLIC benchmark_main)
    add_test(${benchmark} ${benchmark} --benchmark_color=true)
    add_dependencies(${benchmark} mpark)

    add_executable(${benchmark}-dev execute.mpark.cpp)
    target_include_directories(${benchmark}-dev PUBLIC ${mpark_dev_INCLUDE_DIRS})
    target_link_libraries(${benchmark}-dev PUBLIC benchmark_main)
    add_test(${benchmark}-dev ${benchmark}-dev --benchmark_color=true)
    add_dependencies(${benchmark}-dev mpark-dev)

    add_executable(${benchmark}-v1.3.0 execute.mpark.cpp)
    target_include_directories(${benchmark}-v1.3.0 PUBLIC ${mpark_v1_3_0_INCLUDE_DIRS})
    target_link_libraries(${benchmark}-v1.3.0 PUBLIC benchmark_main)
    add_test(${benchmark}-v1.3.0 ${benchmark}-v1.3.0 --benchmark_color=true)
    add_dependencies(${benchmark}-v1.3.0 mpark-v1.3.0)
  endfunction()
else()
  function(mpark_add_compile_benchmark)
  endfunction()

  function(mpark_add_execute_benchmark)
  endfunction()
endif()
