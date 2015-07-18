#include <string>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Assign, SameLRef) {
  mpark::variant<int, std::string> v("hello world!"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `v`.
  std::string s("hello"s);
  v = s;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

TEST(Assign, SameRRef) {
  mpark::variant<int, std::string> v("hello world!"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `v`.
  v = "hello"s;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

TEST(Assign, SameConversion) {
  mpark::variant<int, std::string> v("hello world!"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `v`.
  v = "hello";
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

TEST(Assign, SameInitializerList) {
  mpark::variant<int, std::string> v("hello world!"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `v`.
  v = {'h', 'e', 'l', 'l', 'o'};
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

TEST(Assign, DifferentLRef) {
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Change types.
  std::string s("hello"s);
  v = s;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Assign, DifferentRRef) {
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Change types.
  v = "hello"s;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Assign, DifferentConversion) {
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Change types.
  v = "hello";
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Assign, DifferentInitializerList) {
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Use `std::string::operator=` to assign into `v` with
  // a `std::initializer_list<char>` -> `std::string` conversion.
  v = {'h', 'e', 'l', 'l', 'o'};
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Assign, ExactMatch) {
  mpark::variant<const char *, std::string> v("hello world");
  // Check `v`.
  EXPECT_EQ(typeid(const char *), v.type());
  EXPECT_EQ("hello world", mpark::get<const char *>(v));
  // Assign `v`.
  v = "hello";
  // Check `v`.
  EXPECT_EQ(typeid(const char *), v.type());
  EXPECT_EQ("hello", mpark::get<const char *>(v));
}

TEST(Assign, BetterMatch) {
  mpark::variant<int, double> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // `char` -> `int` is better than `char` -> `double`
  v = 'x';
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(static_cast<int>('x'), mpark::get<int>(v));
}

TEST(Assign, NoMatch) {
  struct x {};
  static_assert(!std::is_assignable<mpark::variant<int, std::string>, x>{},
                "mpark::variant<int, std::string> v(42); v = x;");
}

TEST(Assign, Ambiguous) {
  static_assert(!std::is_assignable<mpark::variant<short, long>, int>{},
                "mpark::variant<short, long> v(42); v = 42");
}

TEST(Variant, SameLRef) {
  // Construct `v`.
  mpark::variant<int, std::string> v("hello world"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Construct `w`.
  mpark::variant<int, std::string> w("hello"s);
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Assign from `w`.
  v = w;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
}

TEST(Variant, SameRRef) {
  // Construct `v`.
  mpark::variant<int, std::string> v("hello world"s);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &x = mpark::get<std::string>(v);
  EXPECT_EQ("hello world"s, x);
  // Save the "hello world"'s capacity.
  auto capacity = x.capacity();
  // Construct `w`.
  mpark::variant<int, std::string> w("hello"s);
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Assign from `w`.
  v = std::move(w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  const std::string &y = mpark::get<std::string>(v);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ(""s, mpark::get<std::string>(w));
}

TEST(Variant, DifferentLRef) {
  // Construct `v`.
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Construct `w`.
  mpark::variant<int, std::string> w("hello"s);
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Assign from `w`.
  v = w;
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
}

TEST(Variant, DifferentRRef) {
  // Construct `v`.
  mpark::variant<int, std::string> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Construct `w`.
  mpark::variant<int, std::string> w("hello"s);
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
  // Assign from `w`.
  v = std::move(w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ(""s, mpark::get<std::string>(w));
}

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
  } catch (const std::exception &ex) {
    EXPECT_STREQ("copy", ex.what());
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
  } catch (const std::exception &ex) {
    EXPECT_STREQ("copy", ex.what());
    EXPECT_FALSE(v.valid());
    // We can still assign into a variant in an invalid state.
    v = 42;
    // Check `v`.
    EXPECT_TRUE(v.valid());
    EXPECT_EQ(typeid(int), v.type());
    EXPECT_EQ(42, mpark::get<int>(v));
  }
}

TEST(Valid, ThrowOnVariantAssignment) {
  mpark::variant<int, thrower_t> v(mpark::in_place<thrower_t>);
  // Check `v`.
  EXPECT_TRUE(v.valid());
  EXPECT_EQ(typeid(thrower_t), v.type());
  try {
    // Since `variant` is already in `thrower_t`, assignment optimization kicks
    // and we simply invoke `thrower_t &thrower_t::operator=(thrower_t &&);`,
    // which throws.
    v = thrower_t{};
  } catch (const std::exception &ex) {
    EXPECT_STREQ("assign", ex.what());
    EXPECT_FALSE(v.valid());
    // We can still assign into a variant in an invalid state.
    v = 42;
    // Check `v`.
    EXPECT_TRUE(v.valid());
    EXPECT_EQ(typeid(int), v.type());
    EXPECT_EQ(42, mpark::get<int>(v));
  }
}
