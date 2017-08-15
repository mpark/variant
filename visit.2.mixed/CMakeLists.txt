# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(test "visit.2.mixed")
set(repeat 1)

set(title "Double visitation - Mixed variants (${repeat}x)")
set(xlabel "Number of alternatives")

set(datasets)

add_dataset(datasets anthonyw ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets boost    ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets eggs     ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets juice    ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets mpark    ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets pdimov   ${test} ${repeat} "[1] + (5..25).step(5).to_a")
add_dataset(datasets std      ${test} ${repeat} "[1] + (5..25).step(5).to_a")

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
