// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <cassert>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

enum Qual { LRef, ConstLRef, RRef, ConstRRef };

struct get_qual {
  constexpr Qual operator()(int &) const { return LRef; }
  constexpr Qual operator()(const int &) const { return ConstLRef; }
  constexpr Qual operator()(int &&) const { return RRef; }
  constexpr Qual operator()(const int &&) const { return ConstRRef; }
};  // get_qual

TEST(Visit, MutVarMutType) {
  std_exp::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(42, std_exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, std_exp::visit(get_qual(), v));
  EXPECT_EQ(RRef, std_exp::visit(get_qual(), std::move(v)));
}

TEST(Visit, MutVarConstType) {
  std_exp::variant<const int> v(42);
  EXPECT_EQ(42, std_exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std_exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std_exp::visit(get_qual(), std::move(v)));
}

TEST(Visit, ConstVarMutType) {
  const std_exp::variant<int> v(42);
  EXPECT_EQ(42, std_exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std_exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std_exp::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr std_exp::variant<int> v(42);
    static_assert(42 == std_exp::get<int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == std_exp::visit(get_qual(), v), "");
    static_assert(ConstRRef == std_exp::visit(get_qual(), std::move(v)), "");
  }
}

TEST(Visit, ConstVarConstType) {
  const std_exp::variant<const int> v(42);
  EXPECT_EQ(42, std_exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, std_exp::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, std_exp::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr std_exp::variant<const int> v(42);
    static_assert(42 == std_exp::get<const int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == std_exp::visit(get_qual(), v), "");
    static_assert(ConstRRef == std_exp::visit(get_qual(), std::move(v)), "");
  }
}

TEST(Visit_Homogeneous, Double) {
  std_exp::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            std_exp::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(101), w(202), x("helllo");
    struct add {
      constexpr int operator()(int lhs, int rhs) const { return lhs + rhs; }
      constexpr int operator()(int lhs, const char *) const { return lhs; }
      constexpr int operator()(const char *, int rhs) const { return rhs; }
      constexpr int operator()(const char *, const char *) const { return 0; }
    };  // add
    static_assert(303 == std_exp::visit(add{}, v, w), "");
    static_assert(202 == std_exp::visit(add{}, w, x), "");
    static_assert(101 == std_exp::visit(add{}, x, v), "");
    static_assert(0 == std_exp::visit(add{}, x, x), "");
  }
}

TEST(Visit_Homogeneous, Quintuple) {
  std_exp::variant<int, std::string> v(101), w("+"s), x(202), y("="s), z(303);
  EXPECT_EQ("101+202=303"s,
            std_exp::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}

TEST(Visit_Heterogeneous, Double) {
  std_exp::variant<int, std::string> v("hello"s);
  std_exp::variant<double, const char *> w("world!");
  EXPECT_EQ("hello world!"s,
            std_exp::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Visit_Heterogenous, Quintuple) {
  std_exp::variant<int, double> v(101);
  std_exp::variant<const char *> w("+");
  std_exp::variant<bool, std::string, int> x(202);
  std_exp::variant<char, std::string, const char *> y('=');
  std_exp::variant<long, short> z(303L);
  EXPECT_EQ("101+202=303"s,
            std_exp::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}
