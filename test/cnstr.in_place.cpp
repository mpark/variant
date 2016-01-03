// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_InPlace, IndexDirect) {
  std_exp::variant<int, std::string> v(std_exp::in_place_index<0>, 42);
  EXPECT_EQ(42, std_exp::get<0>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(std_exp::in_place_index<0>, 42);
    static_assert(42 == std_exp::get<0>(v), "");
  }
}

TEST(Cnstr_InPlace, IndexDirectDuplicate) {
  std_exp::variant<int, int> v(std_exp::in_place_index<0>, 42);
  EXPECT_EQ(42, std_exp::get<0>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, int> v(std_exp::in_place_index<0>, 42);
    static_assert(42 == std_exp::get<0>(v), "");
  }
}

TEST(Cnstr_InPlace, IndexConversion) {
  std_exp::variant<int, std::string> v(std_exp::in_place_index<1>, "42");
  EXPECT_EQ("42"s, std_exp::get<1>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(std_exp::in_place_index<0>, 1.1);
    static_assert(1 == std_exp::get<0>(v), "");
  }
}

TEST(Cnstr_InPlace, IndexConversionDuplicateRef) {
  std::ostringstream strm;
  std_exp::variant<std::ostream &, std::ostream &> v(std_exp::in_place_index<0>, strm);
  EXPECT_EQ(&strm, &std_exp::get<0>(v));
}

TEST(Cnstr_InPlace, IndexInitializerList) {
  std_exp::variant<int, std::string> v(std_exp::in_place_index<1>, {'4', '2'});
  EXPECT_EQ("42"s, std_exp::get<1>(v));
}

TEST(Cnstr_InPlace, TypeDirect) {
  std_exp::variant<int, std::string> v(std_exp::in_place_type<std::string>, "42"s);
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(std_exp::in_place_type<int>, 42);
    static_assert(42 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_InPlace, TypeDirectRef) {
  int expected = 42;
  std_exp::variant<int &, std::ostream &> v(std_exp::in_place_type<int &>, expected);
  EXPECT_EQ(expected, std_exp::get<int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<int &>(v));
}

TEST(Cnstr_InPlace, TypeConversion) {
  std_exp::variant<int, std::string> v(std_exp::in_place_type<int>, 42.5);
  EXPECT_EQ(42, std_exp::get<int>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(std_exp::in_place_type<int>, 42.5);
    static_assert(42 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_InPlace, TypeConversionRef) {
  std::ostringstream strm;
  std_exp::variant<int, std::ostream &> v(std_exp::in_place_type<std::ostream &>, strm);
  EXPECT_EQ(&strm, &std_exp::get<std::ostream &>(v));
}

TEST(Cnstr_InPlace, TypeInitializerList) {
  std_exp::variant<int, std::string> v(std_exp::in_place_type<std::string>, {'4', '2'});
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));
}
