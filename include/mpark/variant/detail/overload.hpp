#pragma once

#include <type_traits>
#include <utility>

namespace mpark {

  namespace detail {

    namespace variant {

      /* A class that inherits from function objects and
         results in an overloaded callable object. */
      template <typename F, typename... Fs>
      struct overload_;

      /* Base case. */
      template <typename F>
      struct overload_<F> : std::decay_t<F> {
        using std::decay_t<F>::operator();

        explicit overload_(F f) : std::decay_t<F>(static_cast<F>(f)) {}
      };  // overload_<F>

      /* Recursive case. */
      template <typename F, typename... Fs>
      struct overload_ : std::decay_t<F>, overload_<Fs...> {
        using std::decay_t<F>::operator();
        using overload_<Fs...>::operator();

        explicit overload_(F f, Fs... fs)
            : std::decay_t<F>(static_cast<F>(f)),
              overload_<Fs...>(static_cast<Fs>(fs)...) {}
      };  // overload_<F, Fs...>

      /* Factory function for overload. */
      template <typename... Fs>
      auto overload(Fs &&... fs) {
        return overload_<Fs &&...>{std::forward<Fs>(fs)...};
      }

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark
