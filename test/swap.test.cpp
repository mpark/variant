#include <string>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Swap, Same) {
  mpark::variant<int, std::string> v("hello"s);
  mpark::variant<int, std::string> w("world"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("world"s, mpark::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("world"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
}

TEST(Swap, Different) {
  mpark::variant<int, std::string> v(42);
  mpark::variant<int, std::string> w("hello"s);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(int), w.type());
  EXPECT_EQ(42, mpark::get<int>(w));
}
