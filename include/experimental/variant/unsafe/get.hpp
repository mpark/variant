// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_UNSAFE_GET_HPP
#define EXPERIMENTAL_VARIANT_UNSAFE_GET_HPP

#include <cassert>

#include <experimental/detail/qualify_as.hpp>
#include <experimental/detail/type_traits.hpp>
#include <experimental/tuple.hpp>

namespace std {
namespace experimental {

// Forward declaration.
template <typename... Ts>
class variant;

namespace detail {
namespace variant {

// Forward declaration.
template <typename... Ts>
class variant_base;

}  // namespace variant
}  // namespace detail

namespace unsafe {

struct get_impl {
  template <size_t I, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = detail::repack_t<decay_t<V>, variant>;
    static_assert(I < experimental::tuple_size<alternatives>{}, "");
    using T = experimental::tuple_element_t<I, alternatives>;
    assert(!v.corrupted_by_exception());
    using R = detail::qualify_as_t<T, V &&>;
    return static_cast<R>(forward<V>(v).storage_[detail::size_constant<I>{}]);
  }

  template <typename T, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = detail::repack_t<decay_t<V>, variant>;
    static_assert(tuple_count<T, alternatives>{} == 1, "");
    return get<tuple_find<T, alternatives>{}>(forward<V>(v));
  }
};  // get_impl

template <size_t I, typename... Ts>
constexpr auto &&get(const detail::variant::variant_base<Ts...> &v) {
  return get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(detail::variant::variant_base<Ts...> &v) {
  return get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(const detail::variant::variant_base<Ts...> &&v) {
  return get_impl::get<I>(move(v));
}

template <size_t I, typename... Ts>
constexpr auto &&get(detail::variant::variant_base<Ts...> &&v) {
  return get_impl::get<I>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(const detail::variant::variant_base<Ts...> &v) {
  return get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(detail::variant::variant_base<Ts...> &v) {
  return get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(const detail::variant::variant_base<Ts...> &&v) {
  return get_impl::get<T>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(detail::variant::variant_base<Ts...> &&v) {
  return get_impl::get<T>(move(v));
}

}  // namespace unsafe

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_UNSAFE_GET_HPP
