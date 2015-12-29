// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EXPERIMENTAL_VARIANT_DETAIL_INVOKE_HPP
#define EXPERIMENTAL_VARIANT_DETAIL_INVOKE_HPP

#include <utility>

namespace std {
namespace experimental {
namespace detail {

/* `std::invoke` */

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

template <typename F, typename... As>
constexpr auto invoke(F &&f, As &&... as)
  RETURN(forward<F>(f)(forward<As>(as)...))

template <typename B, typename T, typename D>
constexpr auto invoke(T B::*pmv, D &&d)
  RETURN(forward<D>(d).*pmv)

template <typename Pmv, typename Ptr>
constexpr auto invoke(Pmv pmv, Ptr &&ptr)
  RETURN((*forward<Ptr>(ptr)).*pmv)

template <typename B, typename T, typename D, typename... As>
constexpr auto invoke(T B::*pmf, D &&d, As &&... as)
  RETURN((forward<D>(d).*pmf)(forward<As>(as)...))

template <typename Pmf, typename Ptr, typename... As>
constexpr auto invoke(Pmf pmf, Ptr &&ptr, As &&... as)
  RETURN(((*forward<Ptr>(ptr)).*pmf)(forward<As>(as)...))

#undef RETURN

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // EXPERIMENTAL_VARIANT_DETAIL_INVOKE_HPP
