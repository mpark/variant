# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

if (MPARK_VARIANT_INCLUDE_STD_BENCHMARKS)
  function(std_add_dataset dataset range)
    metabench_add_dataset(${dataset} std.cpp.erb ${range} NAME std)
  endfunction()
else()
  function(std_add_dataset)
  endfunction()
endif()
