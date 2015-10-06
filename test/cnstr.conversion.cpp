#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(ConvCnstr, Direct) {
  mpark::variant<int, std::string> v(42);
  EXPECT_EQ(0u, v.index());
  EXPECT_EQ(42, mpark::get<int>(v));
}

TEST(ConvCnstr, DirectRef) {
  int expected = 42;
  mpark::variant<int &, std::string &> v(expected);
  auto &actual = mpark::get<int &>(v);
  EXPECT_EQ(0u, v.index());
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(ConvCnstr, DirectConversion) {
  mpark::variant<int, std::string> v("42");
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ("42"s, mpark::get<std::string>(v));
}

TEST(ConvCnstr, DirectConversionRef) {
  std::ostringstream strm;
  mpark::variant<int &, std::ostream &> v(strm);
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ(&strm, &mpark::get<std::ostream &>(v));
}

TEST(ConvCnstr, CopyInitialization) {
  mpark::variant<int, std::string> v = 42;
  EXPECT_EQ(0u, v.index());
  EXPECT_EQ(42, mpark::get<int>(v));
}

TEST(ConvCnstr, CopyInitializationRef) {
  std::string expected = "42";
  mpark::variant<int &, std::string &> v = expected;
  EXPECT_EQ(1u, v.index());
  auto &actual = mpark::get<std::string &>(v);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(ConvCnstr, CopyInitializationConversion) {
  mpark::variant<int, std::string> v = "42";
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ("42"s, mpark::get<std::string>(v));
}

TEST(ConvCnstr, CopyInitializationConversionRef) {
  std::ostringstream strm;
  mpark::variant<int &, std::ostream &> v = strm;
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ(&strm, &mpark::get<std::ostream &>(v));
}
