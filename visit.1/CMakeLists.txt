# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(name "visit.1")
set(repeat 10)

set(title "Single visitation (${repeat}x)")
set(xlabel "Number of alternatives")

set(range "[1] + (5...30).step(5).to_a + (30..60).step(10).to_a")

set(datasets)
add_dataset(datasets anthonyw ${name} ${repeat} ${range})
add_dataset(datasets eggs     ${name} ${repeat} ${range})
add_dataset(datasets juice    ${name} ${repeat} ${range})
add_dataset(datasets mapbox   ${name} ${repeat} ${range})
add_dataset(datasets mpark    ${name} ${repeat} ${range})
add_dataset(datasets pdimov   ${name} ${repeat} ${range})
add_dataset(datasets std      ${name} ${repeat} ${range})
add_dataset(datasets boost    ${name} ${repeat}
            "[1] + (5...30).step(5).to_a + (30..50).step(10).to_a")

# Add a binary size chart
metabench_add_chart(
  benchmark.binary_size.${name}
  ALL
  ASPECT EXECUTABLE_SIZE
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT binary_size.html
)

# Add a compile time chart
metabench_add_chart(
  benchmark.compile_time.${name}
  ALL
  ASPECT COMPILATION_TIME
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT compile_time.html
)
