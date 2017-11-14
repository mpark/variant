// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <map>
#include <string>
#include <vector>

#include <gtest/gtest.h>

struct JsonIsh : mpark::variant<bool, int, std::string, std::vector<JsonIsh>> {
  using variant::variant;
};

TEST(Variant, Bool) {
  JsonIsh json = true;
  EXPECT_TRUE(mpark::get<bool>(json));
  json = false;
  EXPECT_FALSE(mpark::get<bool>(json));
}

TEST(Variant, Int) {
  JsonIsh json = 42;
  EXPECT_EQ(42, mpark::get<int>(json));
}

TEST(Variant, String) {
  JsonIsh json = std::string("hello");
  EXPECT_EQ("hello", mpark::get<std::string>(json));
}

TEST(Variant, Array) {
  JsonIsh json = std::vector<JsonIsh>{true, 42, std::string("world")};
  const auto &array = mpark::get<std::vector<JsonIsh>>(json);
  EXPECT_TRUE(mpark::get<bool>(array[0]));
  EXPECT_EQ(42, mpark::get<int>(array[1]));
  EXPECT_EQ("world", mpark::get<std::string>(array[2]));
}
