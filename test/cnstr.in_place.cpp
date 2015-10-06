#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(InPlaceCnstr, IndexDirect) {
  using I = meta::size_t<0>;
  mpark::variant<int, std::string> v(mpark::in_place<I{}>, 42);
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ(42, mpark::get<I{}>(v));
}

TEST(InPlaceCnstr, IndexDirectDuplicate) {
  using I = meta::size_t<0>;
  mpark::variant<int, int> v(mpark::in_place<I{}>, 42);
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ(42, mpark::get<I{}>(v));
}

TEST(InPlaceCnstr, IndexConversion) {
  using I = meta::size_t<1>;
  mpark::variant<int, std::string> v(mpark::in_place<I{}>, "42");
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ("42"s, mpark::get<I{}>(v));
}

TEST(InPlaceCnstr, IndexConversionDuplicateRef) {
  using I = meta::size_t<0>;
  std::ostringstream strm;
  mpark::variant<std::ostream &, std::ostream &> v(mpark::in_place<I{}>, strm);
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ(&strm, &mpark::get<I{}>(v));
}

TEST(InPlaceCnstr, IndexInitializerList) {
  using I = meta::size_t<1>;
  mpark::variant<int, std::string> v(mpark::in_place<I{}>, {'4', '2'});
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ("42"s, mpark::get<I{}>(v));
}

TEST(InPlaceCnstr, TypeDirect) {
  using T = std::string;
  mpark::variant<int, std::string> v(mpark::in_place<T>, "42"s);
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ("42"s, mpark::get<T>(v));
}

TEST(InPlaceCnstr, TypeDirectRef) {
  using T = int &;
  int expected = 42;
  mpark::variant<int &, std::ostream &> v(mpark::in_place<T>, expected);
  EXPECT_EQ(0u, v.index());
  auto &actual = mpark::get<T>(v);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(&expected, &actual);
}

TEST(InPlaceCnstr, TypeConversion) {
  using T = int;
  mpark::variant<int, std::string> v(mpark::in_place<T>, 42.5);
  EXPECT_EQ(0u, v.index());
  EXPECT_EQ(42, mpark::get<T>(v));
}

TEST(InPlaceCnstr, TypeConversionRef) {
  using T = std::ostream &;
  std::ostringstream strm;
  mpark::variant<int, std::ostream &> v(mpark::in_place<T>, strm);
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ(&strm, &mpark::get<T>(v));
}

TEST(InPlaceCnstr, TypeInitializerList) {
  using T = std::string;
  mpark::variant<int, std::string> v(mpark::in_place<T>, {'4', '2'});
  EXPECT_EQ(1u, v.index());
  EXPECT_EQ("42"s, mpark::get<T>(v));
}
