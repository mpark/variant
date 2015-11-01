// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Hash, Monostate) {
  std_exp::variant<int, std_exp::monostate, std::string> v(
      std_exp::monostate{});
  // Construct hash function objects.
  std::hash<std_exp::variant<int, std_exp::monostate, std::string>>
      variant_hash;
  // Check the hash.
  EXPECT_EQ(0u, variant_hash(v));
}

TEST(Hash, String) {
  std_exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));
  // Construct hash function objects.
  std::hash<std::string> string_hash;
  std::hash<std_exp::variant<int, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(string_hash("hello"s), variant_hash(v));
}
