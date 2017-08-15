# MPark.Variant
#
# Copyright Michael Park, 2015-2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

set(name "ctor.fwd")
set(repeat 50)

set(title "Forwarding constructor (${repeat}x)")
set(xlabel "Number of alternatives")

set(datasets)

add_dataset(
  datasets anthonyw ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets boost    ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a")

add_dataset(
  datasets eggs     ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets juice    ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets mapbox   ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets mpark    ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets pdimov   ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

add_dataset(
  datasets std      ${name} ${repeat}
  "[1] + (5..20).step(5).to_a + (30..50).step(10).to_a + (75..100).step(25).to_a")

# Add a compile-time chart
metabench_add_chart(
  benchmark.compile_time.${name}
  ALL
  ASPECT COMPILATION_TIME
  TITLE ${title}
  XLABEL ${xlabel}
  DATASETS ${datasets}
  OUTPUT compile_time.html
)

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
