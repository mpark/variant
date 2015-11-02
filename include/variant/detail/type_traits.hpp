// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_TYPE_TRAITS_HPP
#define VARIANT_DETAIL_TYPE_TRAITS_HPP

#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

namespace std {
namespace experimental {
namespace detail {

/* `overload_set` */

template <typename List>
struct overload_set;

template <>
struct overload_set<meta::list<>> {
  void operator()() const;
};  // overload_set<meta::list<>>

template <typename T, typename... Ts>
struct overload_set<meta::list<T, Ts...>> : overload_set<meta::list<Ts...>> {
  using super = overload_set<meta::list<Ts...>>;

  using super::operator();

  meta::id<T> operator()(T) const;

  template <typename U = T, typename = meta::if_<is_lvalue_reference<U>>>
  void operator()(remove_reference_t<T> &&) const = delete;
};  // overload_set<meta::list<T, Ts...>>

/* `get_best_match` */
template <typename List, typename T>
using get_best_match = meta::_t<result_of_t<overload_set<List>(T)>>;

/* `false_t` */
template <typename... Ts>
struct false_t : false_type {};

/* `is_nothrow_swappable` (N4511) */

template <typename T, typename U = T>
struct is_nothrow_swappable
    : meta::bool_<noexcept(swap(declval<T &>(), declval<U &>()))> {};

/* `priority` */

template <size_t I>
struct priority;

template <>
struct priority<0> {};

template <size_t I>
struct priority : priority<I - 1> {};

/* `same_type` (similar to `std::common_type`) */

template <typename... Ts>
struct same_type;

template <>
struct same_type<> {};

template <typename T, typename... Ts>
struct same_type<T, Ts...> : enable_if<meta::and_<is_same<T, Ts>...>{}, T> {};

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_TYPE_TRAITS_HPP
