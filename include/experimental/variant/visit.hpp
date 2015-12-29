// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_VISIT_HPP
#define EXPERIMENTAL_VARIANT_VISIT_HPP

#include <initializer_list>
#include <utility>

#include <experimental/detail/type_traits.hpp>
#include <experimental/variant/bad_variant_access.hpp>
#include <experimental/variant/unsafe/visit.hpp>

namespace std {
namespace experimental {

template <typename F, typename... Vs>
constexpr decltype(auto) visit(F &&f, Vs &&... vs) {
  return detail::any_of({vs.corrupted_by_exception()...}, true)
             ? throw bad_variant_access{}
             : unsafe::visit(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_VISIT_HPP
