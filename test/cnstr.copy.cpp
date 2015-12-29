// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_Copy, Value) {
  // `v`
  std_exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));
  // `w`
  std_exp::variant<int, std::string> w(v);
  EXPECT_EQ("hello"s, std_exp::get<std::string>(w));
  // Check `v`
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));

  /* constexpr */ {
    // `v`
    constexpr std_exp::variant<int, const char *> v(42);
    static_assert(42 == std_exp::get<int>(v), "");
    // `w`
    constexpr std_exp::variant<int, const char *> w(v);
    static_assert(42 == std_exp::get<int>(w), "");
  }
}

TEST(Cnstr_Copy, Ref) {
  std::string s = "hello"s;
  // `v`
  std_exp::variant<int &, std::string &> v(s);
  EXPECT_EQ("hello"s, std_exp::get<std::string &>(v));
  EXPECT_EQ(&s, &std_exp::get<std::string &>(v));
  // `w`
  std_exp::variant<int &, std::string &> w(v);
  EXPECT_EQ("hello"s, std_exp::get<std::string &>(w));
  EXPECT_EQ(&s, &std_exp::get<std::string &>(w));
  // Check `v`
  EXPECT_EQ("hello"s, std_exp::get<std::string &>(v));
  EXPECT_EQ(&s, &std_exp::get<std::string &>(v));
}
