// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

TEST(Access_HoldsAlternative, Typical) {
  std_exp::variant<int, std::string> v(42);
  EXPECT_TRUE(std_exp::holds_alternative<0>(v));
  EXPECT_FALSE(std_exp::holds_alternative<1>(v));
  EXPECT_TRUE(std_exp::holds_alternative<int>(v));
  EXPECT_FALSE(std_exp::holds_alternative<std::string>(v));

  /* constexpr */ {
    constexpr std_exp::variant<int, const char *> v(42);
    static_assert(std_exp::holds_alternative<0>(v), "");
    static_assert(!std_exp::holds_alternative<1>(v), "");
    static_assert(std_exp::holds_alternative<int>(v), "");
    static_assert(!std_exp::holds_alternative<const char *>(v), "");
  }
}
