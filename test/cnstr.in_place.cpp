// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_InPlace, IndexDirect) {
  exp::variant<int, std::string> v(exp::in_place_index<0>, 42);
  EXPECT_EQ(42, exp::get<0>(v));
}

TEST(Cnstr_InPlace, IndexDirectDuplicate) {
  exp::variant<int, int> v(exp::in_place_index<0>, 42);
  EXPECT_EQ(42, exp::get<0>(v));
}

TEST(Cnstr_InPlace, IndexConversion) {
  exp::variant<int, std::string> v(exp::in_place_index<1>, "42");
  EXPECT_EQ("42"s, exp::get<1>(v));
}

TEST(Cnstr_InPlace, IndexConversionDuplicateRef) {
  std::ostringstream strm;
  exp::variant<std::ostream &, std::ostream &> v(exp::in_place_index<0>, strm);
  EXPECT_EQ(&strm, &exp::get<0>(v));
}

TEST(Cnstr_InPlace, IndexInitializerList) {
  exp::variant<int, std::string> v(exp::in_place_index<1>, {'4', '2'});
  EXPECT_EQ("42"s, exp::get<1>(v));
}

TEST(Cnstr_InPlace, TypeDirect) {
  exp::variant<int, std::string> v(exp::in_place_type<std::string>, "42"s);
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}

TEST(Cnstr_InPlace, TypeDirectRef) {
  int expected = 42;
  exp::variant<int &, std::ostream &> v(exp::in_place_type<int &>, expected);
  auto &actual = exp::get<int &>(v);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(Cnstr_InPlace, TypeConversion) {
  exp::variant<int, std::string> v(exp::in_place_type<int>, 42.5);
  EXPECT_EQ(42, exp::get<int>(v));
}

TEST(Cnstr_InPlace, TypeConversionRef) {
  std::ostringstream strm;
  exp::variant<int, std::ostream &> v(exp::in_place_type<std::ostream &>, strm);
  EXPECT_EQ(&strm, &exp::get<std::ostream &>(v));
}

TEST(Cnstr_InPlace, TypeInitializerList) {
  exp::variant<int, std::string> v(exp::in_place_type<std::string>, {'4', '2'});
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}
