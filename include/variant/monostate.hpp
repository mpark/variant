// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_MONOSTATE_HPP
#define VARIANT_MONOSTATE_HPP

namespace std {
namespace experimental {

//- 20.N.M monostate:

struct monostate {};

/* relation operators for `monostate`. */
constexpr bool operator==(monostate, monostate) { return true; }
constexpr bool operator!=(monostate, monostate) { return false; }
constexpr bool operator<(monostate, monostate) { return false; }
constexpr bool operator>(monostate, monostate) { return false; }
constexpr bool operator<=(monostate, monostate) { return true; }
constexpr bool operator>=(monostate, monostate) { return true; }

}  // namespace experimental
}  // namespace std

namespace std {

//- 20.N.M hash support for `monostate`:

template <>
struct hash<experimental::monostate> {
  using argument_type = experimental::monostate;
  using result_type = size_t;

  result_type operator()(const argument_type &) const { return 0u; }
};  // hash<experimental::monostate>

}  // namespace std

#endif  // VARIANT_MONOSTATE_HPP
