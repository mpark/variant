// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_REL_HPP
#define VARIANT_REL_HPP

#include <cstddef>
#include <cstdlib>

#include <variant/detail/index_visitor.hpp>
#include <variant/visit.hpp>

namespace std {
namespace experimental {
namespace detail {

struct equal {
  template <typename T>
  constexpr bool operator()(const T &lhs, const T &rhs) const {
    return lhs == rhs;
  }

  template <typename T, typename U>
  constexpr bool operator()(const T &, const U &) const {
    assert(false);
    return false;
  }
};  // equal

struct less {
  template <typename T>
  constexpr bool operator()(const T &lhs, const T &rhs) const {
    return lhs < rhs;
  }

  template <typename T, typename U>
  constexpr bool operator()(const T &, const U &) const {
    assert(false);
    return false;
  }
};  // less

}  // namespace detail

//- 20.N.8 relational operators:

template <typename... Ts>
constexpr bool operator==(const variant<Ts...> &lhs,
                          const variant<Ts...> &rhs) {
  using namespace detail;
  return lhs.index() == rhs.index() && unsafe::visit(detail::equal{}, lhs, rhs);
}

template <typename... Ts>
constexpr bool operator!=(const variant<Ts...> &lhs,
                          const variant<Ts...> &rhs) {
  return !(lhs == rhs);
}

template <typename... Ts>
constexpr bool operator<(const variant<Ts...> &lhs,
                         const variant<Ts...> &rhs) {
  using namespace detail;
  return lhs.index() < rhs.index() || (lhs.index() == rhs.index() &&
                                       unsafe::visit(detail::less{}, lhs, rhs));
}

template <typename... Ts>
constexpr bool operator>(const variant<Ts...> &lhs,
                         const variant<Ts...> &rhs) {
  return rhs < lhs;
}

template <typename... Ts>
constexpr bool operator<=(const variant<Ts...> &lhs,
                          const variant<Ts...> &rhs) {
  return !(lhs > rhs);
}

template <typename... Ts>
constexpr bool operator>=(const variant<Ts...> &lhs,
                          const variant<Ts...> &rhs) {
  return !(lhs < rhs);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_REL_HPP
