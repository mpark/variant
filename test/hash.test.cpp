#include <string>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Hash, Null) {
  mpark::variant<int, mpark::null_t, std::string> v;
  // Check `v`.
  EXPECT_EQ(typeid(mpark::null_t), v.type());
  // Construct hash function objects.
  std::hash<mpark::variant<int, mpark::null_t, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(0u, variant_hash(v));
}

TEST(Hash, NonNull) {
  mpark::variant<int, std::string> v("hello"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Construct hash function objects.
  std::hash<mpark::variant<int, std::string>> variant_hash;
  std::hash<std::string> string_hash;
  // Check the hash.
  EXPECT_EQ(string_hash("hello"s), variant_hash(v));
}
