// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Assign_Copy, SameType) {
  struct Obj {
    constexpr Obj() {}
    Obj(const Obj &) noexcept { EXPECT_TRUE(false); }
    Obj(Obj &&) = default;
    Obj &operator=(const Obj &) noexcept { EXPECT_TRUE(true); return *this; }
    Obj &operator=(Obj &&) = delete;
  };
  // `v`, `w`.
  mpark::variant<Obj, int> v, w;
  // copy assignment.
  v = w;
}

TEST(Assign_Copy, DiffType) {
  struct Obj {
    constexpr Obj() {}
    Obj(const Obj &) noexcept { EXPECT_TRUE(true); }
    Obj(Obj &&) = default;
    Obj &operator=(const Obj &) noexcept { EXPECT_TRUE(false); return *this; }
    Obj &operator=(Obj &&) = delete;
  };
  // `v`, `w`.
  mpark::variant<Obj, int> v(42), w;
  // move assignment.
  v = w;
}

TEST(Assign_Copy, ValuelessByException) {
  struct move_thrower_t {
    constexpr move_thrower_t() {}
    move_thrower_t(const move_thrower_t &) = default;
    move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
    move_thrower_t &operator=(const move_thrower_t &) = default;
    move_thrower_t &operator=(move_thrower_t &&) = default;
  };  // move_thrower_t
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.valueless_by_exception());
  mpark::variant<int, move_thrower_t> w(42);
  w = v;
  EXPECT_TRUE(w.valueless_by_exception());
}
