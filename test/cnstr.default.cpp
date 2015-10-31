// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

TEST(Cnstr_Default, Variant) {
  exp::variant<int, std::string> v;
  EXPECT_EQ(0, exp::get<0>(v));

  /* constexpr */ {
    constexpr exp::variant<int> v;
    static_assert(0 == exp::get<0>(v), "");
  }
}

TEST(Cnstr_Default, DiamondVariant) {
  exp::variant<> v;
  (void)v;
}
