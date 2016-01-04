// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <experimental/variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Swap, Same) {
  std_exp::variant<int, std::string> v("hello"s);
  std_exp::variant<int, std::string> w("world"s);
  // Check `v`.
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("world"s, std_exp::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ("world"s, std_exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ("hello"s, std_exp::get<std::string>(w));
}

TEST(Swap, Different) {
  std_exp::variant<int, std::string> v(42);
  std_exp::variant<int, std::string> w("hello"s);
  // Check `v`.
  EXPECT_EQ(42, std_exp::get<int>(v));
  // Check `w`.
  EXPECT_EQ("hello"s, std_exp::get<std::string>(w));
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`.
  EXPECT_EQ("hello"s, std_exp::get<std::string>(v));
  // Check `w`.
  EXPECT_EQ(42, std_exp::get<int>(w));
}

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) = default;
};  // move_thrower_t

TEST(Swap, OneCorruptedByException) {
  // `v` normal, `w` corrupted.
  std_exp::variant<int, move_thrower_t> v(42), w(42);
  EXPECT_THROW(w = move_thrower_t{}, std::runtime_error);
  EXPECT_EQ(42, std_exp::get<int>(v));
  EXPECT_TRUE(w.corrupted_by_exception());
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`, `w`.
  EXPECT_TRUE(v.corrupted_by_exception());
  EXPECT_EQ(42, std_exp::get<int>(w));
}

TEST(Swap, BothCorruptedByException) {
  // `v`, `w` both corrupted.
  std_exp::variant<int, move_thrower_t> v(42);
  EXPECT_THROW(v = move_thrower_t{}, std::runtime_error);
  std_exp::variant<int, move_thrower_t> w(v);
  EXPECT_TRUE(v.corrupted_by_exception());
  EXPECT_TRUE(w.corrupted_by_exception());
  // Swap.
  using std::swap;
  swap(v, w);
  // Check `v`, `w`.
  EXPECT_TRUE(v.corrupted_by_exception());
  EXPECT_TRUE(w.corrupted_by_exception());
}

TEST(Swap, DtorsSame) {
  struct Obj {
    Obj(size_t *dtor_count) : dtor_count_(dtor_count) {}
    ~Obj() { ++(*dtor_count_); }
    size_t *dtor_count_;
  };  // Obj
  size_t v_count = 0;
  size_t w_count = 0;
  {
    std_exp::variant<Obj> v{&v_count}, w{&w_count};
    using std::swap;
    swap(v, w);
    // Calls `std::swap(Obj &lhs, Obj &rhs)`, with which we perform:
    // ```
    // {
    //   Obj temp(move(lhs));
    //   lhs = move(rhs);
    //   rhs = move(temp);
    // }  `++v_count` from `temp::~Obj()`.
    // ```
    EXPECT_EQ(1u, v_count);
    EXPECT_EQ(0u, w_count);
  }
  EXPECT_EQ(2u, v_count);
  EXPECT_EQ(1u, w_count);
}

namespace detail {

struct Obj {
  Obj(size_t *dtor_count) : dtor_count_(dtor_count) {}
  ~Obj() { ++(*dtor_count_); }
  size_t *dtor_count_;
};  // Obj

void swap(Obj &lhs, Obj &rhs) { std::swap(lhs.dtor_count_, rhs.dtor_count_); }

}  // namespace detail

TEST(Swap, DtorsSameWithSwap) {
  size_t v_count = 0;
  size_t w_count = 0;
  {
    std_exp::variant<detail::Obj> v{&v_count}, w{&w_count};
    using std::swap;
    swap(v, w);
    // Calls `detail::swap(Obj &lhs, Obj &rhs)`, with which doesn't call any destructors.
    EXPECT_EQ(0u, v_count);
    EXPECT_EQ(0u, w_count);
  }
  EXPECT_EQ(1u, v_count);
  EXPECT_EQ(1u, w_count);
}

TEST(Swap, DtorsDifferent) {
  struct V {
    V(size_t *dtor_count) : dtor_count_(dtor_count) {}
    ~V() { ++(*dtor_count_); }
    size_t *dtor_count_;
  };  // V
  struct W {
    W(size_t *dtor_count) : dtor_count_(dtor_count) {}
    ~W() { ++(*dtor_count_); }
    size_t *dtor_count_;
  };  // W
  size_t v_count = 0;
  size_t w_count = 0;
  {
    std_exp::variant<V, W> v{std_exp::in_place_type<V>, &v_count}, w{std_exp::in_place_type<W>, &w_count};
    using std::swap;
    swap(v, w);
    // Calls `std::swap(std_exp::variant<V, W> &, std_exp::variant<V, W> &)`,
    // with which we perform:
    // ```
    // {
    //   std_exp::variant<V, W> temp(move(v));
    //   v = move(w);     // `++v_count`
    //   w = move(temp);  // `++w_count`
    // }  // `++v_count`
    // ```
    EXPECT_EQ(2u, v_count);
    EXPECT_EQ(1u, w_count);
  }
  EXPECT_EQ(3u, v_count);
  EXPECT_EQ(2u, w_count);
}
