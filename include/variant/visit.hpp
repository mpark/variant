// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_VISIT_HPP
#define VARIANT_VISIT_HPP

#include <utility>

#include <variant/detail/vtable.hpp>

namespace std {
namespace experimental {

template <typename F, typename... Vs>
decltype(auto) visit(F &&f, Vs &&... vs) {
  static constexpr auto vtable = detail::make_vtable<F &&, Vs &&...>();
  return detail::at(vtable, {vs.index()...})(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_VISIT_HPP
