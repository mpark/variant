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

struct Json : mpark::variant<bool,
                             int,
                             std::string,
                             std::vector<Json>,
                             std::map<std::string, Json>> {
  using variant::variant;
};

TEST(Variant, Bool) {
  Json json = true;
  EXPECT_TRUE(mpark::get<bool>(json));
  json = false;
  EXPECT_FALSE(mpark::get<bool>(json));
}

TEST(Variant, Int) {
  Json json = 42;
  EXPECT_EQ(42, mpark::get<int>(json));
}

TEST(Variant, String) {
  Json json = std::string("hello");
  EXPECT_EQ("hello", mpark::get<std::string>(json));
}

TEST(Variant, Array) {
  Json json = std::vector<Json> { true, 42, std::string("world") };
  const std::vector<Json>& array = mpark::get<std::vector<Json>>(json);
  EXPECT_TRUE(mpark::get<bool>(array[0]));
  EXPECT_EQ(42, mpark::get<int>(array[1]));
  EXPECT_EQ("world", mpark::get<std::string>(array[2]));
}

TEST(Variant, Dictionary) {
  Json json = std::map<std::string, Json> {
    { "x", true },
    { "y", 42 },
    { "z", std::string("world") }
  };
  const std::map<std::string, Json>& dict = mpark::get<std::map<std::string, Json>>(json);
  EXPECT_TRUE(mpark::get<bool>(dict.at("x")));
  EXPECT_EQ(42, mpark::get<int>(dict.at("y")));
  EXPECT_EQ("world", mpark::get<std::string>(dict.at("z")));
}
