// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_ACCESS_HPP
#define VARIANT_ACCESS_HPP

#include <cassert>
#include <type_traits>

#include <meta/meta.hpp>

#include <variant/bad_variant_access.hpp>
#include <variant/detail/qualify_as.hpp>
#include <variant/detail/unsafe/get.hpp>

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
  using alternatives = meta::list<Ts...>;
  static_assert(meta::count<alternatives, T>{} == 1, "");
  return holds_alternative<meta::find_index<alternatives, T>{}>(v);
}

namespace detail {

struct get_if_impl {
  template <size_t I, typename V>
  static constexpr auto *get_if(V *v) {
    using alternatives = meta::as_list<decay_t<V>>;
    static_assert(I < alternatives::size(), "");
    using T = meta::at_c<alternatives, I>;
    assert(v);
    if (!holds_alternative<I>(*v)) {
      using R = add_pointer_t<meta::_t<qualify_as<T, V>>>;
      return static_cast<R>(nullptr);
    }  // if
    return &unsafe::get<I>(*v);
  }

  template <typename T, typename V>
  static constexpr auto *get_if(V *v) {
    using alternatives = meta::as_list<decay_t<V>>;
    static_assert(meta::count<alternatives, T>{} == 1, "");
    return get_if<meta::find_index<alternatives, T>{}>(v);
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
    auto *result = get_if<I>(&v);
    if (!result) {
      throw bad_variant_access{};
    }  // if
    using alternatives = meta::as_list<decay_t<V>>;
    using T = meta::at_c<alternatives, I>;
    using R = meta::_t<qualify_as<T, V &&>>;
    return static_cast<R>(*result);
  }

  template <typename T, typename V>
  static constexpr auto &&get(V &&v) {
    using alternatives = meta::as_list<decay_t<V>>;
    static_assert(meta::count<alternatives, T>{} == 1, "");
    return get<meta::find_index<alternatives, T>{}>(forward<V>(v));
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

#endif  // VARIANT_ACCESS_HPP
