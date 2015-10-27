#ifndef VARIANT_DETAIL_OVERLOAD_HPP
#define VARIANT_DETAIL_OVERLOAD_HPP

#include <type_traits>
#include <utility>

namespace std {
namespace experimental {
namespace detail {

/* A class that inherits from function objects and
            results in an overloaded callable object. */
template <typename F, typename... Fs>
struct overload_;

/* Base case. */
template <typename F>
struct overload_<F> : decay_t<F> {
  using decay_t<F>::operator();

  overload_(F f) : decay_t<F>(static_cast<F>(f)) {}
};  // overload_<F>

/* Recursive case. */
template <typename F, typename... Fs>
struct overload_ : decay_t<F>, overload_<Fs...> {
  using decay_t<F>::operator();
  using overload_<Fs...>::operator();

  overload_(F f, Fs... fs)
      : decay_t<F>(static_cast<F>(f)),
        overload_<Fs...>(static_cast<Fs>(fs)...) {}
};  // overload_<F, Fs...>

/* Factory function for overload. */
template <typename... Fs>
overload_<Fs &&...> overload(Fs &&... fs) {
  return {forward<Fs>(fs)...};
}

}  // namespace detail
}  // namespace experimental
}  // namespace std

#endif  // VARIANT_DETAIL_OVERLOAD_HPP
