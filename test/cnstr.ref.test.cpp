#include <string>
#include <sstream>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(Direct, LRef) {
  std::string x("hello"s);
  mpark::variant<int &, std::string &> v(x);
  // Check `x`.
  EXPECT_EQ("hello"s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
}

TEST(Direct, RRef) {
  static_assert(
      !std::is_constructible<mpark::variant<int &, std::string &>, int>{},
      "mpark::variant<int &, std::string &> v(42);");
}

TEST(Direct, Conversion) {
  std::ostringstream strm;
  // `std::ostringstream &` -> `std::ostream &` is the only valid conversion.
  mpark::variant<int &, std::ostream &> v(strm);
  // Check `v`.
  EXPECT_EQ(typeid(std::ostream &), v.type());
  mpark::get<std::ostream &>(v) << "hello"s;
  EXPECT_EQ("hello"s, strm.str());
}

TEST(Direct, InitializerList) {
  static_assert(
      !std::is_constructible<mpark::variant<int &, std::string &>,
                             std::initializer_list<char>>{},
      "mpark::variant<int &, std::string &> v({'h', 'e', 'l', 'l', 'o'});");
}

TEST(Direct, ExactMatch) {
  std::ostringstream strm;
  mpark::variant<std::ostringstream &, std::ostream &> v(strm);
  // Check `v`.
  EXPECT_EQ(typeid(std::ostringstream &), v.type());
  mpark::get<std::ostringstream &>(v) << "hello"s;
  EXPECT_EQ("hello"s, strm.str());
}

TEST(Direct, BetterMatch) {
  std::stringstream strm;
  // `std::stringstream &` -> `std::iostream &` is better than
  // `std::stringstream &` -> `std::ostream &`
  mpark::variant<std::iostream &, std::ostream &> v(strm);
  // Check `v`.
  EXPECT_EQ(typeid(std::iostream &), v.type());
  mpark::get<std::iostream &>(v) << "hello"s;
  EXPECT_EQ("hello"s, strm.str());
}

TEST(Direct, NoMatch) {
  struct X {};
  static_assert(
      !std::is_constructible<mpark::variant<int &, std::string &>, X &>{},
      "X x; mpark::variant<int &, std::string &> v(x);");
}

TEST(Direct, Ambiguous) {
  static_assert(
      !std::is_constructible<mpark::variant<std::istream &, std::ostream &>,
                             std::stringstream &>{},
      "std::stringstream strm;\n"
      "mpark::variant<std::istream &, std::ostream &> v(strm);");
}

TEST(InPlace, Emplace) {
  std::stringstream strm;
  mpark::variant<std::iostream &, std::ostream &> v(
      mpark::in_place<std::ostream &>, strm);
  // Check `v`.
  EXPECT_EQ(typeid(std::ostream &), v.type());
  mpark::get<std::ostream &>(v) << "hello"s;
  EXPECT_EQ("hello"s, strm.str());
}

TEST(InPlace, LRef) {
  std::string x("hello"s);
  mpark::variant<int &, std::string &> v(mpark::in_place<std::string &>, x);
  // Check `x`.
  EXPECT_EQ("hello"s, x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
}

TEST(InPlace, RRef) {
  // error: static_assert failed ""
  //        static_assert(std::is_constructible<T, Args &&...>{}, "");
  //                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // mpark::variant<int &, std::string &> v(mpark::in_place<std::string &>,
  //                                        "hello"s);
}

TEST(InPlace, Conversion) {
  // `std::ostringstream &` -> `std::ostream &` is the only valid conversion.
  std::ostringstream strm;
  mpark::variant<int &, std::ostream &> v(
      mpark::in_place<std::ostream &>, strm);
  // Check `v`.
  EXPECT_EQ(typeid(std::ostream &), v.type());
  mpark::get<std::ostream &>(v) << "hello"s;
  EXPECT_EQ("hello"s, strm.str());
}

TEST(InPlace, InitializerList) {
  // error: no matching constructor for initialization of
  //        'mpark::variant<int &, std::string &>'
  // mpark::variant<int &, std::string &> v(mpark::in_place<std::string &>,
  //                                        {'h', 'e', 'l', 'l', 'o'});
}

TEST(Variant, Default) {
  static_assert(
      !std::is_default_constructible<mpark::variant<int &, std::string &>>{},
      "mpark::variant<int &, std::string &> v;");
}

TEST(Variant, Copy) {
  std::string x("hello"s);
  mpark::variant<int &, std::string &> v(x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  // Copy from `v`.
  mpark::variant<int &, std::string &> w(v);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}

TEST(Variant, Move) {
  std::string x("hello"s);
  mpark::variant<int &, std::string &> v(x);
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  // Move from `v`.
  mpark::variant<int &, std::string &> w(std::move(v));
  // Check `v`.
  EXPECT_EQ(typeid(std::string &), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(v));
  // Check `w`.
  EXPECT_EQ(typeid(std::string &), w.type());
  EXPECT_EQ("hello"s, mpark::get<std::string &>(w));
}

TEST(Nullable, Default) {
  // Nullable `v`.
  mpark::variant<mpark::null_t, int &, std::string &> v;
  EXPECT_EQ(typeid(mpark::null_t), v.type());
  // Nullable `w`.
  mpark::variant<int &, mpark::null_t, std::string &> w;
  EXPECT_EQ(typeid(mpark::null_t), w.type());
  // Nullable `x`.
  mpark::variant<int &, std::string &, mpark::null_t> x;
  EXPECT_EQ(typeid(mpark::null_t), x.type());
}
