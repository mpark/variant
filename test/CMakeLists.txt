# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.6.3)

set(MPARK_VARIANT_CXX_STANDARD 14 CACHE STRING "C++ Standard")

set(CMAKE_CXX_STANDARD ${MPARK_VARIANT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MPARK_VARIANT_INCLUDE_MPARK_TESTS)

  function(add_gtest name)
    add_executable(${name} ${name}.cpp)
    set_target_properties(${name} PROPERTIES COMPILE_FLAGS "${cxx_strict}")
    target_link_libraries(${name} gtest_main mpark_variant)
    add_test(${name} ${name} --gtest_color=yes)
  endfunction(add_gtest)

  add_gtest(access.get)
  add_gtest(access.get_if)
  add_gtest(access.holds_alternative)
  add_gtest(assign.copy)
  add_gtest(assign.conversion)
  add_gtest(assign.emplace)
  add_gtest(assign.move)
  add_gtest(cnstr.copy)
  add_gtest(cnstr.conversion)
  add_gtest(cnstr.default)
  add_gtest(cnstr.in_place)
  add_gtest(cnstr.move)
  add_gtest(dtor)
  add_gtest(hash)
  add_gtest(intro)
  add_gtest(rel)
  add_gtest(swap)
  add_gtest(visit)

endif()

if(MPARK_VARIANT_INCLUDE_LIBCXX_TESTS)

  include(ExternalProject)

  set(MPARK_VARIANT_LLVM_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/llvm)
  set(MPARK_VARIANT_LLVM_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/llvm-build)

  ExternalProject_Add(llvm
    GIT_REPOSITORY    http://llvm.org/git/llvm.git
    GIT_SHALLOW       1
    CMAKE_ARGS        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                      "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
    SOURCE_DIR        ${MPARK_VARIANT_LLVM_SOURCE_DIR}
    BINARY_DIR        ${MPARK_VARIANT_LLVM_BINARY_DIR}
    BUILD_COMMAND     ${CMAKE_COMMAND} --build . --target cxx
          COMMAND     ${CMAKE_COMMAND} --build . --target cxxabi
          COMMAND     ${CMAKE_COMMAND} --build . --target cxx_experimental
    INSTALL_COMMAND   ""  # Disable install step
    STEP_TARGETS      download
  )

  set(MPARK_VARIANT_LIBCXX_SOURCE_DIR ${MPARK_VARIANT_LLVM_SOURCE_DIR}/projects/libcxx)
  set(MPARK_VARIANT_LIBCXX_BINARY_DIR ${MPARK_VARIANT_LLVM_BINARY_DIR}/projects/libcxx)

  ExternalProject_Add(libcxx-download
    GIT_REPOSITORY    http://llvm.org/git/libcxx.git
    GIT_SHALLOW       1
    DEPENDS           llvm-download
    SOURCE_DIR        ${MPARK_VARIANT_LIBCXX_SOURCE_DIR}
    BINARY_DIR        ${MPARK_VARIANT_LIBCXX_BINARY_DIR}
    CONFIGURE_COMMAND ""  # Disable configure step
    BUILD_COMMAND     ""  # Disable build step
    INSTALL_COMMAND   ""  # Disable install step
  )

  set(MPARK_VARIANT_LIBCXXABI_SOURCE_DIR ${MPARK_VARIANT_LLVM_SOURCE_DIR}/projects/libcxxabi)
  set(MPARK_VARIANT_LIBCXXABI_BINARY_DIR ${MPARK_VARIANT_LLVM_BINARY_DIR}/projects/libcxxabi)

  ExternalProject_Add(libcxxabi-download
    GIT_REPOSITORY    http://llvm.org/git/libcxxabi.git
    GIT_SHALLOW       1
    DEPENDS           llvm-download
    SOURCE_DIR        ${MPARK_VARIANT_LIBCXXABI_SOURCE_DIR}
    BINARY_DIR        ${MPARK_VARIANT_LIBCXXABI_BINARY_DIR}
    CONFIGURE_COMMAND ""  # Disable configure step
    BUILD_COMMAND     ""  # Disable build step
    INSTALL_COMMAND   ""  # Disable install step
  )

  ExternalProject_Add_StepDependencies(llvm configure libcxx-download libcxxabi-download)

  add_test(run_libcxx_tests
    ${CMAKE_COMMAND} -E
        env MPARK_VARIANT_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        env MPARK_VARIANT_SOURCE_DIR=${CMAKE_SOURCE_DIR}
        env MPARK_VARIANT_LIBCXX_SOURCE_DIR=${MPARK_VARIANT_LIBCXX_SOURCE_DIR}
        env MPARK_VARIANT_LIBCXX_SITE_CONFIG=${MPARK_VARIANT_LIBCXX_BINARY_DIR}/test/lit.site.cfg
        env MPARK_VARIANT_LIT=${MPARK_VARIANT_LLVM_BINARY_DIR}/bin/llvm-lit
        ${CMAKE_CURRENT_SOURCE_DIR}/run_libcxx_tests.sh
  )

endif()
