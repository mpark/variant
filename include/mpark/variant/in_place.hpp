#pragma once

#include <cstddef>

#include <meta/meta.hpp>

namespace mpark {

  //- 20.N.5 in-place construction:

  /* disambiguation tag type for in-place construction

     `in_place_t` is an empty class type used to disambiguate the
     overloads of constructors and member functions of that take arguments
     (possibly a parameter pack) for in-place construction of some value. */
  struct in_place_t {};

  /* an overloaded function for index-based in-place construction

     `in_place<I>` is an overloaded function used to disambiguate the overloads
     of constructors and member functions of that take arguments
     (possibly a parameter pack) for index-based in-place construction.

     `decltype(in_place<I>) == in_place_t (&)(meta::size_t<I>)` */
  template <std::size_t I> in_place_t in_place(meta::size_t<I>) { return {}; }

  /* an overloaded function for type-based in-place construction

     `in_place<T>` is an overloaded function used to disambiguate the overloads
     of constructors and member functions of that take arguments
     (possibly a parameter pack) for type-based in-place construction.

     `decltype(in_place<T>) == in_place_t (&)(meta::id<T>)` */
  template <typename T> in_place_t in_place(meta::id<T>) { return {}; }

}  // namespace mpark
