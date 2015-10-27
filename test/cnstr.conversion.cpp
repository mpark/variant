#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Cnstr_Conversion, Direct) {
  exp::variant<int, std::string> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
}

TEST(Cnstr_Conversion, DirectRef) {
  int expected = 42;
  exp::variant<int &, std::string &> v(expected);
  auto &actual = exp::get<int &>(v);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(Cnstr_Conversion, DirectConversion) {
  exp::variant<int, std::string> v("42");
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}

TEST(Cnstr_Conversion, DirectConversionRef) {
  std::ostringstream strm;
  exp::variant<int &, std::ostream &> v(strm);
  EXPECT_EQ(&strm, &exp::get<std::ostream &>(v));
}

TEST(Cnstr_Conversion, CopyInitialization) {
  exp::variant<int, std::string> v = 42;
  EXPECT_EQ(42, exp::get<int>(v));
}

TEST(Cnstr_Conversion, CopyInitializationRef) {
  std::string expected = "42";
  exp::variant<int &, std::string &> v = expected;
  auto &actual = exp::get<std::string &>(v);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(Cnstr_Conversion, CopyInitializationConversion) {
  exp::variant<int, std::string> v = "42";
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}

TEST(Cnstr_Conversion, CopyInitializationConversionRef) {
  std::ostringstream strm;
  exp::variant<int &, std::ostream &> v = strm;
  EXPECT_EQ(&strm, &exp::get<std::ostream &>(v));
}
