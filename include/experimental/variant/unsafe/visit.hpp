// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_UNSAFE_VISIT_HPP
#define EXPERIMENTAL_VARIANT_UNSAFE_VISIT_HPP

#include <utility>

#include <experimental/detail/variant/as_variant_base.hpp>
#include <experimental/detail/variant/vtable.hpp>

namespace std {
namespace experimental {
namespace unsafe {

template <typename F, typename... Vs>
constexpr decltype(auto) visit(F &&f, Vs &&... vs) {
  using namespace detail::variant;
  constexpr auto vtable = make_vtable<F &&, as_variant_base<Vs &&>...>();
  return at(vtable, {vs.index()...})(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace unsafe
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_UNSAFE_VISIT_HPP
