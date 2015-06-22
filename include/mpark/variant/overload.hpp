#pragma once

#include <type_traits>
#include <utility>

namespace mpark {

  /* A class that inherits from function objects and
     results in an overloaded callable object. */
  template <typename... Fns>
  struct overload;

  /* Base case. */
  template <typename Fn>
  struct overload<Fn> : std::decay_t<Fn> {

    using std::decay_t<Fn>::operator();

    explicit overload(Fn fn) : std::decay_t<Fn>(static_cast<Fn>(fn)) {}

  };  // overload<Fn>

  /* Recursive case. */
  template <typename Fn, typename... Fns>
  struct overload<Fn, Fns...> : std::decay_t<Fn>, overload<Fns...> {

    using std::decay_t<Fn>::operator();
    using overload<Fns...>::operator();

    /* Cache the function object. */
    explicit overload(Fn fn, Fns... fns)
        : std::decay_t<Fn>(static_cast<Fn>(fn)),
          overload<Fns...>(static_cast<Fns>(fns)...) {}

  };  // overload<Fn, Fns...>

  /* Factory function for overload. */
  template <typename... Fns>
  auto make_overload(Fns &&... fns) {
    return overload<Fns &&...>(std::forward<Fns>(fns)...);
  }

}  // namespace mpark
