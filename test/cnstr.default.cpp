#include <mpark/variant.hpp>

#include <string>

#include <gtest/gtest.h>

TEST(DefaultCnstr, VariantTs) {
  mpark::variant<int, std::string> v;
  (void)v;
}

TEST(DefaultCnstr, Variant) {
  mpark::variant<> v;
  (void)v;
}
