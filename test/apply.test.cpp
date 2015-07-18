#include <string>
#include <sstream>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

enum class Qualifier { None, LRef, ConstLRef, RRef, ConstRRef };

struct get_qualifier {
  Qualifier operator()(int &) const { return Qualifier::LRef; }
  Qualifier operator()(const int &) const { return Qualifier::ConstLRef; }
  Qualifier operator()(int &&) const { return Qualifier::RRef; }
  Qualifier operator()(const int &&) const { return Qualifier::ConstRRef; }
};  // get_qualifier

TEST(LRef, MutableVariantMutableType) {
  mpark::variant<int> v(42);
  // Check `v`.
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::LRef, mpark::apply(get_qualifier(), v));
}

TEST(LRef, MutableVariantConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(typeid(const int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, mpark::apply(get_qualifier(), v));
}

TEST(LRef, ConstVariantMutableType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstLRef, mpark::apply(get_qualifier(), v));
}

TEST(RRef, MutableVariantMutableType) {
  mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::RRef, mpark::apply(get_qualifier(), std::move(v)));
}

TEST(RRef, MutableVariantConstType) {
  mpark::variant<const int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstRRef, mpark::apply(get_qualifier(), std::move(v)));
}

TEST(RRef, ConstVariantMutableType) {
  const mpark::variant<int> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(Qualifier::ConstRRef, mpark::apply(get_qualifier(), std::move(v)));
}

TEST(Homo, Double) {
  mpark::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            mpark::apply([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Homo, Quintuple) {
  mpark::variant<int, std::string> v(101), w("+"s), x(202), y("="s), z(303);
  EXPECT_EQ("101+202=303"s,
            mpark::apply([](const auto &... elems) {
              std::ostringstream strm;
              std::initializer_list<int>({(strm << elems, 0)...});
              return std::move(strm).str();
            }, v, w, x, y, z));
}

TEST(Hetero, Double) {
  mpark::variant<int, std::string> v("hello"s);
  mpark::variant<double, const char *> w("world!");
  EXPECT_EQ("hello world!"s,
            mpark::apply([](const auto &lhs, const auto &rhs) {
              std::ostringstream strm;
              strm << lhs << ' ' << rhs;
              return strm.str();
            }, v, w));
}

TEST(Hetero, Quintuple) {
  mpark::variant<int, double> v(101);
  mpark::variant<const char *> w("+");
  mpark::variant<bool, std::string, int> x(202);
  mpark::variant<char, std::string, const char *> y('=');
  mpark::variant<long, short> z(303L);
  auto actual = mpark::apply([](const auto &... elems) {
    std::ostringstream strm;
    std::initializer_list<int>({(strm << elems, 0)...});
    return std::move(strm).str();
  }, v, w, x, y, z);
  EXPECT_EQ("101+202=303"s, actual);
}
