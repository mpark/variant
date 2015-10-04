#pragma once

#include <tuple>
#include <utility>

#include <exp/functional.hpp>

namespace exp {

  /* `std::apply` (since C++17) */

  template <typename F, typename Tuple, size_t... Is>
  decltype(auto) apply_(F &&f, Tuple &&tuple, std::index_sequence<Is...>) {
    return exp::invoke(std::forward<F>(f),
                       std::get<Is>(std::forward<Tuple>(tuple))...);
  }

  template <typename F, typename Tuple>
  decltype(auto) apply(F &&f, Tuple &&tuple) {
    return apply_(
        std::forward<F>(f),
        std::forward<Tuple>(tuple),
        std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>{});
  }

}  // namespace exp
