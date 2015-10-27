#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

TEST(Cnstr_Default, Variant) {
  exp::variant<int, std::string> v;
  EXPECT_EQ(0, exp::get<0>(v));
}

TEST(Cnstr_Default, DiamondVariant) {
  exp::variant<> v;
  (void)v;
}
