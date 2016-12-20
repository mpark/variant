// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Assign_Emplace, IndexDirect) {
  mpark::variant<int, std::string> v;
  v.emplace<1>("42"s);
  EXPECT_EQ("42"s, mpark::get<1>(v));
}

#if 0
TEST(Assign_Emplace, IndexDirectRef) {
  int x = 42;
  std::ostringstream y;
  mpark::variant<int &, std::ostream &> v(mpark::in_place_index<0>, x);
  EXPECT_EQ(&x, &mpark::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &mpark::get<1>(v));
}
#endif

TEST(Assign_Emplace, IndexDirectDuplicate) {
  mpark::variant<int, int> v;
  v.emplace<1>(42);
  EXPECT_EQ(42, mpark::get<1>(v));
}

#if 0
TEST(Assign_Emplace, IndexDirectDuplicateRef) {
  int x, y;
  mpark::variant<int &, int &> v(mpark::in_place_index<1>, y);
  EXPECT_EQ(&y, &mpark::get<1>(v));
  v.emplace<0>(x);
  EXPECT_EQ(&x, &mpark::get<0>(v));
}
#endif

TEST(Assign_Emplace, IndexConversion) {
  mpark::variant<int, std::string> v;
  v.emplace<1>("42");
  EXPECT_EQ("42"s, mpark::get<1>(v));
}

#if 0
TEST(Assign_Emplace, IndexConversionRef) {
  int x = 42;
  std::ostringstream y;
  mpark::variant<int &, std::ostream &> v(mpark::in_place_index<0>, x);
  EXPECT_EQ(&x, &mpark::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &mpark::get<1>(v));
}
#endif

TEST(Assign_Emplace, IndexConversionDuplicate) {
  mpark::variant<int, int> v;
  v.emplace<1>(1.1);
  EXPECT_EQ(1, mpark::get<1>(v));
}

#if 0
TEST(Assign_Emplace, IndexConversionDuplicateRef) {
  std::ostringstream x, y;
  mpark::variant<std::ostream &, std::ostream &> v(mpark::in_place_index<0>,
                                                     x);
  EXPECT_EQ(&x, &mpark::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &mpark::get<1>(v));
}
#endif

TEST(Assign_Emplace, IndexInitializerList) {
  mpark::variant<int, std::string> v;
  v.emplace<1>({'4', '2'});
  EXPECT_EQ("42"s, mpark::get<1>(v));
}

TEST(Assign_Emplace, TypeDirect) {
  mpark::variant<int, std::string> v;
  v.emplace<std::string>("42"s);
  EXPECT_EQ("42"s, mpark::get<std::string>(v));
}

#if 0
TEST(Assign_Emplace, TypeDirectRef) {
  int x = 42;
  std::ostringstream y;
  mpark::variant<int &, std::ostream &> v(mpark::in_place_type<int &>, x);
  EXPECT_EQ(&x, &mpark::get<int &>(v));
  v.emplace<std::ostream &>(y);
  EXPECT_EQ(&y, &mpark::get<std::ostream &>(v));
}
#endif

TEST(Assign_Emplace, TypeConversion) {
  mpark::variant<int, std::string> v;
  v.emplace<int>(1.1);
  EXPECT_EQ(1, mpark::get<int>(v));
}

#if 0
TEST(Assign_Emplace, TypeConversionRef) {
  int x = 42;
  std::ostringstream y;
  mpark::variant<int &, std::ostream &> v(x);
  EXPECT_EQ(&x, &mpark::get<int &>(v));
  v.emplace<std::ostream &>(y);
  EXPECT_EQ(&y, &mpark::get<std::ostream &>(v));
}
#endif

TEST(Assign_Emplace, TypeInitializerList) {
  mpark::variant<int, std::string> v;
  v.emplace<std::string>({'4', '2'});
  EXPECT_EQ("42"s, mpark::get<std::string>(v));
}
