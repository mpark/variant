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

TEST(InPlaceCnstr, IndexConversion) {
  using I = meta::size_t<1>;
  mpark::variant<int, std::string> v(mpark::in_place<I{}>, "42");
  EXPECT_EQ(I{}, v.index());
  EXPECT_EQ("42"s, mpark::get<I{}>(v));
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
  EXPECT_EQ(typeid(T), v.type());
  EXPECT_EQ("42"s, mpark::get<T>(v));
}

TEST(InPlaceCnstr, TypeConversion) {
  using T = int;
  mpark::variant<int, std::string> v(mpark::in_place<T>, 42.5);
  EXPECT_EQ(typeid(T), v.type());
  EXPECT_EQ(42, mpark::get<T>(v));
}

TEST(InPlaceCnstr, TypeInitializerList) {
  using T = std::string;
  mpark::variant<int, std::string> v(mpark::in_place<T>, {'4', '2'});
  EXPECT_EQ(typeid(T), v.type());
  EXPECT_EQ("42"s, mpark::get<T>(v));
}
