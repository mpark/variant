// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Swap, Same) {
  exp::variant<int, std::string> v("hello"s);
  exp::variant<int, std::string> w("world"s);
  // Check `v`.
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("world"s, exp::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ("world"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("hello"s, exp::get<std::string>(w));
}

TEST(Swap, Different) {
  exp::variant<int, std::string> v(42);
  exp::variant<int, std::string> w("hello"s);
  // Check `v`.
  EXPECT_EQ(42, exp::get<int>(v));
  // Check `w`.
  EXPECT_EQ("hello"s, exp::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(42, exp::get<int>(w));
}
