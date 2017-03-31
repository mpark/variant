// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Rel, SameTypeSameValue) {
  mpark::variant<int, std::string> v(0), w(0);
  // `v` op `w`
  EXPECT_TRUE(v == w);
  EXPECT_FALSE(v != w);
  EXPECT_FALSE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_TRUE(v >= w);
  // `w` op `v`
  EXPECT_TRUE(w == v);
  EXPECT_FALSE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_FALSE(w > v);
  EXPECT_TRUE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> cv(0), cw(0);
    // `cv` op `cw`
    static_assert(cv == cw, "");
    static_assert(!(cv != cw), "");
    static_assert(!(cv < cw), "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(cv >= cw, "");
    // `cw` op `cv`
    static_assert(cw == cv, "");
    static_assert(!(cw != cv), "");
    static_assert(!(cw < cv), "");
    static_assert(!(cw > cv), "");
    static_assert(cw <= cv, "");
    static_assert(cw >= cv, "");
  }
}

TEST(Rel, SameTypeDiffValue) {
  mpark::variant<int, std::string> v(0), w(1);
  // `v` op `w`
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // `w` op `v`
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> cv(0), cw(1);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
  }
}

TEST(Rel, DiffTypeSameValue) {
  mpark::variant<int, unsigned int> v(0), w(0u);
  // `v` op `w`
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // `w` op `v`
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */ {
    constexpr mpark::variant<int, unsigned int> cv(0), cw(0u);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
  }
}

TEST(Rel, DiffTypeDiffValue) {
  mpark::variant<int, unsigned int> v(0), w(1u);
  // `v` op `w`
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_TRUE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_FALSE(v >= w);
  // `w` op `v`
  EXPECT_FALSE(w == v);
  EXPECT_TRUE(w != v);
  EXPECT_FALSE(w < v);
  EXPECT_TRUE(w > v);
  EXPECT_FALSE(w <= v);
  EXPECT_TRUE(w >= v);

  /* constexpr */  {
    constexpr mpark::variant<int, unsigned int> cv(0), cw(1u);
    // `cv` op `cw`
    static_assert(!(cv == cw), "");
    static_assert(cv != cw, "");
    static_assert(cv < cw, "");
    static_assert(!(cv > cw), "");
    static_assert(cv <= cw, "");
    static_assert(!(cv >= cw), "");
    // `cw` op `cv`
    static_assert(!(cw == cv), "");
    static_assert(cw != cv, "");
    static_assert(!(cw < cv), "");
    static_assert(cw > cv, "");
    static_assert(!(cw <= cv), "");
    static_assert(cw >= cv, "");
  }
}

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  [[noreturn]] move_thrower_t(move_thrower_t &&) {
    throw std::runtime_error("");
  }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) = default;
  friend bool operator<(const move_thrower_t &, const move_thrower_t &) {
    return false;
  }
  friend bool operator>(const move_thrower_t &, const move_thrower_t &) {
    return false;
  }
  friend bool operator<=(const move_thrower_t &, const move_thrower_t &) {
    return true;
  }
  friend bool operator>=(const move_thrower_t &, const move_thrower_t &) {
    return true;
  }
  friend bool operator==(const move_thrower_t &, const move_thrower_t &) {
    return true;
  }
  friend bool operator!=(const move_thrower_t &, const move_thrower_t &) {
    return false;
  }
};  // move_thrower_t

TEST(Rel, OneValuelessByException) {
  // `v` normal, `w` corrupted.
  mpark::variant<int, move_thrower_t> v(42), w(42);
  EXPECT_THROW(w = move_thrower_t{}, std::runtime_error);
  EXPECT_FALSE(v.valueless_by_exception());
  EXPECT_TRUE(w.valueless_by_exception());
  // `v` op `w`
  EXPECT_FALSE(v == w);
  EXPECT_TRUE(v != w);
  EXPECT_FALSE(v < w);
  EXPECT_TRUE(v > w);
  EXPECT_FALSE(v <= w);
  EXPECT_TRUE(v >= w);
}

TEST(Rel, BothValuelessByException) {
  // `v`, `w` both corrupted.
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  mpark::variant<int, move_thrower_t> w(v);
  EXPECT_TRUE(v.valueless_by_exception());
  EXPECT_TRUE(w.valueless_by_exception());
  // `v` op `w`
  EXPECT_TRUE(v == w);
  EXPECT_FALSE(v != w);
  EXPECT_FALSE(v < w);
  EXPECT_FALSE(v > w);
  EXPECT_TRUE(v <= w);
  EXPECT_TRUE(v >= w);
}
