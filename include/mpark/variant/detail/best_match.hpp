#pragma once

#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      template <typename Ts>
      struct best_match;

      template <>
      struct best_match<meta::list<>> {
        void operator()() const;
      };

      template <typename T, typename... Ts>
      struct best_match<meta::list<T, Ts...>> : best_match<meta::list<Ts...>> {
        using best_match<meta::list<Ts...>>::operator();
        meta::id<T> operator()(const T &) const;
        meta::id<T> operator()(T &&) const;
      };

      template <typename Ts, typename T>
      using get_best_match =
          meta::_t<decltype(best_match<Ts>()(std::declval<T>()))>;

      template <typename Ts, typename T, typename = meta::void_<>>
      struct has_best_match : std::false_type {};

      template <typename Ts, typename T>
      struct has_best_match<Ts, T, meta::void_<get_best_match<Ts, T>>>
          : std::true_type {};

    }  // variant

  }  // detail

}  // mpark
