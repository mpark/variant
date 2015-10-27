#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Hash, Monostate) {
  exp::variant<int, exp::monostate, std::string> v(exp::monostate{});
  // Construct hash function objects.
  std::hash<exp::variant<int, exp::monostate, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(0u, variant_hash(v));
}

TEST(Hash, String) {
  exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  // Construct hash function objects.
  std::hash<std::string> string_hash;
  std::hash<exp::variant<int, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(string_hash("hello"s), variant_hash(v));
}
