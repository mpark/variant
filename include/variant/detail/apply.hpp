// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_APPLY_HPP
#define VARIANT_DETAIL_APPLY_HPP

#include <array>
#include <tuple>
#include <utility>

#include <variant/detail/invoke.hpp>

namespace std {
namespace experimental {
namespace detail {

/* `std::apply` */

template <typename F, typename Tuple, size_t... Is>
decltype(auto) apply_(F &&f, Tuple &&tuple, index_sequence<Is...>) {
  return invoke(forward<F>(f), get<Is>(forward<Tuple>(tuple))...);
}

template <typename F, typename Tuple>
decltype(auto) apply(F &&f, Tuple &&tuple) {
  return apply_(forward<F>(f),
                forward<Tuple>(tuple),
                make_index_sequence<tuple_size<decay_t<Tuple>>{}>{});
}

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_APPLY_HPP
