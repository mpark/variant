// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_VISIT_HPP
#define VARIANT_VISIT_HPP

#include <utility>

#include <variant/bad_variant_access.hpp>
#include <variant/detail/unsafe/visit.hpp>

namespace std {
namespace experimental {

namespace detail {

constexpr bool any_of(initializer_list<bool> bs) {
  for (bool b : bs) {
    if (b) {
      return true;
    }  // if
  }  // for
  return false;
}

}  // namespace detail

template <typename F, typename... Vs>
constexpr decltype(auto) visit(F &&f, Vs &&... vs) {
  using namespace detail;
  if (any_of({vs.corrupted_by_exception()...})) {
    throw bad_variant_access{};
  }  // if
  return unsafe::visit(forward<F>(f), forward<Vs>(vs)...);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_VISIT_HPP
