#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Variant, Intro) {
  mpark::variant<int, float> v(1.1f);
  // Check `v`.
  EXPECT_EQ(typeid(float), v.type());
  EXPECT_FLOAT_EQ(1.1f, mpark::get<float>(v));
  // Change type.
  v = 42;
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Copy from `v`.
  mpark::variant<int, float> w = v;
  // Check `w`.
  EXPECT_EQ(typeid(int), w.type());
  EXPECT_EQ(42, mpark::get<int>(w));
  // mpark::get<double>(v);  // ill-formed.
  EXPECT_THROW(mpark::get<float>(v), mpark::bad_variant_access);
}
