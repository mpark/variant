# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_BOOST_BENCHMARKS)
  find_package(Boost 1.68.0 REQUIRED)

  function(boost_add_compile_benchmark dataset repeat range)
    metabench_add_dataset(${dataset} compile.boost.cpp.erb ${range}
                          NAME boost ENV "{repeat: ${repeat}}")
    target_link_libraries(${dataset} PUBLIC Boost::boost)
  endfunction()

  function(boost_add_execute_benchmark benchmark)
    add_executable(${benchmark} execute.boost.cpp)
    target_include_directories(${benchmark} PUBLIC ${boost_INCLUDE_DIRS})
    target_link_libraries(${benchmark} PUBLIC Boost::boost benchmark_main)
    add_test(${benchmark} ${benchmark} --benchmark_color=true)
  endfunction()
else()
  function(boost_add_compile_benchmark)
  endfunction()

  function(boost_add_execute_benchmark)
  endfunction()
endif()
