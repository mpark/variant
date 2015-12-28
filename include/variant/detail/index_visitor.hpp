// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_INDEX_VISITOR_HPP
#define VARIANT_DETAIL_INDEX_VISITOR_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include <variant/detail/apply.hpp>
#include <variant/detail/type_traits.hpp>

namespace std {
namespace experimental {
namespace detail {

/* `index_visitor` */

template <template <size_t...> class F, typename... Args>
struct index_visitor {
  template <size_t... Is>
  auto operator()(size_constant<Is>...) && {
    return apply(
        [](Args... args) { return F<Is...>{static_cast<Args>(args)...}; },
        move(args_));
  }

  tuple<Args...> args_;
};  // index_visitor

template <template <size_t...> class F, typename... Args>
index_visitor<F, Args &&...> make_index_visitor(Args &&... args) {
  return {forward_as_tuple(forward<Args>(args)...)};
}

/* `is_index_visitor` */

template <typename T>
struct is_index_visitor : false_type {};

template <template <size_t...> class F, typename... Args>
struct is_index_visitor<index_visitor<F, Args...>> : true_type {};

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_INDEX_VISITOR_HPP
