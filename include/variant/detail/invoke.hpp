#ifndef VARIANT_DETAIL_INVOKE_HPP
#define VARIANT_DETAIL_INVOKE_HPP

#include <utility>

namespace std {
namespace experimental {
namespace detail {

/* `std::invoke` */

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

template <typename F, typename... As>
auto invoke(F &&f, As &&... as) RETURN(forward<F>(f)(forward<As>(as)...))

template <typename B, typename T, typename D>
auto invoke(T B::*pmv, D &&d) RETURN(forward<D>(d).*pmv)

template <typename Pmv, typename Ptr>
auto invoke(Pmv pmv, Ptr &&ptr) RETURN((*forward<Ptr>(ptr)).*pmv)

template <typename B, typename T, typename D, typename... As>
auto invoke(T B::*pmf, D &&d, As &&... as)
  RETURN((forward<D>(d).*pmf)(forward<As>(as)...))

template <typename Pmf, typename Ptr, typename... As>
auto invoke(Pmf pmf, Ptr &&ptr, As &&... as)
  RETURN(((*forward<Ptr>(ptr)).*pmf)(forward<As>(as)...))

#undef RETURN

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_INVOKE_HPP
