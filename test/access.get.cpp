#include <variant.hpp>

#include <gtest/gtest.h>

namespace exp = std::experimental;

enum class Qualifier { Ptr, ConstPtr, LRef, ConstLRef, RRef, ConstRRef };

Qualifier get_qualifier(int *) { return Qualifier::Ptr; }
Qualifier get_qualifier(const int *) { return Qualifier::ConstPtr; }
Qualifier get_qualifier(int &) { return Qualifier::LRef; }
Qualifier get_qualifier(const int &) { return Qualifier::ConstLRef; }
Qualifier get_qualifier(int &&) { return Qualifier::RRef; }
Qualifier get_qualifier(const int &&) { return Qualifier::ConstRRef; }

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) { throw std::runtime_error(""); }
};  // move_thrower_t

TEST(Access_Get, MutVarMutType) {
  exp::variant<int> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::LRef, get_qualifier(exp::get<int>(v)));
  EXPECT_EQ(Qualifier::RRef, get_qualifier(exp::get<int>(std::move(v))));
}

TEST(Access_Get, MutVarConstType) {
  exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, get_qualifier(exp::get<const int>(v)));
  EXPECT_EQ(Qualifier::ConstRRef,
            get_qualifier(exp::get<const int>(std::move(v))));
}

TEST(Access_Get, ConstVarMutType) {
  const exp::variant<int> v(42);
  EXPECT_EQ(42, exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, get_qualifier(exp::get<int>(v)));
  EXPECT_EQ(Qualifier::ConstRRef, get_qualifier(exp::get<int>(std::move(v))));
}

TEST(Access_Get, ConstVarConstType) {
  const exp::variant<const int> v(42);
  EXPECT_EQ(42, exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, get_qualifier(exp::get<const int>(v)));
  EXPECT_EQ(Qualifier::ConstRRef,
            get_qualifier(exp::get<const int>(std::move(v))));
}

TEST(Access_Get, MoveCorruptedByException) {
  exp::variant<int, move_thrower_t> v(42);
  try {
    v = move_thrower_t{};
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_THROW(exp::get<int>(v), exp::bad_variant_access);
    EXPECT_THROW(exp::get<move_thrower_t>(v), exp::bad_variant_access);
  }  // try
}

TEST(Access_Get, CopyCorruptedByException) {
  exp::variant<int, move_thrower_t> v(42);
  try {
    move_thrower_t move_thrower;
    v = move_thrower;
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_THROW(exp::get<int>(v), exp::bad_variant_access);
    EXPECT_THROW(exp::get<move_thrower_t>(v), exp::bad_variant_access);
  }  // try
}
