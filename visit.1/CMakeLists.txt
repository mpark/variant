# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

set(name "visit.1")
set(repeat 10)

set(title "Single visitation (${repeat}x)")
set(xlabel "Number of alternatives")

set(range "[1] + (5...30).step(5).to_a + (30..60).step(10).to_a")

set(datasets)
add_compile_benchmark(datasets anthonyw ${name} ${repeat} ${range})
add_compile_benchmark(datasets boost    ${name} ${repeat}
                      "[1] + (5...30).step(5).to_a + (30..50).step(10).to_a")
add_compile_benchmark(datasets eggs     ${name} ${repeat} ${range})
add_compile_benchmark(datasets juice    ${name} ${repeat} ${range})
add_compile_benchmark(datasets mapbox   ${name} ${repeat} ${range})
add_compile_benchmark(datasets mpark    ${name} ${repeat} ${range})
add_compile_benchmark(datasets pdimov   ${name} ${repeat} ${range})
add_compile_benchmark(datasets std      ${name} ${repeat} ${range})

# Add a binary size chart
metabench_add_chart(
  chart.binary_size.${name}
  ALL
  ASPECT EXECUTABLE_SIZE
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT binary_size.html
)

# Add a compile time chart
metabench_add_chart(
  chart.compile_time.${name}
  ALL
  ASPECT COMPILATION_TIME
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT compile_time.html
)

add_execute_benchmark(anthonyw ${name})
add_execute_benchmark(boost ${name})
add_execute_benchmark(eggs ${name})
add_execute_benchmark(juice ${name})
add_execute_benchmark(mapbox ${name})
add_execute_benchmark(mpark ${name})
add_execute_benchmark(pdimov ${name})
add_execute_benchmark(std ${name})
