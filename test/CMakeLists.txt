cmake_minimum_required(VERSION 3.6.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)

macro(add_compiler_flag testname flag)
  check_cxx_compiler_flag(${flag} ${testname})
  if(${testname})
    add_compile_options(${flag})
  endif()
endmacro()

add_compiler_flag(WALL -Wall)
add_compiler_flag(WEXTRA -Wextra)
add_compiler_flag(WERROR -Werror)

if(MPARK_VARIANT_INCLUDE_MPARK_TESTS)

  set(MPARK_VARIANT_GOOGLETEST_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
  set(MPARK_VARIANT_GOOGLETEST_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest)
  set(MPARK_VARIANT_GOOGLETEST_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)

  configure_file(
    googletest-download/CMakeLists.txt.in
    ${MPARK_VARIANT_GOOGLETEST_DOWNLOAD_DIR}/CMakeLists.txt)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY ${MPARK_VARIANT_GOOGLETEST_DOWNLOAD_DIR})

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${MPARK_VARIANT_GOOGLETEST_DOWNLOAD_DIR})

  add_subdirectory(${MPARK_VARIANT_GOOGLETEST_SOURCE_DIR} ${MPARK_VARIANT_GOOGLETEST_BINARY_DIR})

  function(add_gtest name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} gtest gtest_main pthread)
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
    CMAKE_ARGS        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                      "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                      "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
    SOURCE_DIR        ${MPARK_VARIANT_LLVM_SOURCE_DIR}
    BINARY_DIR        ${MPARK_VARIANT_LLVM_BINARY_DIR}
    BUILD_COMMAND     make cxx cxxabi cxx_experimental
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
        env MPARK_VARIANT_LIT=${MPARK_VARIANT_LLVM_SOURCE_DIR}/utils/lit/lit.py
        ${CMAKE_CURRENT_SOURCE_DIR}/run_libcxx_tests.sh
  )

endif()
