// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Access_HoldsAlternative, Typical) {
  mpark::variant<int, std::string> v(42);
  EXPECT_TRUE(mpark::holds_alternative<0>(v));
  EXPECT_FALSE(mpark::holds_alternative<1>(v));
  EXPECT_TRUE(mpark::holds_alternative<int>(v));
  EXPECT_FALSE(mpark::holds_alternative<std::string>(v));

  /* constexpr */ {
    constexpr mpark::variant<int, const char *> cv(42);
    static_assert(mpark::holds_alternative<0>(cv), "");
    static_assert(!mpark::holds_alternative<1>(cv), "");
    static_assert(mpark::holds_alternative<int>(cv), "");
    static_assert(!mpark::holds_alternative<const char *>(cv), "");
  }
}
