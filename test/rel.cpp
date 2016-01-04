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

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) = default;
  friend bool operator< (const move_thrower_t &, const move_thrower_t &) { return false; }
  friend bool operator> (const move_thrower_t &, const move_thrower_t &) { return false; }
  friend bool operator<=(const move_thrower_t &, const move_thrower_t &) { return true; }
  friend bool operator>=(const move_thrower_t &, const move_thrower_t &) { return true; }
  friend bool operator==(const move_thrower_t &, const move_thrower_t &) { return true; }
  friend bool operator!=(const move_thrower_t &, const move_thrower_t &) { return false; }
};  // move_thrower_t

TEST(Rel, OneCorruptedByException) {
  // `v` normal, `w` corrupted.
  std_exp::variant<int, move_thrower_t> v(42), w(42);
  EXPECT_THROW(w = move_thrower_t{}, std::runtime_error);
  EXPECT_FALSE(v.corrupted_by_exception());
  EXPECT_TRUE(w.corrupted_by_exception());
  // v op w
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
}

TEST(Rel, BothCorruptedByException) {
  // `v`, `w` both corrupted.
  std_exp::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  std_exp::variant<int, move_thrower_t> w(v);
  EXPECT_TRUE(v.corrupted_by_exception());
  EXPECT_TRUE(w.corrupted_by_exception());
  // v op w
  EXPECT_TRUE(v == w);
  EXPECT_FALSE(v != w);
  EXPECT_FALSE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_TRUE(v >= w);
}
