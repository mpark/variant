// MPark.Variant
//
// Copyright Michael Park, 2015-2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MPARK_VARIANTS_LIB_HPP
#define MPARK_VARIANTS_LIB_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mpark/variants/config.hpp>

#define RETURN(...)                                          \
  noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { \
    return __VA_ARGS__;                                      \
  }

namespace mpark {
  namespace variants {
    namespace lib {

      template <typename T>
      struct identity { using type = T; };

      inline namespace cpp14 {
        template <typename T, std::size_t N>
        struct array {
          constexpr const T &operator[](std::size_t index) const {
            return data[index];
          }

          T data[N > 0 ? N : 1];
        };

        template <typename T>
        using add_pointer_t = typename std::add_pointer<T>::type;

        template <typename... Ts>
        using common_type_t = typename std::common_type<Ts...>::type;

        template <typename T>
        using decay_t = typename std::decay<T>::type;

        template <bool B, typename T = void>
        using enable_if_t = typename std::enable_if<B, T>::type;

        template <typename T>
        using remove_const_t = typename std::remove_const<T>::type;

#ifdef MPARK_INTEGER_SEQUENCE
        template <std::size_t... Is>
        using index_sequence = std::index_sequence<Is...>;

        template <std::size_t N>
        using make_index_sequence = std::make_index_sequence<N>;

        template <typename... Ts>
        using index_sequence_for = std::index_sequence_for<Ts...>;
#else
        template <std::size_t...>
        struct index_sequence {};

        template <typename Lhs, typename Rhs>
        struct make_index_sequence_concat;

        template <std::size_t... Lhs, std::size_t... Rhs>
        struct make_index_sequence_concat<index_sequence<Lhs...>,
                                          index_sequence<Rhs...>>
            : identity<index_sequence<Lhs..., (sizeof...(Lhs) + Rhs)...>> {};

        template <std::size_t N>
        struct make_index_sequence_impl;

        template <std::size_t N>
        using make_index_sequence = typename make_index_sequence_impl<N>::type;

        template <std::size_t N>
        struct make_index_sequence_impl
            : make_index_sequence_concat<make_index_sequence<N / 2>,
                                         make_index_sequence<N - (N / 2)>> {};

        template <>
        struct make_index_sequence_impl<0> : identity<index_sequence<>> {};

        template <>
        struct make_index_sequence_impl<1> : identity<index_sequence<0>> {};

        template <typename... Ts>
        using index_sequence_for = make_index_sequence<sizeof...(Ts)>;
#endif

        // <functional>
#ifdef MPARK_TRANSPARENT_OPERATORS
        using equal_to = std::equal_to<>;
#else
        struct equal_to {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) == std::forward<Rhs>(rhs))
        };
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
        using not_equal_to = std::not_equal_to<>;
#else
        struct not_equal_to {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) != std::forward<Rhs>(rhs))
        };
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
        using less = std::less<>;
#else
        struct less {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) < std::forward<Rhs>(rhs))
        };
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
        using greater = std::greater<>;
#else
        struct greater {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) > std::forward<Rhs>(rhs))
        };
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
        using less_equal = std::less_equal<>;
#else
        struct less_equal {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) <= std::forward<Rhs>(rhs))
        };
#endif

#ifdef MPARK_TRANSPARENT_OPERATORS
        using greater_equal = std::greater_equal<>;
#else
        struct greater_equal {
          template <typename Lhs, typename Rhs>
          inline constexpr auto operator()(Lhs &&lhs, Rhs &&rhs) const
            RETURN(std::forward<Lhs>(lhs) >= std::forward<Rhs>(rhs))
        };
