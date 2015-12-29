// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_DETAIL_VARIANT_VTABLE_HPP
#define EXPERIMENTAL_DETAIL_VARIANT_VTABLE_HPP

#include <array>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <experimental/detail/invoke.hpp>
#include <experimental/detail/type_traits.hpp>
#include <experimental/detail/variant/index_visitor.hpp>
#include <experimental/variant/unsafe/get.hpp>

namespace std {
namespace experimental {
namespace detail {
namespace variant {

/* `make_array` */

template <typename... Ts, typename T = same_type_t<Ts...>>
constexpr auto make_array_(priority<1>, Ts &&... ts) {
  return array<T, sizeof...(Ts)>{{forward<Ts>(ts)...}};
}

template <typename... Ts>
constexpr auto make_array_(priority<0>, Ts &&...) {
  static_assert(false_t<Ts...>{},
                "The return types of all handlers must be equivalent for "
                "return type deduction of `type_switch`.");
}

template <typename... Ts>
constexpr decltype(auto) make_array(Ts &&... ts) {
  return make_array_(priority<1>{}, forward<Ts>(ts)...);
}

/* `at` */

template <typename T>
constexpr const T &at_(const T &elem, const size_t *) {
  return elem;
}

template <typename T, size_t N>
constexpr auto &&at_(const array<T, N> &elems, const size_t *index) {
  return at_(elems[*index], index + 1);
}

template <typename T, size_t N>
constexpr auto &&at(const array<T, N> &elems, initializer_list<size_t> list) {
  return at_(elems, begin(list));
}

/* `make_vtable` */

template <typename... Vs>
struct vtable {
  template <typename F, size_t... Is>
  static constexpr decltype(auto) dispatch_(true_type, F f, Vs... vs) {
    return invoke(static_cast<F>(f)(size_constant<Is>{}...),
                  unsafe::get<Is>(static_cast<Vs>(vs))...);
  }

  template <typename F, size_t... Is>
  static constexpr decltype(auto) dispatch_(false_type, F f, Vs... vs) {
    return invoke(static_cast<F>(f), unsafe::get<Is>(static_cast<Vs>(vs))...);
  }

  template <typename F, size_t... Is>
  static constexpr decltype(auto) dispatch(F f, Vs... vs) {
    return dispatch_<F, Is...>(is_index_visitor<decay_t<F>>{},
                               static_cast<F>(f),
                               static_cast<Vs>(vs)...);
  }

  template <typename F, size_t... Is>
  static constexpr auto make(index_sequence<Is...>) {
    return &dispatch<F, Is...>;
  }

  template <typename F, size_t... Is, size_t... Js, typename... Ls>
  static constexpr auto make(index_sequence<Is...>,
                             index_sequence<Js...>,
                             Ls... ls) {
    return make_array(make<F>(index_sequence<Is..., Js>{}, ls...)...);
  }
};  // vtable

template <typename F, typename... Vs>
constexpr auto make_vtable() {
  return vtable<Vs...>::template make<F>(
      index_sequence<>{},
      make_index_sequence<experimental::tuple_size<
          repack_t<decay_t<Vs>, experimental::variant>>{}>{}...);
}

}  // namespace variant
}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_DETAIL_VARIANT_VTABLE_HPP
