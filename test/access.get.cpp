// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

enum Qual { Ptr, ConstPtr, LRef, ConstLRef, RRef, ConstRRef };

constexpr Qual get_qual(int *) { return Ptr; }
constexpr Qual get_qual(const int *) { return ConstPtr; }
constexpr Qual get_qual(int &) { return LRef; }
constexpr Qual get_qual(const int &) { return ConstLRef; }
constexpr Qual get_qual(int &&) { return RRef; }
constexpr Qual get_qual(const int &&) { return ConstRRef; }

TEST(Access_Get, MutVarMutType) {
  mpark::variant<int> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(mpark::get<int>(v)));
  EXPECT_EQ(RRef, get_qual(mpark::get<int>(std::move(v))));
}

#if 0
TEST(Access_Get, MutVarMutTypeRef) {
  int expected = 42;
  mpark::variant<int &> v(expected);
  EXPECT_EQ(expected, mpark::get<int &>(v));
  EXPECT_EQ(&expected, &mpark::get<int &>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(mpark::get<int &>(v)));
  EXPECT_EQ(LRef, get_qual(mpark::get<int &>(std::move(v))));
}
#endif

TEST(Access_Get, MutVarConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(mpark::get<const int>(std::move(v))));
}

#if 0
TEST(Access_Get, MutVarConstTypeRef) {
  int expected = 42;
  mpark::variant<const int &> v(expected);
  EXPECT_EQ(expected, mpark::get<const int &>(v));
  EXPECT_EQ(&expected, &mpark::get<const int &>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int &>(v)));
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int &>(std::move(v))));
}
#endif

TEST(Access_Get, ConstVarMutType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(mpark::get<int>(std::move(v))));

  /* constexpr */ {
    constexpr mpark::variant<int> cv(42);
    static_assert(42 == mpark::get<int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == get_qual(mpark::get<int>(cv)), "");
    static_assert(ConstRRef == get_qual(mpark::get<int>(std::move(cv))), "");
  }
}

#if 0
TEST(Access_Get, ConstVarMutTypeRef) {
  int expected = 42;
  const mpark::variant<int &> v(expected);
  EXPECT_EQ(expected, mpark::get<int &>(v));
  EXPECT_EQ(&expected, &mpark::get<int &>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(mpark::get<int &>(v)));
  EXPECT_EQ(LRef, get_qual(mpark::get<int &>(std::move(v))));
}
#endif

TEST(Access_Get, ConstVarConstType) {
  const mpark::variant<const int> v(42);
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(mpark::get<const int>(std::move(v))));

  /* constexpr */ {
    constexpr mpark::variant<const int> cv(42);
    static_assert(42 == mpark::get<const int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == get_qual(mpark::get<const int>(cv)), "");
    static_assert(ConstRRef == get_qual(mpark::get<const int>(std::move(cv))), "");
  }
}

#if 0
TEST(Access_Get, ConstVarConstTypeRef) {
  int expected = 42;
  const mpark::variant<const int &> v(expected);
  EXPECT_EQ(expected, mpark::get<const int &>(v));
  EXPECT_EQ(&expected, &mpark::get<const int &>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int &>(v)));
  EXPECT_EQ(ConstLRef, get_qual(mpark::get<const int &>(std::move(v))));

  /* constexpr */ {
    static constexpr int expected = 42;
    constexpr mpark::variant<const int &> cv(expected);
    static_assert(42 == mpark::get<const int &>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == get_qual(mpark::get<const int &>(cv)), "");
    static_assert(ConstLRef == get_qual(mpark::get<const int &>(std::move(cv))), "");
  }
}
#endif

TEST(Access_Get, ValuelessByException) {
  struct move_thrower_t {
    move_thrower_t() = default;
    move_thrower_t(const move_thrower_t &) = default;
    [[noreturn]] move_thrower_t(move_thrower_t &&) {
      throw std::runtime_error("");
    }
    move_thrower_t &operator=(const move_thrower_t &) = default;
    move_thrower_t &operator=(move_thrower_t &&) = default;
  };  // move_thrower_t
  mpark::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.valueless_by_exception());
  EXPECT_THROW(mpark::get<int>(v), mpark::bad_variant_access);
  EXPECT_THROW(mpark::get<move_thrower_t>(v), mpark::bad_variant_access);
}
