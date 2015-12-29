// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_ACCESS_HPP
#define EXPERIMENTAL_VARIANT_ACCESS_HPP

#include <cassert>
#include <type_traits>

#include <experimental/tuple.hpp>
#include <experimental/variant/bad_variant_access.hpp>
#include <experimental/variant/detail/qualify_as.hpp>
#include <experimental/variant/detail/unsafe/get.hpp>

namespace std {
namespace experimental {

// Forward declaration.
template <typename... Ts>
class variant;

//- 20.N.7 element access:

template <size_t I, typename... Ts>
constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  return v.index() == I;
}

template <typename T, typename... Ts>
constexpr bool holds_alternative(const variant<Ts...> &v) noexcept {
  using alternatives = variant<Ts...>;
  static_assert(tuple_count<T, alternatives>{} == 1, "");
  return holds_alternative<tuple_find<T, alternatives>{}>(v);
}

namespace detail {

struct get_if_impl {
  template <size_t I, typename V>
  static constexpr auto *get_if(V *v) {
    using alternatives = decay_t<V>;
    static_assert(I < experimental::tuple_size<alternatives>{}, "");
    using T = experimental::tuple_element_t<I, alternatives>;
    assert(v);
    if (!holds_alternative<I>(*v)) {
      using R = add_pointer_t<qualify_as_t<T, V>>;
      return static_cast<R>(nullptr);
    }  // if
    return &unsafe::get<I>(*v);
  }

  template <typename T, typename V>
  static constexpr auto *get_if(V *v) {
    using alternatives = decay_t<V>;
    static_assert(tuple_count<T, alternatives>{} == 1, "");
    return get_if<tuple_find<T, alternatives>{}>(v);
  }
};  // get_if_impl

}  // namespace detail

template <size_t I, typename... Ts>
constexpr auto *get_if(const variant<Ts...> *v) noexcept {
  return detail::get_if_impl::get_if<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto *get_if(variant<Ts...> *v) noexcept {
  return detail::get_if_impl::get_if<I>(v);
}

template <typename T, typename... Ts>
constexpr auto *get_if(const variant<Ts...> *v) noexcept {
  return detail::get_if_impl::get_if<T>(v);
}

template <typename T, typename... Ts>
constexpr auto *get_if(variant<Ts...> *v) noexcept {
  return detail::get_if_impl::get_if<T>(v);
}

namespace detail {

struct get_impl {
  template <size_t I, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = decay_t<V>;
    using T = experimental::tuple_element_t<I, alternatives>;
    using R = qualify_as_t<T, V &&>;
    auto *result = get_if<I>(&v);
    return static_cast<R>(*(result ? result : throw bad_variant_access{}));
  }

  template <typename T, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = decay_t<V>;
    static_assert(tuple_count<T, alternatives>{} == 1, "");
    return get<tuple_find<T, alternatives>{}>(forward<V>(v));
  }
};  // get_impl

}  // namespace detail

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) {
  return detail::get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) {
  return detail::get_impl::get<I>(v);
}

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) {
  return detail::get_impl::get<I>(move(v));
}

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) {
  return detail::get_impl::get<I>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) {
  return detail::get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) {
  return detail::get_impl::get<T>(v);
}

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) {
  return detail::get_impl::get<T>(move(v));
}

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) {
  return detail::get_impl::get<T>(move(v));
}

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_ACCESS_HPP
