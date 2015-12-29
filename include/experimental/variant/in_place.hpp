// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_IN_PLACE_HPP
#define EXPERIMENTAL_VARIANT_IN_PLACE_HPP

#include <cstddef>

namespace std {
namespace experimental {

//- 20.N.M in-place construction:

/* `in_place_index_t<I>` is an empty class type used to disambiguate the
   overloads of constructors and member functions of that take arguments
   (possibly a parameter pack) for index-based in-place construction. */
template <size_t I> struct in_place_index_t {};
template <size_t I> constexpr in_place_index_t<I> in_place_index{};

/* `in_place_type_t<I>` is an empty class type used to disambiguate the
   overloads of constructors and member functions of that take arguments
   (possibly a parameter pack) for type-based in-place construction. */
template <class T> struct in_place_type_t {};
template <class T> constexpr in_place_type_t<T> in_place_type{};

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_IN_PLACE_HPP
