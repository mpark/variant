// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_UNSAFE_GET_HPP
#define VARIANT_DETAIL_UNSAFE_GET_HPP

#include <cassert>

#include <meta/meta.hpp>

namespace std {
namespace experimental {

template <typename... Ts> class variant;

namespace detail {
namespace unsafe {

struct impl {
  template <size_t I, typename V>
  static constexpr auto &&get(V &&v) {
    using members = meta::as_list<decay_t<V>>;
    static_assert(I < members::size(), "");
    assert(!v.corrupted_by_exception());
    return forward<V>(v)[meta::size_t<I>{}];
  }

  template <typename T, typename V>
  static constexpr auto &&get(V &&v) {
    using members = meta::as_list<decay_t<V>>;
    static_assert(meta::count<members, T>{} == 1, "");
    return get<meta::find_index<members, T>{}>(forward<V>(v));
  }
};  // impl

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) { return impl::get<I>(v); }

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) { return impl::get<I>(v); }

template <size_t I, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) { return impl::get<I>(move(v)); }

template <size_t I, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) { return impl::get<I>(move(v)); }

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &v) { return impl::get<T>(v); }

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &v) { return impl::get<T>(v); }

template <typename T, typename... Ts>
constexpr auto &&get(const variant<Ts...> &&v) { return impl::get<T>(move(v)); }

template <typename T, typename... Ts>
constexpr auto &&get(variant<Ts...> &&v) { return impl::get<T>(move(v)); }

}  // namespace unsafe
}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_UNSAFE_GET_HPP
