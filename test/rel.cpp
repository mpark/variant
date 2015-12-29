// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

TEST(Rel, SameTypeSameValue) {
  std_exp::variant<int, std::string> v(0), w(0);
  // v op w
  EXPECT_TRUE(v == w);
  EXPECT_FALSE(v != w);
  EXPECT_FALSE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_TRUE(v >= w);
  // w op v
  EXPECT_TRUE(w == v);
  EXPECT_FALSE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_FALSE(w > v);
  EXPECT_TRUE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(0), w(0);
    // v op w
    static_assert(v == w, "");
    static_assert(!(v != w), "");
    static_assert(!(v < w), "");
    static_assert(!(v > w), "");
    static_assert(v <= w, "");
    static_assert(v >= w, "");
    // w op v
    static_assert(w == v, "");
    static_assert(!(w != v), "");
    static_assert(!(w < v), "");
    static_assert(!(w > v), "");
    static_assert(w <= v, "");
    static_assert(w >= v, "");
  }
}

TEST(Rel, SameTypeDiffValue) {
  std_exp::variant<int, std::string> v(0), w(1);
  // v op w
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // w op v
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(0), w(1);
    // v op w
    static_assert(!(v == w), "");
    static_assert(v != w, "");
    static_assert(v < w, "");
    static_assert(!(v > w), "");
    static_assert(v <= w, "");
    static_assert(!(v >= w), "");
    // w op v
    static_assert(!(w == v), "");
    static_assert(w != v, "");
    static_assert(!(w < v), "");
    static_assert(w > v, "");
    static_assert(!(w <= v), "");
    static_assert(w >= v, "");
  }
}

TEST(Rel, DiffTypeSameValue) {
  std_exp::variant<int, unsigned int> v(0), w(0u);
  // v op w
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // w op v
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr std_exp::variant<int, unsigned int> v(0), w(0u);
    // v op w
    static_assert(!(v == w), "");
    static_assert(v != w, "");
    static_assert(v < w, "");
    static_assert(!(v > w), "");
    static_assert(v <= w, "");
    static_assert(!(v >= w), "");
    // w op v
    static_assert(!(w == v), "");
    static_assert(w != v, "");
    static_assert(!(w < v), "");
    static_assert(w > v, "");
    static_assert(!(w <= v), "");
    static_assert(w >= v, "");
  }
}

TEST(Rel, DiffTypeDiffValue) {
  std_exp::variant<int, unsigned int> v(0), w(1u);
  // v op w
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // w op v
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */  {
    constexpr std_exp::variant<int, unsigned int> v(0), w(1u);
    // v op w
    static_assert(!(v == w), "");
    static_assert(v != w, "");
    static_assert(v < w, "");
    static_assert(!(v > w), "");
    static_assert(v <= w, "");
    static_assert(!(v >= w), "");
    // w op v
    static_assert(!(w == v), "");
    static_assert(w != v, "");
    static_assert(!(w < v), "");
    static_assert(w > v, "");
    static_assert(!(w <= v), "");
    static_assert(w >= v, "");
  }
}
