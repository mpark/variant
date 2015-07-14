#include <string>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

struct thrower_t {
  thrower_t() = default;
  thrower_t(const thrower_t &) = default;
  thrower_t(thrower_t &&) { throw std::runtime_error("copy"); }
  thrower_t &operator=(const thrower_t &) = default;
  thrower_t &operator=(thrower_t &&) { throw std::runtime_error("assign"); }
};  // thrower_t

TEST(Valid, ThrowOnTemporaryConstruction) {
  mpark::variant<int, thrower_t> v(42);
  // Check `v`.
  EXPECT_TRUE(v.valid());
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  try {
    // Since `thrower_t`'s move constructor always throws, we will fail to
    // construct the temporary object. This results in our variant staying in
    // its original state.
    v = thrower_t{};
  } catch (...) {
    EXPECT_TRUE(v.valid());
    EXPECT_EQ(typeid(int), v.type());
    EXPECT_EQ(42, mpark::get<int>(v));
  }
}

TEST(Valid, ThrowOnVariantConstruction) {
  mpark::variant<int, thrower_t> v(42);
  // Check `v`.
  EXPECT_TRUE(v.valid());
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  try {
    // Since `thrower_t`'s copy constructor never throws, we successfully
    // construct the temporary object by copying `thrower_t`. We then proceed to
    // move the temporary object into our variant, at which point `thrower_t`'s
    // move constructor throws. This results in our variant transitioning into
    // the invalid state.
    thrower_t thrower;
    v = thrower;
  } catch (...) {
    EXPECT_FALSE(v.valid());
    // We can still assign into a variant in an invalid state.
    v = 42;
    // Check `v`.
    EXPECT_TRUE(v.valid());
    EXPECT_EQ(typeid(int), v.type());
    EXPECT_EQ(42, mpark::get<int>(v));
  }
}
