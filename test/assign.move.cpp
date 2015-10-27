#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Assign_Move, SameType) {
  // `v`, `w`.
  exp::variant<int, std::string> v("hello"s), w("world"s);
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  EXPECT_EQ("world"s, exp::get<std::string>(w));
  // move assignment.
  v = std::move(w);
  EXPECT_EQ("world"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_TRUE(exp::get<std::string>(w).empty());
}

TEST(Assign_Move, DiffType) {
  // `v`, `w`.
  exp::variant<int, std::string> v(42), w("world"s);
  EXPECT_EQ(42, exp::get<int>(v));
  EXPECT_EQ("world"s, exp::get<std::string>(w));
  // move assignment.
  v = std::move(w);
  EXPECT_EQ("world"s, exp::get<std::string>(v));
  // Check `w`.
  EXPECT_TRUE(exp::get<std::string>(w).empty());
}
