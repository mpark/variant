// MPark.Variant
//
// Copyright Michael Park, 2015-2016
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <gtest/gtest.h>

struct Obj {
  Obj(bool &dtor_called) : dtor_called_(dtor_called) {}
  ~Obj() { dtor_called_ = true; }
  bool &dtor_called_;
};  // Obj

TEST(Dtor, Value) {
  bool dtor_called = false;
  // Construct/Destruct `Obj`.
  {
    mpark::variant<Obj> v(mpark::in_place_type<Obj>, dtor_called);
  }
  // Check that the destructor was called.
  EXPECT_TRUE(dtor_called);
}
