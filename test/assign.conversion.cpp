#include <variant.hpp>

#include <string>
#include <sstream>

#include <gtest/gtest.h>

namespace exp = std::experimental;

using namespace std::string_literals;

TEST(Assign_Conversion, SameType) {
  exp::variant<int, std::string> v(101);
  EXPECT_EQ(101, exp::get<int>(v));
  v = 202;
  EXPECT_EQ(202, exp::get<int>(v));
}

TEST(Assign_Conversion, SameTypeConversion) {
  exp::variant<int, std::string> v(1.1);
  EXPECT_EQ(1, exp::get<int>(v));
  v = 2.2;
  EXPECT_EQ(2, exp::get<int>(v));
}

TEST(Assign_Conversion, DiffType) {
  exp::variant<int, std::string> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  v = "42"s;
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}

TEST(Assign_Conversion, DiffTypeConversion) {
  exp::variant<int, std::string> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  v = "42";
  EXPECT_EQ("42"s, exp::get<std::string>(v));
}

TEST(Assign_Conversion, ExactMatch) {
  exp::variant<const char *, std::string> v;
  v = "hello";
  EXPECT_EQ("hello", exp::get<const char *>(v));
}

TEST(Assign_Conversion, BetterMatch) {
  exp::variant<int, double> v;
  // `char` -> `int` is better than `char` -> `double`
  v = 'x';
  EXPECT_EQ(static_cast<int>('x'), exp::get<int>(v));
}

TEST(Assign_Conversion, NoMatch) {
  struct x {};
  static_assert(!std::is_assignable<exp::variant<int, std::string>, x>{},
                "variant<int, std::string> v; v = x;");
}

TEST(Assign_Conversion, Ambiguous) {
  static_assert(!std::is_assignable<exp::variant<short, long>, int>{},
                "variant<short, long> v; v = 42;");
}

TEST(Assign_Conversion, SameTypeOptimization) {
  exp::variant<int, std::string> v("hello world!"s);
  // Check `v`.
  const std::string &x = exp::get<std::string>(v);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world!"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `v`.
  v = "hello"s;
  // Check `v`.
  const std::string &y = exp::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world!", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

struct copy_thrower_t {
  copy_thrower_t() = default;
  copy_thrower_t(const copy_thrower_t &) {
    throw std::runtime_error("copy constructor");
  }
  copy_thrower_t(copy_thrower_t &&) = default;
  copy_thrower_t &operator=(const copy_thrower_t &) {
    throw std::runtime_error("copy assignment");
  }
  copy_thrower_t &operator=(copy_thrower_t &&) = default;
};  // copy_thrower_t

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) {
    throw std::runtime_error("move constructor");
  }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) {
    throw std::runtime_error("move assignment");
  }
};  // move_thrower_t

TEST(Assign_Conversion, ThrowOnAssignment) {
  exp::variant<int, move_thrower_t> v(exp::in_place_type<move_thrower_t>);
  try {
    // Since `variant` is already in `move_thrower_t`, assignment optimization
    // kicks and we simply invoke
    // `move_thrower_t &operator=(move_thrower_t &&);` which throws.
    v = move_thrower_t{};
  } catch (const std::exception &) {
    EXPECT_FALSE(v.corrupted_by_exception());
    EXPECT_EQ(1u, v.index());
    // We can still assign into a variant in an invalid state.
    v = 42;
    // Check `v`.
    EXPECT_FALSE(v.corrupted_by_exception());
    EXPECT_EQ(42, exp::get<int>(v));
  }  // try
}

TEST(Assign_Conversion, ThrowOnTemporaryConstruction) {
  exp::variant<int, copy_thrower_t> v(42);
  try {
    // Since `copy_thrower_t`'s copy constructor always throws, we will fail to
    // construct the temporary object. This results in our variant staying in
    // its original state.
    copy_thrower_t copy_thrower{};
    v = copy_thrower;
  } catch (const std::exception &) {
    EXPECT_FALSE(v.corrupted_by_exception());
    EXPECT_EQ(0u, v.index());
    EXPECT_EQ(42, exp::get<int>(v));
  }  // try
}

TEST(Assign_Conversion, ThrowOnVariantConstruction) {
  exp::variant<int, move_thrower_t> v(42);
  try {
    // Since `move_thrower_t`'s copy constructor never throws, we successfully
    // construct the temporary object by copying `move_thrower_t`. We then
    // proceed to move the temporary object into our variant, at which point
    // `move_thrower_t`'s move constructor throws. This results in our `variant`
    // transitioning into the invalid state.
    move_thrower_t move_thrower;
    v = move_thrower;
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    // We can still assign into a variant in an invalid state.
    v = 42;
    // Check `v`.
    EXPECT_FALSE(v.corrupted_by_exception());
    EXPECT_EQ(42, exp::get<int>(v));
  }  // try
}
