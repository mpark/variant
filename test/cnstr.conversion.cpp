// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_Conversion, Direct) {
  std_exp::variant<int, std::string> v(42);
  EXPECT_EQ(42, std_exp::get<int>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(42);
    static_assert(42 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_Conversion, DirectRef) {
  int expected = 42;
  std_exp::variant<int &, std::string &> v(expected);
  EXPECT_EQ(expected, std_exp::get<int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<int &>(v));

  /* constexpr */ {
    static constexpr int expected = 42;
    constexpr std_exp::variant<const int &> v(expected);
    static_assert(expected == std_exp::get<const int &>(v), "");
    static_assert(&expected == &std_exp::get<const int &>(v), "");
  }
}

TEST(Cnstr_Conversion, DirectConversion) {
  std_exp::variant<int, std::string> v("42");
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(1.1);
    static_assert(1 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_Conversion, DirectConversionRef) {
  std::ostringstream strm;
  std_exp::variant<int &, std::ostream &> v(strm);
  EXPECT_EQ(&strm, &std_exp::get<std::ostream &>(v));
}

TEST(Cnstr_Conversion, CopyInitialization) {
  std_exp::variant<int, std::string> v = 42;
  EXPECT_EQ(42, std_exp::get<int>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v = 42;
    static_assert(42 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_Conversion, CopyInitializationRef) {
  std::string expected = "42";
  std_exp::variant<int &, std::string &> v = expected;
  EXPECT_EQ(expected, std_exp::get<std::string &>(v));
  EXPECT_EQ(&expected, &std_exp::get<std::string &>(v));
}

TEST(Cnstr_Conversion, CopyInitializationConversion) {
  std_exp::variant<int, std::string> v = "42";
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v = 1.1;
    static_assert(1 == std_exp::get<int>(v), "");
  }
}

TEST(Cnstr_Conversion, CopyInitializationConversionRef) {
  std::ostringstream strm;
  std_exp::variant<int &, std::ostream &> v = strm;
  EXPECT_EQ(&strm, &std_exp::get<std::ostream &>(v));
}
