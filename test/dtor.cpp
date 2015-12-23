// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

struct Obj {
  Obj(bool &dtor_called) : dtor_called_(dtor_called) {}
  ~Obj() { dtor_called_ = true; }
  bool &dtor_called_;
};  // Obj

TEST(Dtor, Value) {
  bool dtor_called = false;
  // Construct/Destruct `Obj`.
  {
    std_exp::variant<Obj, Obj &> v(std_exp::in_place_type<Obj>, dtor_called);
  }
  // Check that the destructor was called.
  EXPECT_TRUE(dtor_called);
}

TEST(Dtor, Ref) {
  bool dtor_called = false;
  Obj obj(dtor_called);
  {
    std_exp::variant<Obj, Obj &> v(std_exp::in_place_type<Obj &>, obj);
  }
  // Check that the destructor was __not__ called.
  EXPECT_FALSE(dtor_called);
}
