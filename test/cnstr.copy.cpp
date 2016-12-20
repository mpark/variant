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

TEST(Cnstr_Copy, Value) {
  // `v`
  mpark::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // `w`
  mpark::variant<int, std::string> w(v);
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Check `v`
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));

  /* constexpr */ {
    // `v`
    constexpr mpark::variant<int, const char *> v(42);
    static_assert(42 == mpark::get<int>(v), "");
    // `w`
    constexpr mpark::variant<int, const char *> w(v);
    static_assert(42 == mpark::get<int>(w), "");
  }
}

#if 0
TEST(Cnstr_Copy, Ref) {
  std::string s = "hello"s;
  // `v`
  mpark::variant<int &, std::string &> v(s);
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  EXPECT_EQ(&s, &mpark::get<std::string &>(v));
  // `w`
  mpark::variant<int &, std::string &> w(v);
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
  EXPECT_EQ(&s, &mpark::get<std::string &>(w));
  // Check `v`
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  EXPECT_EQ(&s, &mpark::get<std::string &>(v));
}
#endif

TEST(Cnstr_Copy, ValuelessByException) {
  struct move_thrower_t {
    move_thrower_t() = default;
    move_thrower_t(const move_thrower_t &) = default;
    move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
    move_thrower_t &operator=(const move_thrower_t &) = default;
    move_thrower_t &operator=(move_thrower_t &&) = default;
  };  // move_thrower_t
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.valueless_by_exception());
  mpark::variant<int, move_thrower_t> w(v);
  EXPECT_TRUE(w.valueless_by_exception());
}
