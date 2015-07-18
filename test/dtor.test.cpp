#include <mpark/variant.hpp>

#include <gtest/gtest.h>

TEST(Variant, Destructor) {
  enum class Type { None, X, Y };
  struct X {
    X(Type &type) : type_(type) {}
    ~X() { type_ = Type::X; }
    Type &type_;
  };  // X
  struct Y {
    Y(Type &type) : type_(type) {}
    ~Y() { type_ = Type::Y; }
    Type &type_;
  };  // Y
  Type type = Type::None;
  // Construct/Destruct X.
  {
    mpark::variant<X, Y> v(mpark::in_place<X>, type);
    // Check `v`.
    EXPECT_EQ(typeid(X), v.type());
  }
  // Check `type`.
  EXPECT_EQ(Type::X, type);
  // Construct/Destruct X.
  {
    mpark::variant<X, Y> v(mpark::in_place<Y>, type);
    // Check `v`.
    EXPECT_EQ(typeid(Y), v.type());
  }
  // Check `type`.
  EXPECT_EQ(Type::Y, type);
}

