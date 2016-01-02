// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_MONOSTATE_HPP
#define EXPERIMENTAL_MONOSTATE_HPP

#include <cstddef>
#include <functional>

namespace std {
namespace experimental {
inline namespace fundamentals_v3 {

struct monostate {};

constexpr bool operator< (monostate, monostate) { return false; }
constexpr bool operator> (monostate, monostate) { return false; }
constexpr bool operator<=(monostate, monostate) { return true;  }
constexpr bool operator>=(monostate, monostate) { return true;  }
constexpr bool operator==(monostate, monostate) { return true;  }
constexpr bool operator!=(monostate, monostate) { return false; }

}  // namespace fundamentals_v3
}  // namespace experimental

template <>
struct hash<experimental::monostate> {
  using argument_type = experimental::monostate;
  using result_type = size_t;
  result_type operator()(const argument_type &) const { return 0u; }
};  // hash<experimental::monostate>

}  // namespace std

#endif  // EXPERIMENTAL_MONOSTATE_HPP
