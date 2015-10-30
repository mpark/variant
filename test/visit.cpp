// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>
#include <sstream>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

enum class Qualifier { LRef, ConstLRef, RRef, ConstRRef };

struct get_qualifier {
  Qualifier operator()(int &) const { return Qualifier::LRef; }
  Qualifier operator()(const int &) const { return Qualifier::ConstLRef; }
  Qualifier operator()(int &&) const { return Qualifier::RRef; }
  Qualifier operator()(const int &&) const { return Qualifier::ConstRRef; }
};  // get_qualifier

TEST(Visit, MutVarMutType) {
  exp::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::LRef, exp::visit(get_qualifier(), v));
  EXPECT_EQ(Qualifier::RRef, exp::visit(get_qualifier(), std::move(v)));
}

TEST(Visit, MutVarConstType) {
  exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, exp::visit(get_qualifier(), v));
  EXPECT_EQ(Qualifier::ConstRRef, exp::visit(get_qualifier(), std::move(v)));
}

TEST(Visit, ConstVarMutType) {
  const exp::variant<int> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, exp::visit(get_qualifier(), v));
  EXPECT_EQ(Qualifier::ConstRRef, exp::visit(get_qualifier(), std::move(v)));
}

TEST(Visit, ConstVarConstType) {
  const exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, exp::visit(get_qualifier(), v));
  EXPECT_EQ(Qualifier::ConstRRef, exp::visit(get_qualifier(), std::move(v)));
}

TEST(Visit_Homogeneous, Double) {
  exp::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            exp::visit([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
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
