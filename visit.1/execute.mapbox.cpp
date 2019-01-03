// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <benchmark/benchmark.h>

#include <cstddef>
#include <type_traits>
#include <vector>

#include <mapbox/variant.hpp>

#include "../lib.hpp"

template <std::size_t I>
using size_constant = std::integral_constant<std::size_t, I>;

template <std::size_t... Is>
using variants = std::vector<mapbox::util::variant<size_constant<Is>...>>;

template <std::size_t... Is>
variants<Is...> make_variants(lib::index_sequence<Is...>) {
  variants<Is...> vs;
  for (std::size_t i = 0; i < 5000 / sizeof...(Is); ++i) {
    int dummy[] = {(vs.push_back(size_constant<Is>{}), 0)...};
    (void)dummy;
  }
  return vs;
}

struct visitor {
  template <std::size_t I>
  constexpr std::size_t operator()(size_constant<I>) const {
    return I;
  }
};

template <std::size_t N>
static void visit1(benchmark::State &state) {
  auto vs = make_variants(lib::make_index_sequence<N>{});
  for (auto _ : state) {
    int sum = 0;
    for (const auto &v : vs) {
      sum += v.match(visitor{});
    }
    benchmark::DoNotOptimize(sum);
  }
}

BENCHMARK_TEMPLATE(visit1, 1);
BENCHMARK_TEMPLATE(visit1, 2);
BENCHMARK_TEMPLATE(visit1, 3);
BENCHMARK_TEMPLATE(visit1, 5);
BENCHMARK_TEMPLATE(visit1, 8);
BENCHMARK_TEMPLATE(visit1, 15);
BENCHMARK_TEMPLATE(visit1, 32);
BENCHMARK_TEMPLATE(visit1, 65);
BENCHMARK_TEMPLATE(visit1, 128);