#endif
      }  // namespace cpp14

      inline namespace cpp17 {

        // <type_traits>
        template <bool B>
        using bool_constant = std::integral_constant<bool, B>;

        template <typename...>
        using void_t = void;

        namespace detail {
          namespace swappable {

            using std::swap;

            template <typename T>
            struct is_swappable_impl {
              private:
              template <typename U,
                        typename = decltype(swap(std::declval<U &>(),
                                                 std::declval<U &>()))>
              inline static std::true_type test(int);

              template <typename U>
              inline static std::false_type test(...);

              public:
              using type = decltype(test<T>(0));
            };

            template <typename T>
            using is_swappable = typename is_swappable_impl<T>::type;

            template <typename T, bool = is_swappable<T>::value>
            struct is_nothrow_swappable
                : bool_constant<noexcept(
                      swap(std::declval<T &>(), std::declval<T &>()))> {};

            template <typename T>
            struct is_nothrow_swappable<T, false> : std::false_type {};

          }  // namespace swappable
        }  // namespace detail

        template <typename T>
        using is_swappable = detail::swappable::is_swappable<T>;

        template <typename T>
        using is_nothrow_swappable = detail::swappable::is_nothrow_swappable<T>;

        // <functional>
        template <typename F, typename... As>
        inline constexpr auto invoke(F &&f, As &&... as)
            RETURN(std::forward<F>(f)(std::forward<As>(as)...))

        template <typename B, typename T, typename D>
        inline constexpr auto invoke(T B::*pmv, D &&d)
            RETURN(std::forward<D>(d).*pmv)

        template <typename Pmv, typename Ptr>
        inline constexpr auto invoke(Pmv pmv, Ptr &&ptr)
            RETURN((*std::forward<Ptr>(ptr)).*pmv)

        template <typename B, typename T, typename D, typename... As>
        inline constexpr auto invoke(T B::*pmf, D &&d, As &&... as)
            RETURN((std::forward<D>(d).*pmf)(std::forward<As>(as)...))

        template <typename Pmf, typename Ptr, typename... As>
        inline constexpr auto invoke(Pmf pmf, Ptr &&ptr, As &&... as)
            RETURN(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<As>(as)...))

        namespace detail {

          template <typename Void, typename, typename...>
          struct invoke_result {};

          template <typename F, typename... Args>
          struct invoke_result<void_t<decltype(lib::invoke(
                                   std::declval<F>(), std::declval<Args>()...))>,
                               F,
                               Args...>
              : identity<decltype(
                    lib::invoke(std::declval<F>(), std::declval<Args>()...))> {};

        }  // namespace detail

        template <typename F, typename... Args>
        using invoke_result = detail::invoke_result<void, F, Args...>;

        template <typename F, typename... Args>
        using invoke_result_t = typename invoke_result<F, Args...>::type;

        namespace detail {

          template <typename Void, typename, typename...>
          struct is_invocable : std::false_type {};

          template <typename F, typename... Args>
          struct is_invocable<void_t<invoke_result_t<F, Args...>>, F, Args...>
              : std::true_type {};

          template <typename Void, typename, typename, typename...>
          struct is_invocable_r : std::false_type {};

          template <typename R, typename F, typename... Args>
          struct is_invocable_r<void_t<invoke_result_t<F, Args...>>,
                                R,
                                F,
                                Args...>
              : std::is_convertible<invoke_result_t<F, Args...>, R> {};

        }  // namespace detail

        template <typename F, typename... Args>
        using is_invocable = detail::is_invocable<void, F, Args...>;

        template <typename R, typename F, typename... Args>
        using is_invocable_r = detail::is_invocable_r<void, R, F, Args...>;

        // <memory>
#ifdef MPARK_BUILTIN_ADDRESSOF
        template <typename T>
        inline constexpr T *addressof(T &arg) {
          return __builtin_addressof(arg);
        }
#else
        namespace detail {

          namespace has_addressof_impl {

            struct fail;

            template <typename T>
            inline fail operator&(T &&);

            template <typename T>
            inline static constexpr bool impl() {
              return (std::is_class<T>::value || std::is_union<T>::value) &&
                     !std::is_same<decltype(&std::declval<T &>()), fail>::value;
            }

          }  // namespace has_addressof_impl

          template <typename T>
          using has_addressof = bool_constant<has_addressof_impl::impl<T>()>;

        }  // namespace detail

        template <typename T,
                  lib::enable_if_t<detail::has_addressof<T>::value, int> = 0>
        inline T *addressof(T &arg) {
          return std::addressof(arg);
        }

        template <typename T,
                  lib::enable_if_t<!detail::has_addressof<T>::value, int> = 0>
        inline constexpr T *addressof(T &arg) {
          return &arg;
        }
#endif

        template <typename T>
        inline const T *addressof(const T &&) = delete;

      }  // namespace cpp17

      template <typename T>
      struct remove_all_extents : identity<T> {};

      template <typename T, std::size_t N>
      struct remove_all_extents<array<T, N>> : remove_all_extents<T> {};

      template <typename T>
      using remove_all_extents_t = typename remove_all_extents<T>::type;

      template <std::size_t N>
      using size_constant = std::integral_constant<std::size_t, N>;

#ifdef MPARK_TYPE_PACK_ELEMENT
      template <std::size_t I, typename... Ts>
      using type_pack_element_t = __type_pack_element<I, Ts...>;
#else
      template <std::size_t I, typename... Ts>
      struct type_pack_element_impl {
        private:
        template <std::size_t, typename T>
        struct indexed_type : identity<T> {};

        template <typename>
        struct set;

        template <std::size_t... Is>
        struct set<index_sequence<Is...>> : indexed_type<Is, Ts>... {};

        template <typename T>
        inline static std::enable_if<true, T> impl(indexed_type<I, T>);

        inline static std::enable_if<false> impl(...);

        public:
        using type = decltype(impl(set<index_sequence_for<Ts...>>{}));
      };

      template <std::size_t I, typename... Ts>
      using type_pack_element = typename type_pack_element_impl<I, Ts...>::type;

      template <std::size_t I, typename... Ts>
      using type_pack_element_t = typename type_pack_element<I, Ts...>::type;
#endif

    }  // namespace lib
  }  // namespace variants
}  // namespace mpark

#undef RETURN

#endif  // MPARK_VARIANTS_LIB_HPP
