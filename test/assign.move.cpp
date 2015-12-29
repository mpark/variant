// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

TEST(Assign_Move, SameType) {
  struct Obj {
    Obj() = default;
    Obj(const Obj &) = delete;
    Obj(Obj &&) noexcept { EXPECT_TRUE(false); }
    Obj &operator=(const Obj &) = delete;
    Obj &operator=(Obj &&) noexcept { EXPECT_TRUE(true); return *this; }
  };
  // `v`, `w`.
  std_exp::variant<Obj, int> v, w;
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
  std_exp::variant<Obj, int> v(42), w;
  // move assignment.
  v = std::move(w);
}
