// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <cassert>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

using namespace std::string_literals;

enum Qual { LRef, ConstLRef, RRef, ConstRRef };

struct get_qual {
  constexpr Qual operator()(int &) const { return LRef; }
  constexpr Qual operator()(const int &) const { return ConstLRef; }
  constexpr Qual operator()(int &&) const { return RRef; }
  constexpr Qual operator()(const int &&) const { return ConstRRef; }
};  // get_qual

TEST(Visit, MutVarMutType) {
  mpark::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, mpark::visit(get_qual(), v));
  EXPECT_EQ(RRef, mpark::visit(get_qual(), std::move(v)));
}

TEST(Visit, MutVarConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, mpark::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, mpark::visit(get_qual(), std::move(v)));
}

TEST(Visit, ConstVarMutType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, mpark::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, mpark::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr mpark::variant<int> cv(42);
    static_assert(42 == mpark::get<int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == mpark::visit(get_qual(), cv), "");
    static_assert(ConstRRef == mpark::visit(get_qual(), std::move(cv)), "");
  }
}

TEST(Visit, ConstVarConstType) {
  const mpark::variant<const int> v(42);
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, mpark::visit(get_qual(), v));
  EXPECT_EQ(ConstRRef, mpark::visit(get_qual(), std::move(v)));

  /* constexpr */ {
    constexpr mpark::variant<const int> cv(42);
    static_assert(42 == mpark::get<const int>(cv), "");
    // Check qualifier.
    static_assert(ConstLRef == mpark::visit(get_qual(), cv), "");
    static_assert(ConstRRef == mpark::visit(get_qual(), std::move(cv)), "");
  }
}

TEST(Visit_Homogeneous, Double) {
  mpark::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            mpark::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> cv(101), cw(202), cx("helllo");
    struct add {
      constexpr int operator()(int lhs, int rhs) const { return lhs + rhs; }
      constexpr int operator()(int lhs, const char *) const { return lhs; }
      constexpr int operator()(const char *, int rhs) const { return rhs; }
      constexpr int operator()(const char *, const char *) const { return 0; }
    };  // add
    static_assert(303 == mpark::visit(add{}, cv, cw), "");
    static_assert(202 == mpark::visit(add{}, cw, cx), "");
    static_assert(101 == mpark::visit(add{}, cx, cv), "");
    static_assert(0 == mpark::visit(add{}, cx, cx), "");
  }
}

TEST(Visit_Homogeneous, Quintuple) {
  mpark::variant<int, std::string> v(101), w("+"s), x(202), y("="s), z(303);
  EXPECT_EQ("101+202=303"s,
            mpark::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}

TEST(Visit_Heterogeneous, Double) {
  mpark::variant<int, std::string> v("hello"s);
  mpark::variant<double, const char *> w("world!");
  EXPECT_EQ("hello world!"s,
            mpark::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Visit_Heterogenous, Quintuple) {
  mpark::variant<int, double> v(101);
  mpark::variant<const char *> w("+");
  mpark::variant<bool, std::string, int> x(202);
  mpark::variant<char, std::string, const char *> y('=');
  mpark::variant<long, short> z(303L);
  EXPECT_EQ("101+202=303"s,
            mpark::visit([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}
