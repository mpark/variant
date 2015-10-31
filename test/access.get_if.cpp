// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <gtest/gtest.h>

namespace exp = std::experimental;

enum Qual { Ptr, ConstPtr, LRef, ConstLRef, RRef, ConstRRef };

constexpr Qual get_qual(int *) { return Ptr; }
constexpr Qual get_qual(const int *) { return ConstPtr; }
constexpr Qual get_qual(int &) { return LRef; }
constexpr Qual get_qual(const int &) { return ConstLRef; }
constexpr Qual get_qual(int &&) { return RRef; }
constexpr Qual get_qual(const int &&) { return ConstRRef; }

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) { throw std::runtime_error(""); }
};  // move_thrower_t

TEST(Access_GetIf, MutVarMutType) {
  exp::variant<int> v(42);
  EXPECT_EQ(42, *exp::get_if<int>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(exp::get_if<int>(&v)));
}

TEST(Access_GetIf, MutVarMutTypeRef) {
  int expected = 42;
  exp::variant<int &> v(expected);
  EXPECT_EQ(expected, *exp::get_if<int &>(&v));
  EXPECT_EQ(&expected, exp::get_if<int &>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(exp::get_if<int &>(&v)));
}

TEST(Access_GetIf, MutVarConstType) {
  exp::variant<const int> v(42);
  EXPECT_EQ(42, *exp::get_if<const int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(exp::get_if<const int>(&v)));
}

TEST(Access_GetIf, MutVarConstTypeRef) {
  int expected = 42;
  exp::variant<const int &> v(expected);
  EXPECT_EQ(expected, *exp::get_if<const int &>(&v));
  EXPECT_EQ(&expected, exp::get_if<const int &>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(exp::get_if<const int &>(&v)));
}

TEST(Access_GetIf, ConstVarMutType) {
  const exp::variant<int> v(42);
  EXPECT_EQ(42, *exp::get_if<int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(exp::get_if<int>(&v)));

  /* constexpr */ {
    constexpr exp::variant<int> v(42);
    static_assert(42 == *exp::get_if<int>(&v), "");
    // Check qualifier.
    static_assert(ConstPtr == get_qual(exp::get_if<int>(&v)), "");
  }
}

TEST(Access_GetIf, ConstVarMutTypeRef) {
  int expected = 42;
  const exp::variant<int &> v(expected);
  EXPECT_EQ(expected, *exp::get_if<int &>(&v));
  EXPECT_EQ(&expected, exp::get_if<int &>(&v));
  // Check qualifier.
  EXPECT_EQ(Ptr, get_qual(exp::get_if<int &>(&v)));
}

TEST(Access_GetIf, ConstVarConstType) {
  const exp::variant<const int> v(42);
  EXPECT_EQ(42, *exp::get_if<const int>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(exp::get_if<const int>(&v)));

  /* constexpr */ {
    constexpr exp::variant<const int> v(42);
    static_assert(42 == *exp::get_if<const int>(&v), "");
    // Check qualifier.
    static_assert(ConstPtr == get_qual(exp::get_if<const int>(&v)), "");
  }
}

TEST(Access_GetIf, ConstVarConstTypeRef) {
  int expected = 42;
  const exp::variant<const int &> v(expected);
  EXPECT_EQ(expected, *exp::get_if<const int &>(&v));
  EXPECT_EQ(&expected, exp::get_if<const int &>(&v));
  // Check qualifier.
  EXPECT_EQ(ConstPtr, get_qual(exp::get_if<const int &>(&v)));
}

TEST(Access_GetIf, MoveCorruptedByException) {
  exp::variant<int, move_thrower_t> v(42);
  try {
    v = move_thrower_t{};
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_EQ(nullptr, exp::get_if<int>(&v));
    EXPECT_EQ(nullptr, exp::get_if<move_thrower_t>(&v));
  }  // try
}

TEST(Access_GetIf, CopyCorruptedByException) {
  exp::variant<int, move_thrower_t> v(42);
  try {
    move_thrower_t move_thrower;
    v = move_thrower;
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_EQ(nullptr, exp::get_if<int>(&v));
    EXPECT_EQ(nullptr, exp::get_if<move_thrower_t>(&v));
  }  // try
}
