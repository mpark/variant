#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_Copy, Value) {
  // `v`
  exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
  // `w`
  exp::variant<int, std::string> w(v);
  EXPECT_EQ("hello"s, exp::get<std::string>(w));
  // Check `v`
  EXPECT_EQ("hello"s, exp::get<std::string>(v));
}

TEST(Cnstr_Copy, Ref) {
  std::string s = "hello"s;
  // `v`
  exp::variant<int &, std::string &> v(s);
  EXPECT_EQ("hello"s, exp::get<std::string &>(v));
  EXPECT_EQ(&s, &exp::get<std::string &>(v));
  // `w`
  exp::variant<int &, std::string &> w(v);
  EXPECT_EQ("hello"s, exp::get<std::string &>(w));
  EXPECT_EQ(&s, &exp::get<std::string &>(w));
  // Check `v`
  EXPECT_EQ("hello"s, exp::get<std::string &>(v));
  EXPECT_EQ(&s, &exp::get<std::string &>(v));
}
