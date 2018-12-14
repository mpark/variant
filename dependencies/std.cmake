# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_STD_BENCHMARKS)
  function(std_add_compile_benchmark dataset repeat range)
    metabench_add_dataset(${dataset} compile.std.cpp.erb ${range}
                          NAME std ENV "{repeat: ${repeat}}")
  endfunction()

  function(std_add_execute_benchmark benchmark)
    add_executable(${benchmark} execute.std.cpp)
    target_link_libraries(${benchmark} PUBLIC benchmark_main)
    add_test(${benchmark} ${benchmark} --benchmark_color=true)
  endfunction()

else()
  function(std_add_compile_benchmark)
  endfunction()

  function(std_add_execute_benchmark)
  endfunction()
endif()
