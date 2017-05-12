// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <string>
#include <sstream>

#include <gtest/gtest.h>

TEST(Assign_Conversion, SameType) {
  mpark::variant<int, std::string> v(101);
  EXPECT_EQ(101, mpark::get<int>(v));
  v = 202;
  EXPECT_EQ(202, mpark::get<int>(v));
}

TEST(Assign_Conversion, SameTypeConversion) {
  mpark::variant<int, std::string> v(1.1);
  EXPECT_EQ(1, mpark::get<int>(v));
  v = 2.2;
  EXPECT_EQ(2, mpark::get<int>(v));
}

TEST(Assign_Conversion, DiffType) {
  mpark::variant<int, std::string> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));
  v = "42";
  EXPECT_EQ("42", mpark::get<std::string>(v));
}

TEST(Assign_Conversion, DiffTypeConversion) {
  mpark::variant<int, std::string> v(42);
  EXPECT_EQ(42, mpark::get<int>(v));
  v = "42";
  EXPECT_EQ("42", mpark::get<std::string>(v));
}

TEST(Assign_Conversion, ExactMatch) {
  mpark::variant<const char *, std::string> v;
  v = std::string("hello");
  EXPECT_EQ("hello", mpark::get<std::string>(v));
}

TEST(Assign_Conversion, BetterMatch) {
  mpark::variant<int, double> v;
  // `char` -> `int` is better than `char` -> `double`
  v = 'x';
  EXPECT_EQ(static_cast<int>('x'), mpark::get<int>(v));
}

TEST(Assign_Conversion, NoMatch) { struct x { };
  static_assert(!std::is_assignable<mpark::variant<int, std::string>, x>{}, "variant<int, std::string> v; v = x;");
}

TEST(Assign_Conversion, Ambiguous) {
  static_assert(!std::is_assignable<mpark::variant<short, long>, int>{}, "variant<short, long> v; v = 42;");
}

TEST(Assign_Conversion, SameTypeOptimization) {
  mpark::variant<int, std::string> v("hello world!");
  // Check `v`.
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world!", x);
  // Save the "hello world!"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=(const char *)` to assign into `v`.
  v = "hello";
  // Check `v`.
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello", y);
  // Since "hello" is shorter than "hello world!", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

struct CopyConstruction : std::exception {};
struct CopyAssignment : std::exception {};
struct MoveConstruction : std::exception {};
struct MoveAssignment : std::exception {};

struct copy_thrower_t {
  constexpr copy_thrower_t() {}
  [[noreturn]] copy_thrower_t(const copy_thrower_t &) {
    throw CopyConstruction{};
  }
  copy_thrower_t(copy_thrower_t &&) = default;
  copy_thrower_t &operator=(const copy_thrower_t &) { throw CopyAssignment{}; }
  copy_thrower_t &operator=(copy_thrower_t &&) = default;
};  // copy_thrower_t

struct move_thrower_t {
  constexpr move_thrower_t() {}
  move_thrower_t(const move_thrower_t &) = default;
  [[noreturn]] move_thrower_t(move_thrower_t &&) { throw MoveConstruction{}; }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) { throw MoveAssignment{}; }
};  // move_thrower_t

TEST(Assign_Conversion, ThrowOnAssignment) {
  mpark::variant<int, move_thrower_t> v(
      mpark::in_place_type_t<move_thrower_t>{});
  // Since `variant` is already in `move_thrower_t`, assignment optimization
  // kicks and we simply invoke
  // `move_thrower_t &operator=(move_thrower_t &&);` which throws.
  EXPECT_THROW(v = move_thrower_t{}, MoveAssignment);
  EXPECT_FALSE(v.valueless_by_exception());
  EXPECT_EQ(1u, v.index());
  // We can still assign into a variant in an invalid state.
  v = 42;
  // Check `v`.
  EXPECT_FALSE(v.valueless_by_exception());
  EXPECT_EQ(42, mpark::get<int>(v));
}

#if 0
TEST(Assign_Conversion, ThrowOnTemporaryConstruction) {
  mpark::variant<int, copy_thrower_t> v(42);
  // Since `copy_thrower_t`'s copy constructor always throws, we will fail to
  // construct the variant. This results in our variant staying in
  // its original state.
  copy_thrower_t copy_thrower{};
  EXPECT_THROW(v = copy_thrower, CopyConstruction);
  EXPECT_FALSE(v.valueless_by_exception());
  EXPECT_EQ(0u, v.index());
  EXPECT_EQ(42, mpark::get<int>(v));
}

TEST(Assign_Conversion, ThrowOnVariantConstruction) {
  mpark::variant<int, move_thrower_t> v(42);
  // Since `move_thrower_t`'s copy constructor never throws, we successfully
  // construct the temporary object by copying `move_thrower_t`. We then
  // proceed to move the temporary object into our variant, at which point
  // `move_thrower_t`'s move constructor throws. This results in our `variant`
  // transitioning into the invalid state.
  move_thrower_t move_thrower;
  EXPECT_THROW(v = move_thrower, MoveConstruction);
  EXPECT_TRUE(v.valueless_by_exception());
  // We can still assign into a variant in an invalid state.
  v = 42;
  // Check `v`.
  EXPECT_FALSE(v.valueless_by_exception());
  EXPECT_EQ(42, mpark::get<int>(v));
}
#endif
