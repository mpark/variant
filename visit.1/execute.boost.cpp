// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <benchmark/benchmark.h>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 50
#include <boost/variant.hpp>
#undef BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#undef BOOST_MPL_LIMIT_LIST_SIZE

template <int>
struct S {};

using V = boost::variant<
    S<0>, S<1>, S<2>, S<3>, S<4>, S<5>, S<6>, S<7>, S<8>, S<9>,
    S<10>, S<11>, S<12>, S<13>, S<14>, S<15>, S<16>, S<17>, S<18>, S<19>,
    S<20>, S<21>, S<22>, S<23>, S<24>, S<25>, S<26>, S<27>, S<28>, S<29>,
    S<30>, S<31>, S<32>, S<33>, S<34>, S<35>, S<36>, S<37>, S<38>, S<39>>;

struct Vis {
  template <typename T>
  void operator()(T) const {}
};

static void BM_Visit1(benchmark::State& state) {
  V v0(S<0>{});
  V v1(S<10>{});
  V v2(S<20>{});
  V v3(S<30>{});
  for (auto _ : state) {
    boost::apply_visitor(Vis{}, v0);
    boost::apply_visitor(Vis{}, v1);
    boost::apply_visitor(Vis{}, v2);
    boost::apply_visitor(Vis{}, v3);
  }
}

BENCHMARK(BM_Visit1);
