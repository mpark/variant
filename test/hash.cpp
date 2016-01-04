// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/monostate.hpp>
#include <experimental/variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Hash, Monostate) {
  std_exp::variant<int, std_exp::monostate, std::string> v(std_exp::monostate{});
  // Construct hash function objects.
  std::hash<std_exp::variant<int, std_exp::monostate, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(0u, variant_hash(v));
}

TEST(Hash, String) {
  std_exp::variant<int, std::string> v("hello"s);
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));
  // Construct hash function objects.
  std::hash<std::string> string_hash;
  std::hash<std_exp::variant<int, std::string>> variant_hash;
  // Check the hash.
  EXPECT_EQ(string_hash("hello"s), variant_hash(v));
}

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) = default;
};  // move_thrower_t

namespace std {

template <>
struct hash<move_thrower_t> {
  using argument_type = move_thrower_t;
  using result_type = size_t;

  result_type operator()(const argument_type &) const {
    EXPECT_TRUE(false);
    return 0u;
  }
};

}  // namespace std

TEST(Hash, CorruptedByException) {
  std_exp::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  EXPECT_TRUE(v.corrupted_by_exception());
  std::hash<std_exp::variant<int, move_thrower_t>> hash;
  EXPECT_EQ(std_exp::tuple_not_found, hash(v));
}
