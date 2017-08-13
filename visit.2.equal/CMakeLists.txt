# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(test "visit.2.equal")
set(title "Double visitation (Equal types)")
set(xlabel "Number of alternatives")

set(datasets)

add_dataset(datasets anthonyw ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets boost    ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets eggs     ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets juice    ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets mapbox   ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets mpark    ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets pdimov   ${test} "[1] + (5..20).step(5).to_a")
add_dataset(datasets std      ${test} "[1] + (5..20).step(5).to_a")

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
