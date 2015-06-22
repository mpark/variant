#include <string>
#include <sstream>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Variant, Typical) {
  mpark::variant<int, std::string> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  v = "hello"s;
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}
