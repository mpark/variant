// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef VARIANT_SWAP_HPP
#define VARIANT_SWAP_HPP

namespace std {
namespace experimental {

//- 20.N.M swap support:

template <typename... Ts>
void swap(variant<Ts...> &lhs,
          variant<Ts...> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

}  // namespace experimental
}  // namespace std

#endif  // VARIANT_SWAP_HPP
