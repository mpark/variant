// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_DETAIL_AS_VARIANT_BASE_HPP
#define EXPERIMENTAL_VARIANT_DETAIL_AS_VARIANT_BASE_HPP

namespace std {
namespace experimental {
namespace detail {

/* `as_variant_base` */
template <typename... Ts>
const variant_base<Ts...> &as_variant_base_impl(const variant_base<Ts...> &v);

template <typename... Ts>
variant_base<Ts...> &as_variant_base_impl(variant_base<Ts...> &v);

template <typename... Ts>
const variant_base<Ts...> &&as_variant_base_impl(const variant_base<Ts...> &&v);

template <typename... Ts>
variant_base<Ts...> &&as_variant_base_impl(variant_base<Ts...> &&v);

template <typename V>
using as_variant_base = decltype(as_variant_base_impl(declval<V>()));

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_DETAIL_AS_VARIANT_BASE_HPP
