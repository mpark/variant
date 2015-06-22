#pragma once

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      /* `std::invoke` (since C++17). */

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

      template <typename F, typename... As>
      auto invoke(F &&f, As &&... as)
          RETURN(std::forward<F>(f)(std::forward<As>(as)...));

      template <typename B, typename T, typename D>
      auto invoke(T B::*pmv, D &&d)
          RETURN(std::forward<D>(d).*pmv);

      template <typename Pmv, typename Ptr>
      auto invoke(Pmv pmv, Ptr &&ptr)
          RETURN((*std::forward<Ptr>(ptr)).*pmv);

      template <typename B, typename T, typename D, typename... As>
      auto invoke(T B::*pmf, D &&d, As &&... as)
          RETURN((std::forward<D>(d).*pmf)(std::forward<As>(as)...));

      template <typename Pmf, typename Ptr, typename... As>
      auto invoke(Pmf pmf, Ptr &&ptr, As &&... as)
          RETURN(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<As>(as)...));

#undef RETURN

      /* `std::make_array` (N3824). */

      template <typename T = void, typename... Ts>
      constexpr auto make_array(Ts &&... ts) {
        using U = std::conditional_t<std::is_void<T>{},
                                     std::common_type_t<std::decay_t<Ts>...>,
                                     T>;
        return std::array<U, sizeof...(Ts)>{{std::forward<Ts>(ts)...}};
      }

      /* `std::rank` (for `std::array`). */

      template <typename T>
      struct rank : public std::integral_constant<std::size_t, 0> {};

      template <typename T, std::size_t N>
      struct rank<std::array<T, N>>
          : public std::integral_constant<std::size_t, rank<T>{} + 1> {};

      /* `at` */

      template <typename T>
      constexpr const T &at_impl(const T &elem, const std::size_t *) {
        return elem;
      }

      template <typename T, std::size_t N>
      constexpr const auto &at_impl(const std::array<T, N> &array,
                                     const std::size_t *index) {
        return at_impl(array[*index], index + 1);
      }

      template <typename T, std::size_t N>
      constexpr const auto &at(
          const std::array<T, N> &array,
          const std::size_t (&indices)[rank<std::array<T, N>>{}]) {
        return at_impl(array, std::begin(indices));
      }

      /* `make_vtable` */

      template <typename F, typename... Vs>
      struct make_vtable_impl {
        private:

        template <typename... Ts>
        static decltype(auto) visit(F f, Vs... vs) {
          return invoke(static_cast<F>(f),
                        static_cast<Vs>(vs).get(meta::id<Ts>{})...);
        }

        template <typename Ts, typename... Us>
        struct impl;

        template <typename... Ts>
        struct impl<meta::list<Ts...>> {
          constexpr auto operator()() const { return &visit<Ts...>; }
        };

        template <typename... Ts, typename... U, typename... Us>
        struct impl<meta::list<Ts...>, meta::list<U...>, Us...> {
          constexpr auto operator()() const {
            return make_array(impl<meta::list<Ts..., U>, Us...>()()...);
          }
        };

        public:

        constexpr auto operator()() const {
          return impl<meta::list<>, meta::as_list<std::decay_t<Vs>>...>()();
        }

      };  // make_vtable_impl

      template <typename F, typename... Vs>
      constexpr auto make_vtable() {
        return make_vtable_impl<F, Vs...>()();
      };

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark
