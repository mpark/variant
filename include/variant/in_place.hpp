#ifndef VARIANT_IN_PLACE_HPP
#define VARIANT_IN_PLACE_HPP

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

#endif  // VARIANT_IN_PLACE_HPP
