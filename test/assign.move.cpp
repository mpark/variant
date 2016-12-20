// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Assign_Move, SameType) {
  struct Obj {
    Obj() = default;
    Obj(const Obj &) = delete;
    Obj(Obj &&) noexcept { EXPECT_TRUE(false); }
    Obj &operator=(const Obj &) = delete;
    Obj &operator=(Obj &&) noexcept { EXPECT_TRUE(true); return *this; }
  };
  // `v`, `w`.
  mpark::variant<Obj, int> v, w;
  // move assignment.
  v = std::move(w);
}

TEST(Assign_Move, DiffType) {
  struct Obj {
    Obj() = default;
    Obj(const Obj &) = delete;
    Obj(Obj &&) noexcept { EXPECT_TRUE(true); }
    Obj &operator=(const Obj &) = delete;
    Obj &operator=(Obj &&) noexcept { EXPECT_TRUE(false); return *this; }
  };
  // `v`, `w`.
  mpark::variant<Obj, int> v(42), w;
  // move assignment.
  v = std::move(w);
}

TEST(Assign_Copy, ValuelessByException) {
  struct move_thrower_t {
    move_thrower_t() = default;
    move_thrower_t(const move_thrower_t &) = default;
    move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
    move_thrower_t &operator=(const move_thrower_t &) = default;
    move_thrower_t &operator=(move_thrower_t &&) = default;
  };  // move_thrower_t
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.valueless_by_exception());
  mpark::variant<int, move_thrower_t> w(42);
  w = std::move(v);
  EXPECT_TRUE(w.valueless_by_exception());
}
