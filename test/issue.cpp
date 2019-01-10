// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <mpark/variant.hpp>

#include <memory>
#include <vector>

#include <gtest/gtest.h>

// https://github.com/mpark/variant/pull/57
TEST(Issue, 57) {
  std::vector<mpark::variant<int, std::unique_ptr<int>>> vec;
  vec.emplace_back(0);
}
