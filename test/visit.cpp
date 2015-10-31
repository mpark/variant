// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <cassert>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

enum Qual { LRef, ConstLRef, RRef, ConstRRef };

struct get_qual {
  constexpr Qual operator()(int &) const { return LRef; }
  constexpr Qual operator()(const int &) const { return ConstLRef; }
  constexpr Qual operator()(int &&) const { return RRef; }
  constexpr Qual operator()(const int &&) const { return ConstRRef; }
};  // get_qual

TEST(Visit, MutVarMutType) {
  exp::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, exp::visit(get_qual(), v));
  EXPECT_EQ(RRef, exp::visit(get_qual(), std::move(v)));
}

TEST(Visit, MutVarConstType) {
  exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, exp::visit(get_qual(), std::move(v)));
}

TEST(Visit, ConstVarMutType) {
  const exp::variant<int> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, exp::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr exp::variant<int> v(42);
    static_assert(42 == exp::get<int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == exp::visit(get_qual(), v), "");
    static_assert(ConstRRef == exp::visit(get_qual(), std::move(v)), "");
  }
}

TEST(Visit, ConstVarConstType) {
  const exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, exp::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr exp::variant<const int> v(42);
    static_assert(42 == exp::get<const int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == exp::visit(get_qual(), v), "");
    static_assert(ConstRRef == exp::visit(get_qual(), std::move(v)), "");
  }
}

TEST(Visit_Homogeneous, Double) {
  exp::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            exp::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));

  /* constexpr */ {
    constexpr exp::variant<int, const char *> v(101), w(202), x("helllo");
    struct add {
      constexpr int operator()(int lhs, int rhs) const { return lhs + rhs; }
      constexpr int operator()(int lhs, const char *) const { return lhs; }
      constexpr int operator()(const char *, int rhs) const { return rhs; }
      constexpr int operator()(const char *, const char *) const { return 0; }
    };  // add
    static_assert(303 == exp::visit(add{}, v, w), "");
    static_assert(202 == exp::visit(add{}, w, x), "");
    static_assert(101 == exp::visit(add{}, x, v), "");
    static_assert(0 == exp::visit(add{}, x, x), "");
  }
}

TEST(Visit_Homogeneous, Quintuple) {
  exp::variant<int, std::string> v(101), w("+"s), x(202), y("="s), z(303);
  EXPECT_EQ("101+202=303"s,
            exp::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}

TEST(Visit_Heterogeneous, Double) {
  exp::variant<int, std::string> v("hello"s);
  exp::variant<double, const char *> w("world!");
  EXPECT_EQ("hello world!"s,
            exp::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Visit_Heterogenous, Quintuple) {
  exp::variant<int, double> v(101);
  exp::variant<const char *> w("+");
  exp::variant<bool, std::string, int> x(202);
  exp::variant<char, std::string, const char *> y('=');
  exp::variant<long, short> z(303L);
  EXPECT_EQ("101+202=303"s,
            exp::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}
