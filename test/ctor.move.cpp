// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

#include "util.hpp"

namespace lib = mpark::lib;

TEST(Ctor_Move, Value) {
  // `v`
  mpark::variant<int, std::string> v("hello");
  EXPECT_EQ("hello", mpark::get<std::string>(v));
  // `w`
  mpark::variant<int, std::string> w(lib::move(v));
  EXPECT_EQ("hello", mpark::get<std::string>(w));

  /* constexpr */ {
    // `cv`
    constexpr mpark::variant<int, const char *> cv(42);
    static_assert(42 == mpark::get<int>(cv), "");
    // `cw`
    constexpr mpark::variant<int, const char *> cw(lib::move(cv));
    static_assert(42 == mpark::get<int>(cw), "");
  }
}

#ifdef MPARK_EXCEPTIONS
TEST(Ctor_Move, ValuelessByException) {
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, MoveConstruction);
  EXPECT_TRUE(v.valueless_by_exception());
  mpark::variant<int, move_thrower_t> w(lib::move(v));
  EXPECT_TRUE(w.valueless_by_exception());
}
#endif
