#include <string>
#include <sstream>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Assign, SameLRef) {
  std::string a("hello world!"s);
  mpark::variant<int &, std::string &> v(a);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &x = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world!"'s capacity.
  auto capacity = x.capacity();
  // Use `std::string::operator=` to assign into `a`.
  std::string b("hello"s);
  v = b;
  // Check `a`.
  EXPECT_EQ("hello"s, a);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &y = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &y);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world!", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
}

TEST(Assign, SameRRef) {
  static_assert(
      !std::is_assignable<mpark::variant<int &, std::string &>, std::string>{},
      "std::string x(\"hello world!\"s);\n"
      "mpark::variant<int &, std::string &> v(x);\n"
      "v = \"hello\"s;");
}

TEST(Assign, DifferentLRef) {
  int x = 42;
  mpark::variant<int &, std::string &> v(x);
  // Check `v`.
  EXPECT_EQ(typeid(int &), v.type());
  EXPECT_EQ(42, mpark::get<int &>(v));
  // Change types.
  std::string y("hello"s);
  v = y;
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
}

TEST(Assign, DifferentRRef) {
  static_assert(
      !std::is_assignable<mpark::variant<int &, std::string &>, std::string>{},
      "int x = 42;\n"
      "mpark::variant<int &, std::string &> v(x);\n"
      "v = \"hello\"s;");
}

TEST(Assign, NoMatch) {
  struct X {};
  static_assert(
      !std::is_assignable<mpark::variant<int &, std::string &>, X &>{},
      "X x;\n"
      "int y = 42;\n"
      "mpark::variant<int &, std::string &> v(y);\n"
      "v = x;");
}

TEST(Assign, Ambiguous) {
  static_assert(
      !std::is_assignable<mpark::variant<std::istream &, std::ostream &>,
                          std::stringstream &>{},
      "std::stringstream strm;\n"
      "mpark::variant<std::istream &, std::ostream &> v(std::cout);\n"
      "v = strm");
}

TEST(Variant, SameCopy) {
  // Construct `v`.
  std::string a("hello world!"s);
  mpark::variant<int &, std::string &> v(a);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &x = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world!"'s capacity.
  auto capacity = x.capacity();
  // Construct `w`.
  std::string b("hello"s);
  mpark::variant<int &, std::string &> w(b);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
  // Assign from `w`.
  v = w;
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &y = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &y);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world!", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}

TEST(Variant, SameMove) {
  // Construct `v`.
  std::string a("hello world!"s);
  mpark::variant<int &, std::string &> v(a);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &x = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ("hello world!"s, x);
  // Save the "hello world!"'s capacity.
  auto capacity = x.capacity();
  // Construct `w`.
  std::string b("hello"s);
  mpark::variant<int &, std::string &> w(b);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
  // Assign from `w`.
  v = std::move(w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &y = mpark::get<std::string &>(v);
  EXPECT_EQ(&a, &y);
  EXPECT_EQ("hello"s, y);
  // Since "hello" is shorter than "hello world!", we should have preserved the
  // existing capacity of the string!.
  EXPECT_EQ(capacity, y.capacity());
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}

TEST(Variant, DifferentCopy) {
  // Construct `v`.
  int a = 42;
  mpark::variant<int &, std::string &> v(a);
  // Check `v`.
  EXPECT_EQ(typeid(int &), v.type());
  const int &x = mpark::get<int &>(v);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ(42, x);
  // Construct `w`.
  std::string b("hello"s);
  mpark::variant<int &, std::string &> w(b);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
  // Assign from `w`.
  v = w;
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &y = mpark::get<std::string &>(v);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ("hello"s, y);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}

TEST(Variant, DifferentMove) {
  // Construct `v`.
  int a = 42;
  mpark::variant<int &, std::string &> v(a);
  // Check `v`.
  EXPECT_EQ(typeid(int &), v.type());
  const int &x = mpark::get<int &>(v);
  EXPECT_EQ(&a, &x);
  EXPECT_EQ(42, x);
  // Construct `w`.
  std::string b("hello"s);
  mpark::variant<int &, std::string &> w(b);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
  // Assign from `w`.
  v = std::move(w);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  const std::string &y = mpark::get<std::string &>(v);
  EXPECT_EQ(&b, &y);
  EXPECT_EQ("hello"s, y);
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}
