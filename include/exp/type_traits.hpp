#pragma once

#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>

#include <meta/meta.hpp>

namespace exp {

  namespace detail {

    template <typename List>
    struct best_match;

    template <>
    struct best_match<meta::list<>> {
      best_match() = default;
      void operator()() const;
    };  // best_match<meta::list<>>

    template <typename T, typename... Ts>
    struct best_match<meta::list<T, Ts...>> : best_match<meta::list<Ts...>> {
      using super = best_match<meta::list<Ts...>>;
      best_match() = default;

      using super::super;

      best_match(T);

      template <typename U = T,
                typename = std::enable_if_t<std::is_lvalue_reference<U>{}>>
      best_match(std::remove_reference_t<T> &&) = delete;

      using super::operator();

      meta::id<T> operator()(T) const;

      template <typename U = T,
                typename = std::enable_if_t<std::is_lvalue_reference<U>{}>>
      void operator()(std::remove_reference_t<T> &&) const = delete;

    };  // best_match<meta::list<T, Ts...>>

  }  // namespace detail

  /* `has_best_match` */
  template <typename List, typename T>
  struct has_best_match : std::is_convertible<T, detail::best_match<List>> {};

  /* `get_best_match` */
  template <typename List, typename T>
  using get_best_match =
      meta::_t<std::result_of_t<detail::best_match<List>(T)>>;

  /* `false_t` */
  template <typename... Ts>
  struct false_t : std::false_type {};

  /* `is_nothrow_swappable` */

  template <typename T, typename U = T>
  struct is_nothrow_swappable
      : meta::bool_<noexcept(
            std::iter_swap(std::declval<T *>(), std::declval<U *>()))> {};

  /* `priority` */

  template <std::size_t I>
  struct priority;

  template <>
  struct priority<0> {};

  template <std::size_t I>
  struct priority : priority<I - 1> {};

  /* `std::rank` for `std::array` */

  template <typename T>
  struct rank : meta::size_t<0> {};

  template <typename T, std::size_t N>
  struct rank<std::array<T, N>> : meta::size_t<rank<T>{} + 1> {};

  /* `same_type` (similar to `std::common_type`) */

  template <typename... Ts>
  struct same_type;

  template <>
  struct same_type<> {};

  template <typename T, typename... Ts>
  struct same_type<T, Ts...>
      : std::enable_if<(std::is_same<T, Ts>{} && ...), T> {};

}  // namespace exp
