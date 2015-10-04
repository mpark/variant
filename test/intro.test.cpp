#include <mpark/variant.hpp>

#include <iostream>
#include <string>

#include <gtest/gtest.h>

TEST(Variant, Intro) {
  using namespace std::string_literals;

  // direct initialization.
  mpark::variant<int, std::string> v("hello world!"s);

  // direct access via reference.
  EXPECT_EQ("hello world!"s, mpark::get<std::string>(v));

  // bad access.
  EXPECT_THROW(mpark::get<int>(v), mpark::bad_variant_access);

  // copy construction.
  mpark::variant<int, std::string> w(v);

  // direct access via pointer.
  EXPECT_FALSE(mpark::get<int>(&w));
  EXPECT_TRUE(mpark::get<std::string>(&w));

  // diff-type assignment.
  v = 42;

  // single visitation.
  EXPECT_EQ(0, mpark::type_switch (v) (
    [](int                ) { return 0; },
    [](const std::string &) { return 1; }
  ));

  // same-type assignment.
  w = "hello"s;

  EXPECT_NE(v, w);

  // make `w` equal to `v`.
  w = 42;

  EXPECT_EQ(v, w);

  // double visitation.
  EXPECT_EQ(0, mpark::type_switch (v, w) (
    [](int                , int                ) { return 0; },
    [](int                , const std::string &) { return 1; },
    [](const std::string &, int                ) { return 2; },
    [](const std::string &, const std::string &) { return 3; }
  ));
}
