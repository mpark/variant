#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Variant, Diamond) {
  // Valid type.
  auto dummy = [](const mpark::variant<> &) {};
  (void)dummy;
  // Not constructible.
  static_assert(!std::is_default_constructible<mpark::variant<>>{},
                "mpark::variant<> v;");
}
