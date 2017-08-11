# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(test "ctor.fwd.vars.N.alts.1")
set(title "N variants, 1 alternative")
set(xlabel "Number of variants")

set(datasets)

add_dataset(
  datasets anthonyw ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets boost ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets eggs ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets juice ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets mapbox ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets mpark ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets pdimov ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

add_dataset(
  datasets std ${test}
  "[1] + (10..50).step(10).to_a + (100..250).step(50).to_a")

# Add a compile-time chart
metabench_add_chart(
  benchmark.compile_time.${test}
  ALL
  ASPECT COMPILATION_TIME
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT compile_time.html
)

# Add a binary size chart
metabench_add_chart(
  benchmark.binary_size.${test}
  ALL
  ASPECT EXECUTABLE_SIZE
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT binary_size.html
)
