// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Assign_Copy, SameType_Value) {
  // `v`, `w`.
  exp::variant<int, std::string> v("hello"s), w("world"s);
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  EXPECT_EQ("world"s, exp::get<std::string>(w));
  // copy assignment.
  v = w;
  EXPECT_EQ("world"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("world"s, exp::get<std::string>(w));
}

TEST(Assign_Copy, DiffType) {
  // `v`, `w`.
  exp::variant<int, std::string> v(42), w("world"s);
  EXPECT_EQ(42, exp::get<int>(v));
  EXPECT_EQ("world"s, exp::get<std::string>(w));
  // copy assignment.
  v = w;
  EXPECT_EQ("world"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("world"s, exp::get<std::string>(w));
}
