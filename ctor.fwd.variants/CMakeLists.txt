# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(datasets)

add_dataset(
  datasets boost ctor.fwd.variants
  "[1] + (10..50).step(10).to_a + (100..500).step(50).to_a")

add_dataset(
  datasets eggs ctor.fwd.variants
  "[1] + (10..50).step(10).to_a + (100..500).step(50).to_a")

add_dataset(
  datasets mpark ctor.fwd.variants
  "[1] + (10..50).step(10).to_a + (100..500).step(50).to_a")

# Add a compile-time chart
metabench_add_chart(
  benchmark.compile_time.ctor.fwd.variants
  ALL
  ASPECT COMPILATION_TIME
  XLABEL "Number of variants"
  DATASETS ${datasets}
  OUTPUT compile_time.html
)

# Add a binary size chart
metabench_add_chart(
  benchmark.binary_size.ctor.fwd.variants
  ALL
  ASPECT EXECUTABLE_SIZE
  XLABEL "Number of variants"
  DATASETS ${datasets}
  OUTPUT binary_size.html
)
