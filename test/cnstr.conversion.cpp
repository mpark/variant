// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Cnstr_Conversion, Direct) {
  mpark::variant<int, std::string> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> v(42);
    static_assert(42 == mpark::get<int>(v), "");
  }
}

#if 0
TEST(Cnstr_Conversion, DirectRef) {
  int expected = 42;
  mpark::variant<int &, std::string &> v(expected);
  EXPECT_EQ(expected, mpark::get<int &>(v));
  EXPECT_EQ(&expected, &mpark::get<int &>(v));

  /* constexpr */ {
    static constexpr int expected = 42;
    constexpr mpark::variant<const int &> v(expected);
    static_assert(expected == mpark::get<const int &>(v), "");
    static_assert(&expected == &mpark::get<const int &>(v), "");
  }
}
#endif

TEST(Cnstr_Conversion, DirectConversion) {
  mpark::variant<int, std::string> v("42");
  EXPECT_EQ("42"s, mpark::get<std::string>(v));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> v(1.1);
    static_assert(1 == mpark::get<int>(v), "");
  }
}

#if 0
TEST(Cnstr_Conversion, DirectConversionRef) {
  std::ostringstream strm;
  mpark::variant<int &, std::ostream &> v(strm);
  EXPECT_EQ(&strm, &mpark::get<std::ostream &>(v));
}
#endif

TEST(Cnstr_Conversion, CopyInitialization) {
  mpark::variant<int, std::string> v = 42;
  EXPECT_EQ(42, mpark::get<int>(v));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> v = 42;
    static_assert(42 == mpark::get<int>(v), "");
  }
}

#if 0
TEST(Cnstr_Conversion, CopyInitializationRef) {
  std::string expected = "42";
  mpark::variant<int &, std::string &> v = expected;
  EXPECT_EQ(expected, mpark::get<std::string &>(v));
  EXPECT_EQ(&expected, &mpark::get<std::string &>(v));
}
#endif

TEST(Cnstr_Conversion, CopyInitializationConversion) {
  mpark::variant<int, std::string> v = "42";
  EXPECT_EQ("42"s, mpark::get<std::string>(v));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> v = 1.1;
    static_assert(1 == mpark::get<int>(v), "");
  }
}

#if 0
TEST(Cnstr_Conversion, CopyInitializationConversionRef) {
  std::ostringstream strm;
  mpark::variant<int &, std::ostream &> v = strm;
  EXPECT_EQ(&strm, &mpark::get<std::ostream &>(v));
}
#endif
