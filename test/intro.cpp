// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <iostream>
#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Variant, Intro) {
  // direct initialization.
  exp::variant<int, std::string> v("hello world!"s);

  // direct access via reference.
  EXPECT_EQ("hello world!"s, exp::get<std::string>(v));

  // bad access.
  EXPECT_THROW(exp::get<int>(v), exp::bad_variant_access);

  // copy construction.
  exp::variant<int, std::string> w(v);

  // direct access via pointer.
  EXPECT_FALSE(exp::get_if<int>(&w));
  EXPECT_TRUE(exp::get_if<std::string>(&w));

  // diff-type assignment.
  v = 42;

  struct unary {
    int operator()(int) const { return 0; }
    int operator()(const std::string &) const { return 1; }
  };  // unary

  // single visitation.
  EXPECT_EQ(0, exp::visit(unary{}, v));

  // same-type assignment.
  w = "hello"s;

  EXPECT_NE(v, w);

  // make `w` equal to `v`.
  w = 42;

  EXPECT_EQ(v, w);

  struct binary {
    int operator()(int, int) const { return 0; }
    int operator()(int, const std::string &) const { return 1; }
    int operator()(const std::string &, int) const { return 2; }
    int operator()(const std::string &, const std::string &) const { return 3; }
  };  // binary

  // binary visitation.
  EXPECT_EQ(0, exp::visit(binary{}, v, w));
}
