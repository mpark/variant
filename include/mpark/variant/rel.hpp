#pragma once

#include <cstddef>

#include <mpark/variant/nullvar.hpp>
#include <mpark/variant/type_switch.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      struct equal {
        template <typename T>
        constexpr bool operator()(const T &lhs, const T &rhs) const {
          return lhs == rhs;
        }

        template <typename T, typename U>
        constexpr bool operator()(const T &, const U &) const {
          return false;
        }
      };  // equal

      struct less {
        template <typename T>
        constexpr bool operator()(const T &lhs, const T &rhs) const {
          return lhs < rhs;
        }

        template <std::size_t I, std::size_t J, typename T, typename U>
        constexpr bool operator()(const T &, const U &) const {
          return I < J;
        }
      };  // less

    }  // namespace variant

  }  // namespace detail

  //- 20.N.8 relational operators:

  constexpr bool operator==(nullvar_t, nullvar_t) { return true; }
  constexpr bool operator!=(nullvar_t, nullvar_t) { return false; }
  constexpr bool operator<(nullvar_t, nullvar_t) { return false; }
  constexpr bool operator>(nullvar_t, nullvar_t) { return false; }
  constexpr bool operator<=(nullvar_t, nullvar_t) { return true; }
  constexpr bool operator>=(nullvar_t, nullvar_t) { return true; }

  template <typename... Ts>
  constexpr bool operator==(const variant<Ts...> &lhs,
                            const variant<Ts...> &rhs) {
    return type_switch(lhs, rhs)(detail::variant::equal{});
  }

  template <typename... Ts>
  constexpr bool operator!=(const variant<Ts...> &lhs,
                            const variant<Ts...> &rhs) {
    return !(lhs == rhs);
  }

  template <typename... Ts>
  constexpr bool operator<(const variant<Ts...> &lhs,
                           const variant<Ts...> &rhs) {
    return type_switch(lhs, rhs)(detail::variant::less{});
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

}  // namespace mpark
