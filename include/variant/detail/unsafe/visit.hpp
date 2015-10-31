// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_DETAIL_UNSAFE_VISIT_HPP
#define VARIANT_DETAIL_UNSAFE_VISIT_HPP

#include <utility>

#include <variant/detail/as_variant_base.hpp>
#include <variant/detail/vtable.hpp>

namespace std {
namespace experimental {
namespace detail {
namespace unsafe {

template <typename F, typename... Vs>
constexpr decltype(auto) visit(F &&f, Vs &&... vs) {
  constexpr auto vtable = make_vtable<F &&, as_variant_base<Vs &&>...>();
  return at(vtable, {vs.index()...})(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace unsafe
}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_UNSAFE_VISIT_HPP
