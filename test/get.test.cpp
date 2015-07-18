#include <mpark/variant.hpp>

#include <gtest/gtest.h>

enum class Qualifier { None, LRef, ConstLRef, RRef, ConstRRef };

Qualifier get_qualifier(int &) { return Qualifier::LRef; }
Qualifier get_qualifier(const int &) { return Qualifier::ConstLRef; }
Qualifier get_qualifier(int &&) { return Qualifier::RRef; }
Qualifier get_qualifier(const int &&) { return Qualifier::ConstRRef; }

TEST(LRef, MutableVariantMutableType) {
  mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::LRef, get_qualifier(mpark::get<int>(v)));
}

TEST(LRef, MutableVariantConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(typeid(const int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, get_qualifier(mpark::get<const int>(v)));
}

TEST(LRef, ConstVariantMutableType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, get_qualifier(mpark::get<int>(v)));
}

TEST(RRef, MutableVariantMutableType) {
  mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(std::move(v)));
  // Check qualifier.
  EXPECT_EQ(Qualifier::RRef, get_qualifier(mpark::get<int>(std::move(v))));
}

TEST(RRef, MutableVariantConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(std::move(v)));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstRRef,
            get_qualifier(mpark::get<const int>(std::move(v))));
}

TEST(RRef, ConstVariantMutableType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(std::move(v)));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstRRef, get_qualifier(mpark::get<int>(std::move(v))));
}
