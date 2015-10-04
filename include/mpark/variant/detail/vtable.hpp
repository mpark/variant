#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include <exp/functional.hpp>
#include <exp/return.hpp>
#include <exp/type_traits.hpp>
#include <meta/meta.hpp>

#include <mpark/variant/unsafe/get.hpp>

namespace mpark {

  namespace detail {

    namespace variant {

      /* `as_index_sequence` */

      template <typename List>
      struct as_index_sequence_;

      template <typename... Ts>
      struct as_index_sequence_<meta::list<Ts...>>
          : meta::id<std::make_index_sequence<sizeof...(Ts)>> {};

      template <typename List>
      using as_index_sequence = meta::_t<as_index_sequence_<List>>;

      /* `at` */

      template <typename T>
      constexpr const T &at_(const T &elem, const std::size_t *) {
        return elem;
      }

      template <typename T, std::size_t N>
      constexpr const auto &at_(const std::array<T, N> &array,
                                const std::size_t *index) {
        return at_(array[*index], index + 1);
      }

      template <typename T, std::size_t N>
      constexpr const auto &at(
          const std::array<T, N> &array,
          const std::size_t(&indices)[exp::rank<std::array<T, N>>{}]) {
        return at_(array, std::begin(indices));
      }

      /* `make_array` */

      template <typename... Ts, typename T = meta::_t<exp::same_type<Ts...>>>
      constexpr auto make_array_(exp::priority<1>, Ts &&... ts) {
        return std::array<T, sizeof...(Ts)>{{std::forward<Ts>(ts)...}};
      }

      template <typename... Ts>
      constexpr auto make_array_(exp::priority<0>, Ts &&...) {
        static_assert(exp::false_t<Ts...>{},
                      "The return types of all handlers must be equivalent for "
                      "return type deduction of `type_switch`.");
      }

      template <typename... Ts>
      constexpr decltype(auto) make_array(Ts &&... ts) {
        return make_array_(exp::priority<1>{}, std::forward<Ts>(ts)...);
      }

      /* `deduce_t` */
      struct deduce_t {};

      /* `invoker` */

      template <typename R>
      struct invoker;

      template <>
      struct invoker<deduce_t> {
        template <std::size_t... Is, typename F, typename... Vs>
        static constexpr auto invoke(exp::priority<1>,
                                     std::index_sequence<Is...>,
                                     F &&f,
                                     Vs &&... vs)
          RETURN(std::forward<F>(f).template operator()<Is...>(
              unsafe::get<Is>(std::forward<Vs>(vs))...))

        template <std::size_t... Is, typename F, typename... Vs>
        static constexpr decltype(auto) invoke(exp::priority<0>,
                                               std::index_sequence<Is...>,
                                               F &&f,
                                               Vs &&... vs) {
          return exp::invoke(std::forward<F>(f),
                             unsafe::get<Is>(std::forward<Vs>(vs))...);
        }
      };  // invoker<deduce_t>

      template <typename R>
      struct invoker {
        template <std::size_t... Is, typename F, typename... Vs>
        static constexpr auto invoke(exp::priority<1>,
                                     std::index_sequence<Is...>,
                                     F &&f,
                                     Vs &&... vs)
          RETURN_TYPE(R, std::forward<F>(f).template operator()<Is...>(
              unsafe::get<Is>(std::forward<Vs>(vs))...))

        template <std::size_t... Is, typename F, typename... Vs>
        static constexpr R invoke(exp::priority<0>,
                                  std::index_sequence<Is...>,
                                  F &&f,
                                  Vs &&... vs) {
          return exp::invoke(std::forward<F>(f),
                             unsafe::get<Is>(std::forward<Vs>(vs))...);
        }
      };  // invoker

      /* `make_vtable` */

      template <typename... Vs>
      struct make_vtable_ {
        template <typename R, typename F, std::size_t... Is>
        static constexpr decltype(auto) invoke(F f, Vs... vs) {
          return invoker<R>::template invoke(exp::priority<1>{},
                                             std::index_sequence<Is...>{},
                                             static_cast<F>(f),
                                             static_cast<Vs>(vs)...);
        }

        template <typename R, typename F, std::size_t... Is>
        static constexpr auto make_vtable(std::index_sequence<Is...>) {
          return &invoke<R, F, Is...>;
        }

        template <typename R,
                  typename F,
                  std::size_t... Is,
                  std::size_t... Js,
                  typename... Ls>
        static constexpr auto make_vtable(std::index_sequence<Is...>,
                                          std::index_sequence<Js...>,
                                          Ls... ls) {
          return make_array(
              make_vtable<R, F>(std::index_sequence<Is..., Js>{}, ls...)...);
        }

      };  // make_vtable_

      template <typename R, typename F, typename... Vs>
      constexpr auto make_vtable() {
        return make_vtable_<Vs...>::template make_vtable<R, F>(
            std::index_sequence<>{},
            as_index_sequence<meta::as_list<std::decay_t<Vs>>>{}...);
      }

    }  // namespace variant

  }  // namespace detail

}  // namespace mpark
