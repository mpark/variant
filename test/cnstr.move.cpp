// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_Move, Value) {
  // `v`
  exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  // `w`
  exp::variant<int, std::string> w(std::move(v));
  EXPECT_EQ("hello"s, exp::get<std::string>(w));
  // Check `v`
  EXPECT_TRUE(exp::get<std::string>(v).empty());
}

TEST(Cnstr_Move, Ref) {
  std::string s = "hello"s;
  // `v`
  exp::variant<int &, std::string &> v(s);
  EXPECT_EQ("hello"s, exp::get<std::string &>(v));
  EXPECT_EQ(&s, &exp::get<std::string &>(v));
  // `w`
  exp::variant<int &, std::string &> w(std::move(v));
  EXPECT_EQ("hello"s, exp::get<std::string &>(w));
  EXPECT_EQ(&s, &exp::get<std::string &>(w));
  // Check `v`
  EXPECT_EQ("hello"s, exp::get<std::string &>(v));
  EXPECT_EQ(&s, &exp::get<std::string &>(v));
}
