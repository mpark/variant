// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_UNSAFE_GET_HPP
#define VARIANT_DETAIL_UNSAFE_GET_HPP

#include <cassert>

#include <meta/meta.hpp>

#include <variant/detail/qualify_as.hpp>

namespace std {
namespace experimental {
namespace detail {

// Forward declaration.
template <typename... Ts>
class variant_base;

namespace unsafe {

struct get_impl {
  template <size_t I, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = meta::as_list<decay_t<V>>;
    static_assert(I < alternatives::size(), "");
    using T = meta::at_c<alternatives, I>;
    assert(!v.corrupted_by_exception());
    using R = meta::_t<qualify_as<T, V &&>>;
    return static_cast<R>(forward<V>(v).storage_[meta::size_t<I>{}]);
  }

  template <typename T, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = meta::as_list<decay_t<V>>;
    static_assert(meta::count<alternatives, T>{} == 1, "");
    return get<meta::find_index<alternatives, T>{}>(forward<V>(v));
  }
};  // get_impl

template <size_t I, typename... Ts>
constexpr auto &&get(const variant_base<Ts...> &v) {
  return get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(variant_base<Ts...> &v) {
  return get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(const variant_base<Ts...> &&v) {
  return get_impl::get<I>(move(v));
}

template <size_t I, typename... Ts>
constexpr auto &&get(variant_base<Ts...> &&v) {
  return get_impl::get<I>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(const variant_base<Ts...> &v) {
  return get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(variant_base<Ts...> &v) {
  return get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(const variant_base<Ts...> &&v) {
  return get_impl::get<T>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(variant_base<Ts...> &&v) {
  return get_impl::get<T>(move(v));
}

}  // namespace unsafe

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_UNSAFE_GET_HPP
