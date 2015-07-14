#include <string>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Direct, Copy) {
  std::string x("hello"s);
  mpark::variant<int, std::string> v(x);
  // Check `x`.
  EXPECT_EQ("hello"s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Direct, Move) {
  std::string x("hello"s);
  mpark::variant<int, std::string> v(std::move(x));
  // Check `x`.
  EXPECT_EQ(""s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Direct, Conversion) {
  // `const char * -> std::string` is the only valid conversion.
  mpark::variant<int, std::string> v("hello");
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Direct, InitializerList) {
  // `const char * -> std::string` is the only valid conversion.
  mpark::variant<int, std::string> v({'h', 'e', 'l', 'l', 'o'});
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(Direct, ExactMatch) {
  const char *x = "hello";
  mpark::variant<const char *, std::string> v(x);
  // Check `v`.
  EXPECT_EQ(typeid(const char *), v.type());
  EXPECT_EQ("hello", mpark::get<const char *>(v));
}

TEST(Direct, BetterMatch) {
  // `char -> int` is better than `char -> double`
  mpark::variant<int, double> v('x');
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(static_cast<int>('x'), mpark::get<int>(v));
}

TEST(Direct, NoMatch) {
  struct x {};
  static_assert(!std::is_constructible<mpark::variant<int, std::string>, x>{},
                "mpark::variant<int, std::string> v(x{});");
}

TEST(Direct, Ambiguous) {
  static_assert(!std::is_constructible<mpark::variant<short, long>, int>{},
                "mpark::variant<short, long> v(42);");
}

TEST(InPlace, Copy) {
  std::string x("hello"s);
  mpark::variant<const char *, std::string> v(mpark::in_place<std::string>, x);
  // Check `x`.
  EXPECT_EQ("hello"s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(InPlace, Move) {
  std::string x("hello"s);
  mpark::variant<const char *, std::string> v(mpark::in_place<std::string>,
                                              std::move(x));
  // Check `x`.
  EXPECT_EQ(""s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(InPlace, Conversion) {
  const char *x = "hello";
  mpark::variant<const char *, std::string> v(mpark::in_place<std::string>, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(InPlace, InitializerList) {
  mpark::variant<const char *, std::string> v(mpark::in_place<std::string>,
                                              {'h', 'e', 'l', 'l', 'o'});
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
}

TEST(InPlace, NoMatch) {
  static_assert(
      !std::is_constructible<mpark::variant<int, std::string>,
                             mpark::in_place_t<double>,
                             double>{},
      "mpark::variant<int, std::string> v(mpark::in_place<double>, 4.2);");
}

TEST(Variant, Default) {
  static_assert(
      !std::is_default_constructible<mpark::variant<int, std::string>>{},
      "mpark::variant<int, std::string> v;");
  // Nullable `v`.
  mpark::variant<mpark::null_t, int, std::string, mpark::null_t> v;
  EXPECT_EQ(typeid(mpark::null_t), v.type());
  // Nullable `w`.
  mpark::variant<int, mpark::null_t, std::string> w;
  EXPECT_EQ(typeid(mpark::null_t), w.type());
  // Nullable `x`.
  mpark::variant<int, std::string, mpark::null_t> x;
  EXPECT_EQ(typeid(mpark::null_t), x.type());
}

TEST(Variant, Copy) {
  mpark::variant<int, std::string> v("hello"s);
  mpark::variant<int, std::string> w(v);
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
}

TEST(Variant, Move) {
  mpark::variant<int, std::string> v("hello"s);
  mpark::variant<int, std::string> w(std::move(v));
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ(""s, mpark::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(w));
}

TEST(Variant, Diamond) {
  // Valid type.
  auto dummy = [](const mpark::variant<> &) {};
  (void)dummy;
  // Not constructible.
  static_assert(!std::is_default_constructible<mpark::variant<>>{},
                "mpark::variant<> v;");
}
