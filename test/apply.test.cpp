#include <string>
#include <sstream>
#include <vector>

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

TEST(LRef, MutableVariantMutableType) {
  mpark::variant<int, std::string> v("hello"s);
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  struct set {
    void operator()(int &elem) const { elem = 24; }
    void operator()(std::string &elem) const { elem = "world"; }
  };
  mpark::apply(set(), v);
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("world"s, mpark::get<std::string>(v));
}

TEST(LRef, MutableVariantConstType) {
  mpark::variant<const int, const std::string> v(42);
  EXPECT_EQ(typeid(const int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(v));
  EXPECT_EQ("42"s,
            mpark::apply([](const auto &elem) {
              return (std::ostringstream() << elem).str();
            }, v));
}

TEST(LRef, ConstVariantMutableType) {
  const mpark::variant<int, std::string> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  EXPECT_EQ("42"s,
            mpark::apply([](const auto &elem) {
              return (std::ostringstream() << elem).str();
            }, v));
}

TEST(RRef, MutableVariantMutableType) {
  mpark::variant<int, std::string> v("hello"s);
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ("hello"s, mpark::get<std::string>(v));
  struct move {
    void operator()(int &&) const { ASSERT_TRUE(false); }
    void operator()(std::string &&elem) const { result = std::move(elem); }
    std::string &result;
  };
  std::string result;
  mpark::apply(move{result}, std::move(v));
  // Check `v`.
  EXPECT_EQ(typeid(std::string), v.type());
  EXPECT_EQ(""s, mpark::get<std::string>(v));
  // Check `result`.
  EXPECT_EQ("hello"s, result);
}

TEST(RRef, MutableVariantConstType) {
  mpark::variant<const int, const std::string> v(42);
  EXPECT_EQ(typeid(const int), v.type());
  EXPECT_EQ(42, mpark::get<const int>(v));
  EXPECT_EQ("42"s,
            mpark::apply([](const auto &&elem) {
              return (std::ostringstream() << elem).str();
            }, std::move(v)));
}

TEST(RRef, ConstVariantMutableType) {
  const mpark::variant<int, std::string> v(42);
  EXPECT_EQ(typeid(int), v.type());
  EXPECT_EQ(42, mpark::get<int>(v));
  EXPECT_EQ("42"s,
            mpark::apply([](const auto &&elem) {
              return (std::ostringstream() << elem).str();
            }, std::move(v)));
}

TEST(Homo, Double) {
  mpark::variant<int, std::string> v("hello"s), w("world!"s);
  EXPECT_EQ("hello world!"s,
            mpark::apply([](const auto &lhs, const auto &rhs) {
              return (std::ostringstream() << lhs << ' ' << rhs).str();
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
              return (std::ostringstream() << lhs << ' ' << rhs).str();
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
