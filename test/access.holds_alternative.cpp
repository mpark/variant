// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <gtest/gtest.h>

namespace exp = std::experimental;

TEST(Access_HoldsAlternative, Typical) {
  exp::variant<int, std::string> v(42);
  EXPECT_TRUE(exp::holds_alternative<0>(v));
  EXPECT_FALSE(exp::holds_alternative<1>(v));
  EXPECT_TRUE(exp::holds_alternative<int>(v));
  EXPECT_FALSE(exp::holds_alternative<std::string>(v));

  /* constexpr */ {
    constexpr exp::variant<int, const char *> v(42);
    static_assert(exp::holds_alternative<0>(v), "");
    static_assert(!exp::holds_alternative<1>(v), "");
    static_assert(exp::holds_alternative<int>(v), "");
    static_assert(!exp::holds_alternative<const char *>(v), "");
  }
}
